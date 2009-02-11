#include "Panel.h"



class EditNamePanel : public Panel {
        

    public:
        

        EditNamePanel( int inW, int inH );
        

        ~EditNamePanel();
        

        // over rides these:
        void setVisible( char inIsVisible );
        char pointerUp( int inX, int inY );
        

    protected:
        // override
        void drawBase();
        void closePressed();
        

        
    private:
        
        // pointer to global player name
        char *mName;

        // override instead of drawing global name if non-NULL
        char *mOverrideName;
        
        
        Button *mKeyButtons[28];
        
        
    };


        
        
