#include "Panel.h"


#include "EditNamePanel.h"
#include "TutorialPanel.h"
#include "HighScoreLoadingPanel.h"
#include "HighScorePanel.h"
#include "ScoreBundle.h"



class MenuPanel : public Panel {
        

    public:
        

        MenuPanel( int inW, int inH );
        

        ~MenuPanel();
        
        void postScore( ScoreBundle *inScore );
        
        

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
        TutorialPanel mTutorialPanel;
        

        // pointer to display, which is maintained by HighScoreLoadingPanel
        HighScorePanel *mDisplayPanel;
        
    };


        
        
