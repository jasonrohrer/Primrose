

#include "minorGems/graphics/Color.h"


class GridSpace {
        
    public:
        
        // constructs w/ center location on screen
        GridSpace( int inX, int inY );
        

        // draws onto screen
        void drawGrid();
        void drawPiece();
        

        char isInside( int inX, int inY );
        
        

        int mX, mY;


        enum NeighborDirections { top=0, right, bottom, left 
            };
        
        
        GridSpace *mNeighbors[4];

        Color *mPieceColor;
        
        
    };

        
        
