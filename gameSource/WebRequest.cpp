#include "WebRequest.h"

#include "minorGems/util/stringUtils.h"
#include "minorGems/util/StringBufferOutputStream.h"

#include "minorGems/network/SocketClient.h"



WebRequest::WebRequest( char *inMethod, char *inURL,
                        char *inBody )
        : mError( false ), mRequest( NULL ), mRequestPosition( -1 ),
          mResultReady( false ), mResult( NULL ),
          mSock( NULL ) {
        
    
    
    char *startString = "http://";

    char *urlCopy = stringDuplicate( inURL );

    
    char *urlStart = stringLocateIgnoreCase(  urlCopy, startString );

    char *serverStart;
    
    if( urlStart == NULL ) {
        // no http:// at start of URL
        serverStart = urlCopy;
        }
    else {
        serverStart = &( urlStart[ strlen( startString ) ] );
        }
    
    // find the first occurrence of "/", which is the end of the
    // server name

    char *serverNameCopy = stringDuplicate( serverStart );
        
    char *serverEnd = strstr( serverNameCopy, "/" );

    char *getPath = strstr( serverStart, "/" );

        
    if( serverEnd == NULL ) {
        serverEnd = &( serverStart[ strlen( serverStart ) ] );
        getPath = "/";
        }
    // terminate the url here to extract the server name
    serverEnd[0] = '\0';

    int portNumber = 80;

        // look for a port number
    char *colon = strstr( serverNameCopy, ":" );
    if( colon != NULL ) {
        char *portNumberString = &( colon[1] );
                
        int numRead = sscanf( portNumberString, "%d",
                              & portNumber );
        if( numRead != 1 ) {
            portNumber = 80;
            }

        // terminate the name here so port isn't taken as part
        // of the address
        colon[0] = '\0';
        }

    mSuppliedAddress = new HostAddress(
        stringDuplicate( serverNameCopy ),
        portNumber );

    mNumericalAddress = NULL;
    
    mLookupThread = new LookupThread( mSuppliedAddress );
    

    mSock = NULL;
    
        
    // compose the request into a buffered stream
    StringBufferOutputStream tempStream;

    tempStream.writeString( inMethod );
    tempStream.writeString( " " );
    tempStream.writeString( getPath );
    tempStream.writeString( " HTTP/1.0\r\n" );
    tempStream.writeString( "Host: " );
    tempStream.writeString( serverNameCopy );
    tempStream.writeString( "\r\n" );
        
    if( inBody != NULL ) {
        char *lengthString = autoSprintf( "Content-Length: %d\r\n",
                                          strlen( inBody ) );
        tempStream.writeString( lengthString );
        delete [] lengthString;
        tempStream.writeString(
            "Content-Type: application/x-www-form-urlencoded\r\n\r\n" );
            
        tempStream.writeString( inBody );
        }
    else {
        tempStream.writeString( "\r\n" );
        }
        
    mRequest = tempStream.getString();
    mRequestPosition = 0;

        
        
    delete [] serverNameCopy;

    delete [] urlCopy;    
    }



WebRequest::~WebRequest() {

    delete mLookupThread;

    delete mSuppliedAddress;
    
    if( mNumericalAddress != NULL ) {
        delete mNumericalAddress;
        }
    
    

    if( mSock != NULL ) {
        delete mSock;
        }
    
    if( mRequest != NULL ) {
        delete [] mRequest;
        }
    
    if( mResult != NULL ) {
        delete [] mResult;
        }
    }



int WebRequest::step() {
    if( mError ) {
        return -1;
        }

    if( mSock == NULL ) {
                
        if( mLookupThread->isLookupDone() ) {
        

            mError = true;

            mNumericalAddress = mLookupThread->getResult();
            

            if( mNumericalAddress != NULL ) {
                

    
                // use timeout of 0 for non-blocking
                // will be set to true if we time out while connecting
                char timedOut;
                
                mSock = SocketClient::connectToServer( mNumericalAddress,
                                                       0,
                                                       &timedOut );
                
                if( mSock != NULL ) {
                    
                    mError = false;
                    }
                }

            if( mError ) {
                // lookup or socket construction failed
                return -1;
                }            
            }
        else {
            // still looking up
            return 0;
            }
        }
    

    int connectStatus = mSock->isConnected();
    
    if( connectStatus == 0 ) {
        // still trying to connect
        return 0;
        }
    else if( connectStatus < 0 ) {
        // failed to connect
        mError = true;
        
        return -1;
        }
    else if( connectStatus == 1 ) {
        // connected
        
        if( mRequestPosition < (int)( strlen( mRequest ) ) ) {
            // try to send more
            char *remainingRequest = &( mRequest[ mRequestPosition ] );
            
            int numSent = mSock->send( (unsigned char *)remainingRequest,
                                       strlen( remainingRequest ),
                                       // non-blocking
                                       false );
            if( numSent == -1 ) {
                mError = true;
                return -1;
                }
            if( numSent == -2 ) {
                return 0;
                }
            
            mRequestPosition += numSent;
            

            // don't start looking for response in same step,
            // even if we just sent the entire request
            
            // in practice, it's never ready that fast
            return 0;
            }
        else if( mResultReady ) {
            return 1;
            }
        else {
            
            // done sending request
            // still receiving response
            
            long bufferLength = 5000;
            unsigned char *buffer = new unsigned char[ bufferLength ];

            // non-blocking

            // keep reading as long as we get full buffers
            int numRead = bufferLength;
            
            while( numRead > 0 ) {
                
                numRead = mSock->receive( buffer, bufferLength, 0 );
                
                if( numRead > 0 ) {
                    for( int i=0; i<numRead; i++ ) {
                        mResponse.push_back( buffer[i] );
                        }
                    }
                }
            
            
            delete [] buffer;
            

            if( numRead == -1 ) {
                // connection closed, done done receiving result

                // process it
                
                char *responseString = mResponse.getElementString();
                int responseLength = mResponse.size();
                
                if( stringLocateIgnoreCase( responseString, "404 Not Found" ) 
                    != NULL ) {
                    
                    mError = true;
                    delete [] responseString;
                    return -1;
                    }

                char *contentStartString = "\r\n\r\n";
                
                char *contentStart = strstr( responseString, 
                                             contentStartString );
            
                if( contentStart != NULL ) {
                    // skip start string
                    char *content =
                        &( contentStart[ strlen( contentStartString ) ] );


                    int resultLength =
                        responseLength
                        - strlen( contentStartString )
                        - ( (int)contentStart - (int)responseString );

                    mResult = new char[ resultLength + 1 ];
                    memcpy( mResult,
                            content, resultLength );

                    mResult[ resultLength ] = '\0';
                    mResultReady = true;
                    
                    delete [] responseString;
                    return 1;
                    }
                else {
                    mError = true;
                    delete [] responseString;
                    return -1;
                    }
                }
            else {
                // still receiving response
                return 0;
                }
            
            }
        }


    // should never get here
    // count it as error
    return -1;
    }

        

char *WebRequest::getResult() {
    if( mResultReady ) {
        return stringDuplicate( mResult );
        }
    else {
        return NULL;
        }
    }
