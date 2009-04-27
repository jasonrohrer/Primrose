#include "ScoreBundle.h"

#include "minorGems/util/stringUtils.h"

#include <string.h>



ScoreBundle::ScoreBundle( char *inName, 
                          unsigned int inScore, unsigned int inSeed,
                          char *inSeedHistory,
                          char *inMoveHistory )
    : mScore( inScore ), mSeed( inSeed ), 
      mSeedHistory( stringDuplicate( inSeedHistory ) ),
      mMoveHistory( stringDuplicate( inMoveHistory ) ),
      mNumMoves( strlen( inMoveHistory ) ) {
    

    int nameLength = strlen( inName );

    if( nameLength > 8 ) {
        nameLength = 8;
        }
    
    memcpy( mName, inName, nameLength );
    mName[ nameLength ] = '\0';

    fillSeedHistoryVectors();
    }



ScoreBundle::ScoreBundle( char *inEncoded )
        : mScore( 0 ), mSeed( 0 ), mSeedHistory( NULL ), mMoveHistory( NULL ),
          mNumMoves( 0 ) {
    
    mName[0] = '\0';
    

    int numParts;
    
    char **parts = split( inEncoded, "#", &numParts );
    

    if( numParts == 5 ) {
        
        char *name = parts[0];
        

        int nameLength = strlen( name );
        
        if( nameLength > 8 ) {
            nameLength = 8;
            }
        
        memcpy( mName, name, nameLength );
        mName[ nameLength ] = '\0';
        
        
        int numRead = sscanf( parts[1],
                              "%u", &mScore );
        if( numRead != 1 ) {
            printf( "Failed to decode score bundle: %s\n", inEncoded );
            }
        
        numRead = sscanf( parts[2],
                          "%u", &mSeed );
        
        if( numRead != 1 ) {
            printf( "Failed to decode score bundle: %s\n", inEncoded );
            }


        mSeedHistory = stringDuplicate( parts[3] );
        

        mMoveHistory = stringDuplicate( parts[4] );
        mNumMoves = strlen( mMoveHistory );
        }
    else {
        printf( "Failed to decode score bundle: %s\n", inEncoded );
        }
    
    

    for( int i=0; i<numParts; i++ ) {
        delete [] parts[i];
        }
    delete [] parts;


    fillSeedHistoryVectors();
    }

    
        
ScoreBundle::~ScoreBundle() {
    if( mSeedHistory != NULL ) {
        delete [] mSeedHistory;
        }
    if( mMoveHistory != NULL ) {
        delete [] mMoveHistory;
        }
    }



ScoreBundle *ScoreBundle::copy() {
    return new ScoreBundle( mName, mScore, mSeed, mSeedHistory, mMoveHistory );
    }



void ScoreBundle::fillSeedHistoryVectors() {
    
    if( strlen( mSeedHistory ) > 0 ) {
        

        int numPairs;
        
        char **pairs = split( mSeedHistory, "__", &numPairs );
        
        
        char failed = false;
        

        for( int i=0; i<numPairs; i++ ) {

            int numElements;
            char **elements = split( pairs[i], "_", &numElements );
            

            if( numElements == 2 ) {
                
                unsigned int moveNumber;
                unsigned int seed;
                int numRead;
                

                numRead = sscanf( elements[0],
                                  "%u", &moveNumber );
                if( numRead != 1 ) {
                    failed = true;
                    }

                numRead = sscanf( elements[1],
                                  "%u", &seed );
                if( numRead != 1 ) {
                    failed = true;
                    }
                
                mSeedHistoryMoveNumbers.push_back( moveNumber );
                mSeedHistorySeeds.push_back( seed );
                }
            else {
                failed = true;
                }
            
            

            for( int j=0; j<numElements; j++ ) {
                delete [] elements[j];
                }
            delete [] elements;
            

            delete [] pairs[i];
            }
        delete [] pairs;

        
        if( failed ) {
            printf( "Failed to decode seed history: %s\n", 
                    mSeedHistory );
            }
        
        
        }
    
        
    }




char ScoreBundle::isSeedResetAfterMove( unsigned int inMoveNumber ) {
    int index = mSeedHistoryMoveNumbers.getElementIndex( inMoveNumber );
    
    return ( index != -1 );
    }



unsigned int ScoreBundle::getNewSeedAfterMove( int inMoveNumber ) {
    
    int index = mSeedHistoryMoveNumbers.getElementIndex( inMoveNumber );

    if( index != -1 ) {
        return *( mSeedHistorySeeds.getElement( index ) );
        }
    else {
        return 0;
        }
    }

        
