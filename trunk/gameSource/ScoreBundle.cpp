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

    
        
ScoreBundle::~ScoreBundle() {
    delete [] mMoveHistory;
    }
