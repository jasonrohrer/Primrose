#include "HighScoreLoadingPanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"

#include <math.h>
#include <GL/gl.h>


Color scoreLoadingGreen( 0, 1, 0 );

Color scoreLoadingRed( 1, 0, 0 );


extern char *savedServerURL;



void HighScoreLoadingPanel::startConnectionTry() {
    mFailed = false;
    mStatusLight.setColor( scoreLoadingGreen.copy() );

    if( mServerURL == NULL && mServerURLFetchRequest == NULL ) {
        // try again
        mServerURLFetchRequest = new WebRequest( "GET",
                                                 mServerFinderURL,
                                                 NULL );
        }
    }



void HighScoreLoadingPanel::setFailed() {
    mFailed = true;
            
    mMessage = "connect failed";
    
    mStatusLight.setColor( scoreLoadingRed.copy() );
    }



HighScoreLoadingPanel::HighScoreLoadingPanel( int inW, int inH )
        : Panel( inW, inH ),
          mMessage( "loading scores" ),
          mStatusLight( inW / 2, inH / 2 ),
          mBlinkTime( 0 ),
          mScoreToPost( NULL ),
          mWebRequest( NULL ),
          mServerFinderURL(
            "http://hcsoftware.sourceforge.net/primrose/serverLocation.txt" ),
          mServerURL( NULL ), 
          mServerURLFetchRequest( NULL ),
          mFailed( false ),
          mDisplayPanel( inW, inH ) {
    
    if( savedServerURL != NULL ) {
        mServerURL = stringDuplicate( savedServerURL );
        }
    

    startConnectionTry();

    // do not manage as sub-panel
    // addSubPanel( &mDisplayPanel );
    }


    
HighScoreLoadingPanel::~HighScoreLoadingPanel() {
    if( mScoreToPost != NULL ) {
        delete mScoreToPost;
        }
    if( mWebRequest != NULL ) {
        delete mWebRequest;
        }

    if( mServerURL != NULL ) {
        delete [] mServerURL;
        }
    if( mServerURLFetchRequest != NULL ) {
        delete mServerURLFetchRequest;
        }
    
    }



void HighScoreLoadingPanel::setLoadingMessage() {
    mMessage = "loading scores";
    }



void HighScoreLoadingPanel::setScoreToPost( ScoreBundle *inBundle ) {
    if( mScoreToPost != NULL ) {
        delete mScoreToPost;
        }

    mScoreToPost = inBundle;

    mMessage = "posting score";


    startConnectionTry();
    
    }





