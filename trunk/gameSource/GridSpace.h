

#include "minorGems/graphics/Color.h"


class GridSpace {
        
    public:
        
        // constructs w/ center location on screen
        GridSpace( int inX, int inY );
        
        ~GridSpace();
        

        // steps animations
        void step();
        

        // draws onto screen
        void drawGrid();
        void drawPieceCenter();
        void drawPieceHalo();
        

        char isInside( int inX, int inY );
        
        
        char isEmpty() {
            return ( mPieceColor == NULL );
            }
        

        // color destroyed by this class
        void placePiece( Color *inColor );
        
            
        
        

        int mX, mY;


        enum NeighborDirections { top=0, right, bottom, left 
            };
        
        
        GridSpace *mNeighbors[4];


    private:
        
        Color *mPieceColor;
        
        Color *mLastColor;
        
        float mColorShiftProgress;
        
        // blend
        Color *mDrawColor;
        
    };

        
        
