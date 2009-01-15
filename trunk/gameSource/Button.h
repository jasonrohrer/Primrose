#include "GridSpace.h"

class Button {
        
    public:
        
        // constructs w/ center location on screen and a string label
        // if label is 1 or 2 characters, drawn with big font
        // if 3 or 4 chars, drawn with little font
        Button( int inX, int inY, char *inString );
        
        ~Button();
        

        char isInside( int inX, int inY ) {
            return mSpace.isInside( inX, inY );
            }
        
        void setVisible( char inIsVisible );

        char isVisible() {
            return mVisible;
            }
        

        // steps animations
        void step();
        

        // draws onto screen
        void draw();
        

        int mX, mY;



    private:
        char *mString;
        
        GridSpace mSpace;
        
        
        char mVisible;
        
        float mFadeProgress;
        
                        
    };

        
        