#ifndef SCORE_BUNDLE_INCLUDED
#define SCORE_BUNDLE_INCLUDED


#include "minorGems/util/SimpleVector.h"




class ScoreBundle {
        
    public:
        
        ScoreBundle( char *inName, 
                     unsigned int inScore, unsigned int inSeed,
                     char *inSeedHistory, char *inMoveHistory );
        
        // encoded as  name#score#seed#seed_history#move_history
        // (#-delimited)
        ScoreBundle( char *inEncoded );
        
        
        ~ScoreBundle();
        

        ScoreBundle *copy();
        
        
        // name up to 8 chars long
        char mName[9];

        unsigned int mScore;
        

        unsigned int mSeed;
        
        
        char *mSeedHistory;
        
        char *mMoveHistory;

        int mNumMoves;

        
        char isSeedResetAfterMove( unsigned int inMoveNumber );
        
        unsigned int getNewSeedAfterMove( int inMoveNumber );
        

    private:
        
        void fillSeedHistoryVectors();
        

        SimpleVector<unsigned int> mSeedHistoryMoveNumbers;
        SimpleVector<unsigned int> mSeedHistorySeeds;


    };



#endif
        
