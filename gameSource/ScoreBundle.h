

class ScoreBundle {
        
    public:
        
        ScoreBundle( char *inName, 
                     unsigned int inScore, unsigned int inSeed,
                     char *inMoveHistory );
        
        ~ScoreBundle();
        
        
        // name up to 8 chars long
        char mName[9];

        unsigned int mScore;
        

        unsigned int mSeed;
        

        char *mMoveHistory;
    };


        
