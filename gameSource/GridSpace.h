#ifndef GRID_SPACE_INCLUDED
#define GRID_SPACE_INCLUDED


#include "minorGems/graphics/Color.h"


class GridSpace {
        
    public:
        
        // constructs w/ center location on screen
        GridSpace( int inX, int inY );
        
        ~GridSpace();
        

        // steps animations
        void step();
        
        
        // true if this space is currently not animating
        char isAnimationDone();
        

        // checks if this space is part of a like-colored group
        // that is surrounded by a single color
        // returns NULL if not surrounded
        // returns color of surrounders if surrounded
        // returns color of this piece if surrounded by edges
        // Recursive
        Color *checkSurrounded();
        
        
        // start clearing animation
        void flipToClear();
        
        

        // draws onto screen
        void drawGrid( float inAlpha=1.0f );
        void drawPieceCenter();
        void drawPieceHalo();
        

        char isInside( int inX, int inY );
        
        
        char isEmpty() {
            return ( mPieceColor == NULL );
            }
        

        // color destroyed by this class
        void setColor( Color *inColor );



        // saves state to return to upon rewind
        void saveState();
        
        void rewindState();
        
        
        
            
        // returns a copy of this space's current color, or NULL
        // if empty
        Color *getColor() {
            if( isEmpty() ) {
                return NULL;
                }
            else {
                return mPieceColor->copy();
                }
            }
        


        char colorMatches( Color *inColor );
        
        
        
        

        int mX, mY;
        
        char mActive;
        
        char mVisited;
        char mChecked;
        char mMarkedForClearing;
        
        
        int mScore;
        
        

        enum NeighborDirections { top=0, right, bottom, left 
            };
        
        
        GridSpace *mNeighbors[4];
        

        // current blend of colors displayed
        Color *mDrawColor;


    private:
        
        Color *mPieceColor;
        
        Color *mLastColor;
        
        float mColorShiftProgress;
        

        char mBrightHalo;
        
        float mBrightHaloProgress;
        
        // active mark fades in too
        float mActiveProgress;
        

        float mScoreFade;
        char mScoreSent;

        Color *mSavedColor;
        char mSavedActive;
        
        
        
    };


#endif        
        
