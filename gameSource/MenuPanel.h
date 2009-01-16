#include "Panel.h"



class MenuPanel : public Panel {
        

    public:
        

        MenuPanel( int inW, int inH );
        

        ~MenuPanel();
        

        // over rides these:
                
        void setVisible( char inIsVisible );        


        char pointerUp( int inX, int inY );
        

        void step();


    protected:
        // override
        void drawBase();


        
    private:
        Button mNewButton;
        Button mTutorialButton;
        Button mHighScoreButton;
        Button mEditNameButton;
        
        Panel mTutorialPanel;
        
        
    };


        
        
