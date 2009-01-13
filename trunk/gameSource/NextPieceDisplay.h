#include "GridSpace.h"
#include "ColorPool.h"

class NextPieceDisplay {
        
    public:
        
        // constructs w/ center location on screen
        NextPieceDisplay( int inX, int inY, ColorPool *inPool );
        
        ~NextPieceDisplay();
        

        // removes the next piece and updates the display
        // returned color destroyed by caller
        
        // this sets the next piece spot to NULL        
        Color *getNextPiece();
        
        // updates the piece display, sliding the next piece forward
        // or generating 2 new colors
        // must be called between calls to getNextPiece
        void update();
        

        // true if getNextPiece will return the second piece from a pair
        char isSecondPiece();
        


        // steps animations
        void step();
        

        // draws onto screen
        void draw();
        

        int mX, mY;



    private:
        ColorPool *mPool;
        
        GridSpace *mSpaces[2];
        

        float mBlinkTime;
        
                        
    };

        
        
