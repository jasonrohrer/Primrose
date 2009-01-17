#ifndef HIGH_SCORE_PANEL_INCLUDED
#define HIGH_SCORE_PANEL_INCLUDED


#include "Panel.h"

#include "ScoreBundle.h"


class HighScorePanel : public Panel {
        

    public:
        
        
        HighScorePanel( int inW, int inH  );
        

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
        

        Button *mAllTimeButtons[8];
        Button *mTodayButtons[8];
    };


#endif

        
        