void HighScoreLoadingPanel::step() {
    Panel::step();
    
    mStatusLight.step();


    mBlinkTime += 0.2;


    // check if we should hide ourself
    if( mVisible &&
        mDisplayPanel.isFullyVisible() ) {
        
        // display showing
        setVisible( false );
        }
    



    if( mServerURLFetchRequest != NULL ) {
        int stepResult = mServerURLFetchRequest->step();
        
        if( stepResult == 1 ) {
            // done
            
            char *result = mServerURLFetchRequest->getResult();
            
            SimpleVector<char *> *tokens = tokenizeString( result );
            
            delete [] result;
            
            if( tokens->size() > 0 ) {
                
                char *returnedURL = *( tokens->getElement( 0 ) );
                
                if( mServerURL != NULL ) {
                    delete [] mServerURL;
                    }
                
                mServerURL = stringDuplicate( returnedURL );
                
                printf( "Got server URL: %s\n", mServerURL );
                
                // save it
                if( savedServerURL != NULL ) {
                    delete [] savedServerURL;
                    }
                savedServerURL = stringDuplicate( mServerURL );
                }
            else {
                setFailed();
                }
            
            for( int i=0; i<tokens->size(); i++ ) {
                delete [] *( tokens->getElement( i ) );
                }
            delete tokens;

            delete mServerURLFetchRequest;
            mServerURLFetchRequest = NULL;
            }
        else if( stepResult == -1 ) {
            // error
            delete mServerURLFetchRequest;
            mServerURLFetchRequest = NULL;
            
            setFailed();
            }
        }
    else if( mWebRequest != NULL ) {
        int stepResult = mWebRequest->step();
    
        if( stepResult == 1 ) {
            // done
            char goodResult = false;
            
            char *result = mWebRequest->getResult();
            
            SimpleVector<char *> *tokens = tokenizeString( result );
            
            delete [] result;

            
            if( tokens->size() >= 2 ) {
                
                int numAllTime;
                
                int numRead = sscanf( *( tokens->getElement( 0 ) ),
                                      "%d", &numAllTime );
                    
                if( numRead == 1 ) {
                        
                    if( tokens->size() >= 2 + numAllTime ) {
                        
                        mDisplayPanel.clearScores();
                        

                        int i;
                        
                        for( i=0; i<numAllTime; i++ ) {
                            
                            ScoreBundle *b =
                                new ScoreBundle( 
                                    *( tokens->getElement( 1 + i ) ) );
                            
                            mDisplayPanel.addAllTimeScore( b );
                            }
                        
                        
                        int numToday;
                
                        int numRead = 
                            sscanf( *( tokens->getElement( 1 +numAllTime ) ),
                                    "%d", &numToday );
                    
                        if( numRead == 1 ) {
                        
                            if( tokens->size() >= 2 + numAllTime + numToday ) {
                        
                                int i;
                                
                                goodResult = true;
                                
                                for( i=0; i<numToday; i++ ) {
                            
                                    ScoreBundle *b =
                                        new ScoreBundle( 
                                            *( tokens->getElement( 
                                                   2 + numAllTime + i ) ) );
                            
                                    mDisplayPanel.addTodayScore( b );
                                    }
                                }
                            }
                        }
                    }
                }


            if( !goodResult ) {
                setFailed();
                }
            else {
                mDisplayPanel.setVisible( true );
                }
            


            
            for( int i=0; i<tokens->size(); i++ ) {
                delete [] *( tokens->getElement( i ) );
                }
            delete tokens;

            delete mWebRequest;
            mWebRequest = NULL;
            }
        else if( stepResult == -1 ) {
            // error
            delete mWebRequest;
            mWebRequest = NULL;
            
            setFailed();
            }
        }
    else if( mScoreToPost != NULL && mServerURL != NULL ) {
        // need to generate a request
        char *body = autoSprintf( "action=post_score"
                                  "&name=%s"
                                  "&score=%u"
                                  "&seed=%u"
                                  "&move_history=%s",
                                  mScoreToPost->mName,
                                  mScoreToPost->mScore,
                                  mScoreToPost->mSeed,
                                  mScoreToPost->mMoveHistory );
        
        delete mScoreToPost;
        mScoreToPost = NULL;
        
        mWebRequest = new WebRequest( "POST",
                                      mServerURL,
                                      body );
        

        delete [] body;
        }
    else if( mVisible && mFadeProgress == 1 && ! mDisplayPanel.isVisible() &&
             !mFailed && mServerURL != NULL ) {

        // wait until fully faded in to avoid jumping abruptly to display
        // if request is fast

        // generate a load request
        char *url = autoSprintf( "%s?action=fetch_scores",
                                 mServerURL );
        
        mWebRequest = new WebRequest( "GET",
                                      url,
                                      NULL );
        
        delete [] url;
        }
    
                                  
        
    }



void HighScoreLoadingPanel::setVisible( char inIsVisible ) {
    Panel::setVisible( inIsVisible );

    if( inIsVisible ) {
        // try again, afresh
        startConnectionTry();
        }
            
    }



char HighScoreLoadingPanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {

        } 
    
    return false;
    }



Color loadingMessageColor( 76/255.0, 76/255.0, 255/255.0 );

        
void HighScoreLoadingPanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        int textY = 100;
            
        drawStringBig( mMessage, center, 
                       mW / 2,
                       textY,
                       &loadingMessageColor, mFadeProgress );        
        
            
        glDisable( GL_BLEND );

        

        mStatusLight.drawGrid( mFadeProgress );

        float glowVal = sin( mBlinkTime - M_PI / 2 ) * 0.5 + 0.5;

        mStatusLight.drawPieceCenter( mFadeProgress * glowVal );
        mStatusLight.drawPieceHalo( mFadeProgress * glowVal );
        }
    
    }



void HighScoreLoadingPanel::closePressed() {
    
    if( mWebRequest != NULL ) {
        delete mWebRequest;
        }
    mWebRequest = NULL;
    
    }


        
        
