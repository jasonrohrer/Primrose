#include "Panel.h"


#include "EditNamePanel.h"
#include "HighScoreLoadingPanel.h"



class MenuPanel : public Panel {
        

    public:
        

        MenuPanel( int inW, int inH );
        

        ~MenuPanel();
        

        // over rides these:
                
        char pointerUp( int inX, int inY );
        

    protected:
        // override
        void drawBase();

        // receive close event
        void closePressed();
        
        
    private:
        Button mNewButton;
        Button mTutorialButton;
        Button mHighScoreButton;
        Button mEditNameButton;
        
        
        HighScoreLoadingPanel mHighScoreLoadingPanel;
        EditNamePanel mEditNamePanel;
        
        
    };


        
        
