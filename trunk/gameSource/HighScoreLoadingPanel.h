#include "Panel.h"

#include "GridSpace.h"

#include "ScoreBundle.h"

#include "WebRequest.h"


class HighScoreLoadingPanel : public Panel {
        

    public:
        

        HighScoreLoadingPanel( int inW, int inH );
        

        ~HighScoreLoadingPanel();
        
        // bundle deleted after posting
        // posting starts when made visible
        void setScoreToPost( ScoreBundle *inBundle );
        
        

        // overrides these:
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
        
        ScoreBundle *mScoreToPost;
        
        WebRequest *mWebRequest;

        
        char *mServerFinderURL;

        char *mServerURL;
        WebRequest *mServerURLFetchRequest;
        
        
    };


        
        
