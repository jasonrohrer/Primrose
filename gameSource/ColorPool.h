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
        
        
        int getLevel() {
            return mLevel;
            }
        


        // steps animations
        void step();
        

        // draws onto screen
        void draw();
        

        int mX, mY;



    private:
        
        GridSpace *mSpaces[7];
        
        int mNumActiveColors;
        int mColorsToSkip;
        
        int mStepsBetweenUpdates;

        int mStepsUntilUpdate;
        
        int mLastStepCount;
        float mStepCountTransitionProgress;
        

        int mLevel;
        
        
        
                        
    };


#endif
        
        
