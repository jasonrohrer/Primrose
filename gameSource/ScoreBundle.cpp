#include "ScoreBundle.h"

#include "minorGems/util/stringUtils.h"

#include <string.h>



ScoreBundle::ScoreBundle( char *inName, 
                          unsigned int inScore, unsigned int inSeed,
                          char *inMoveHistory )
    : mScore( inScore ), mSeed( inSeed ), 
      mMoveHistory( stringDuplicate( inMoveHistory ) ) {
    

    int nameLength = strlen( inName );

    if( nameLength > 8 ) {
        nameLength = 8;
        }
    
    memcpy( mName, inName, nameLength );
    mName[ nameLength ] = '\0';
    }



ScoreBundle::ScoreBundle( char *inEncoded ) {
    
    int numParts;
    
    char **parts = split( inEncoded, "#", &numParts );
    

    if( numParts == 4 ) {
        
        char *name = parts[0];
        

        int nameLength = strlen( name );
        
        if( nameLength > 8 ) {
            nameLength = 8;
            }
        
        memcpy( mName, name, nameLength );
        mName[ nameLength ] = '\0';
        
        
        int numRead = sscanf( parts[1],
                              "%u", &mScore );
        
        numRead = sscanf( parts[2],
                          "%u", &mSeed );
        
        mMoveHistory = stringDuplicate( parts[3] );
        }
    

    for( int i=0; i<numParts; i++ ) {
        delete [] parts[i];
        }
    delete [] parts;
    }

    
        
ScoreBundle::~ScoreBundle() {
    delete [] mMoveHistory;
    }
