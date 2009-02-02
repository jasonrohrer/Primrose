#ifndef COLOR_POOL_INCLUDED
#define COLOR_POOL_INCLUDED


#include "GridSpace.h"


class ColorPool {
        
    public:
        
        // constructs w/ center location on screen
        ColorPool( int inX, int inY );
        
        ~ColorPool();
        

        // gets a color from the pool
        Color *pickColor();
        

        void registerMove();
        
        // undoes move registration
        // this is a hack that doesn't work across color addition
        // however, this is okay, because transitions always happen after
        // an even number of moves, and we only allow undo of the odd moves
        void deRegisterMove();
        
        
        int getLevel() {
            return mLevel;
            }
        
        

        // steps animations
        void step();
        

        // draws onto screen
        void draw( float inAlpha = 1.0f );
        

        int mX, mY;



    private:
        
        GridSpace *mSpaces[7];
        
        int mNumActiveColors;
        int mColorsToSkip;
        
        char mEndPhase;
        

        int mStepsBetweenUpdates;

        int mStepsUntilUpdate;
        
        int mLastStepCount;
        float mStepCountTransitionProgress;
        

        int mLevel;
        
        
        
                        
    };


#endif
        
        
