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
        void drawGrid();
        void drawPieceCenter();
        void drawPieceHalo();
        

        char isInside( int inX, int inY );
        
        
        char isEmpty() {
            return ( mPieceColor == NULL );
            }
        

        // color destroyed by this class
        void setColor( Color *inColor );
        
            
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
        
        

        enum NeighborDirections { top=0, right, bottom, left 
            };
        
        
        GridSpace *mNeighbors[4];


    private:
        
        Color *mPieceColor;
        
        Color *mLastColor;
        
        float mColorShiftProgress;
        
        // blend
        Color *mDrawColor;

        char mBrightHalo;
        
        float mBrightHaloProgress;
        
        
    };


#endif        
        
