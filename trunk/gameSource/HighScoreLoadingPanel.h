#include "Panel.h"

#include "GridSpace.h"


class HighScoreLoadingPanel : public Panel {
        

    public:
        

        HighScoreLoadingPanel( int inW, int inH );
        

        ~HighScoreLoadingPanel();
        

        // over rides these:
        void step();
        
        void setVisible( char inIsVisible );

        char pointerUp( int inX, int inY );
        

    protected:
        // override
        void drawBase();
        void closePressed();
        

        
    private:
        
        char *mMessage;
        
        GridSpace mStatusLight;
        
        float mBlinkTime;

        
    };


        
        
