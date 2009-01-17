#include "Panel.h"

#include "ScoreBundle.h"


class HighScorePanel : public Panel {
        

    public:
        
        
        // loading panel will be set to non-visible as soon as this
        // panel is fully visible (so BACK in this panel returns to menu)
        HighScorePanel( int inW, int inH, Panel *inLoadingPanel );
        

        ~HighScorePanel();
        

        void clearScores();
        
        
        // destroyed by this class
        void addAllTimeScore( ScoreBundle *inScore );
        
        void addTodayScore( ScoreBundle *inScore );
        
        

        // overrides these:
        void setVisible( char inIsVisible );
        
        char pointerUp( int inX, int inY );
        

    protected:
        // override
        void drawBase();
        void closePressed();
        

        
    private:
        
        SimpleVector<ScoreBundle*> mAllTimeScores;
        
        SimpleVector<ScoreBundle*> mTodayScores;
        
        Panel *mLoadingPanel;
        

        Button *mAllTimeButtons[8];
        Button *mTodayButtons[8];
    };


        
        
