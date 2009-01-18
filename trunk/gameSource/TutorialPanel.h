#include "Panel.h"

#include "NextPieceDisplay.h"


class TutorialPanel : public Panel {
        

    public:
        

        TutorialPanel( int inW, int inH );
        

        ~TutorialPanel();
        
        

        // overrides these:
                
        void setVisible( char inIsVisible );
        
        void step();
        

    protected:
        // override
        void drawBase();

        // receive close event
        void closePressed();
        
        
    private:
        
        NextPieceDisplay mNextPieceDemo;
        
        GridSpace *mGridDemo[4][7];
        GridSpace *mAllDemoSpaces[28];


        int mDemoStage;
        

        int mStepsBetweenStages;
        
        int mStepCount;
        
        
    };


        
        
