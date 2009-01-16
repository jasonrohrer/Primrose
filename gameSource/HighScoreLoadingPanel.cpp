#include "HighScoreLoadingPanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"


#include <GL/gl.h>



HighScoreLoadingPanel::HighScoreLoadingPanel( int inW, int inH )
        : Panel( inW, inH ),
          mMessage( "loading scores" ),
          mStatusLight( inW / 2, inH / 2 ) {
          
        
    }


    
HighScoreLoadingPanel::~HighScoreLoadingPanel() {
    }



Color scoreLoadingGreen( 0, 1, 0 );

Color scoreLoadingRed( 1, 0, 0 );


void HighScoreLoadingPanel::step() {
    Panel::step();
    
    mStatusLight.step();
    
    Color *lightColor = mStatusLight.getColor();
    
    if( lightColor != NULL ) {
        // light coming on

        delete lightColor;
        
        if( mStatusLight.mDrawColor != NULL &&
            mStatusLight.mDrawColor->a == 1 ) {
            // done brightening
            
            mStatusLight.setColor( NULL );
            }
        }
    else {
        // light going off

        if( mStatusLight.mDrawColor == NULL ||
            mStatusLight.mDrawColor->a == 0 ) {

            // done dimming
            mStatusLight.setColor( scoreLoadingGreen.copy() );            
            }
        }
    }



void HighScoreLoadingPanel::setVisible( char inIsVisible ) {
    Panel::setVisible( inIsVisible );
    
    }



char HighScoreLoadingPanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {

        } 
    
    return false;
    }



Color loadingMessageColor( 76/255.0, 76/255.0, 255/255.0 );

        
void HighScoreLoadingPanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        int textY = 100;
            
        drawStringBig( mMessage, center, 
                       mW / 2,
                       textY,
                       &loadingMessageColor, mFadeProgress );        
        
            
        glDisable( GL_BLEND );
        

        mStatusLight.drawGrid( mFadeProgress );

        // FIXME:  need to fade this out too
        mStatusLight.drawPieceCenter();
        }
    
    }



void HighScoreLoadingPanel::closePressed() {
    }


        
        
