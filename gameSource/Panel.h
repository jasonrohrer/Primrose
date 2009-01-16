#ifndef PANEL_INCLUDED
#define PANEL_INCLUDED


#include "Button.h"


#include "minorGems/util/SimpleVector.h"



// panel with a close button at the top
class Panel {
        

    public:
        

        Panel( int inW, int inH);
        

        virtual ~Panel();
        

                
        virtual void setVisible( char inIsVisible );

        virtual char isVisible() {
            return mVisible;
            }
        
        // forces to become completely visible instantly
        virtual void forceVisible();
        


        // tells panel that pointer released inside it
        // returns true if release consumed (triggered a button somewhere)
        virtual char pointerUp( int inX, int inY );
        


        // steps animations
        virtual void step();
        

        // draws onto screen
        // don't override this
        void draw();


    protected:
        
        void addSubPanel( Panel *inPanel );
        void addButton( Button *inButton );
        
        char isSubPanelVisible();
        
        // subclasses override this
        virtual void drawBase();
        
        float mFadeProgress;
        
        

    private:
        void drawSubPanels();

        Button mCloseButton;
        
        SimpleVector<Button *> mButtons;

        SimpleVector<Panel *> mSubPanels;
        
        

        int mW, mH;
        
        
        char mVisible;
        
        
    };


#endif

        
        
