#include "MenuPanel.h"

#include "numeral.h"

#include "gameControl.h"


#include <GL/gl.h>



MenuPanel::MenuPanel( int inW, int inH )
        : Panel( inW, inH ),
          mNewButton( inW - 21 - 19, 100, "+" ),
          mTutorialButton( inW - 21 - 19, 200, "+" ),
          mHighScoreButton( inW - 21 - 19, 300, "+" ),
          mEditNameButton( inW - 21 - 19, 400, "+" ),
          mEditNamePanel( inW, inH ) {
          

    mNewButton.setVisible( false );
    mTutorialButton.setVisible( false );
    mHighScoreButton.setVisible( false );
    mEditNameButton.setVisible( false );
    
    addButton( &mNewButton );
    addButton( &mTutorialButton );
    addButton( &mHighScoreButton );
    addButton( &mEditNameButton );

    mEditNamePanel.setVisible( false );
    
    addSubPanel( &mEditNamePanel );
    }


    
MenuPanel::~MenuPanel() {

    }



char MenuPanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {
        
        if( mNewButton.isInside( inX, inY ) ) {
            restartGame();
            
            setVisible( false );
            return true;
            }
        if( mEditNameButton.isInside( inX, inY ) ) {
            
            mEditNamePanel.setVisible( true );
            
            return true;
            }
        } 
    
    return false;
    }



Color menuItemColor( 76/255.0, 76/255.0, 255/255.0 );

        
void MenuPanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        drawStringBig( "new game", right, 
                       mNewButton.mX - 21 - 19,
                       mNewButton.mY,
                       &menuItemColor, mFadeProgress );

        drawStringBig( "instructions", right, 
                       mTutorialButton.mX - 21 - 19,
                       mTutorialButton.mY,
                       &menuItemColor, mFadeProgress );

        drawStringBig( "high scores", right, 
                       mHighScoreButton.mX - 21 - 19,
                       mHighScoreButton.mY,
                       &menuItemColor, mFadeProgress );

        drawStringBig( "edit name", right, 
                       mEditNameButton.mX - 21 - 19,
                       mEditNameButton.mY,
                       &menuItemColor, mFadeProgress );
        

        glDisable( GL_BLEND );
        
        }
    
    }



void MenuPanel::closePressed() {
    
    }


        
        
