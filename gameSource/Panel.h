#include "Button.h"



// panel with a close button at the top
class Panel {
        

    public:
        

        Panel( int inW, int inH);
        

        ~Panel();
        

                
        void setVisible( char inIsVisible );

        char isVisible() {
            return mVisible;
            }
        

        // tells panel that pointer released inside it
        void pointerUp( int inX, int inY );
        


        // steps animations
        void step();
        

        // draws onto screen
        void draw();


    private:

        Button mCloseButton;
        
        int mW, mH;
        
        
        char mVisible;
        
        float mFadeProgress;
        
    };


        
        
