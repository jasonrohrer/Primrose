#include "HighScoreLoadingPanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"

#include <math.h>
#include <GL/gl.h>


Color scoreLoadingGreen( 0, 1, 0 );

Color scoreLoadingRed( 1, 0, 0 );



HighScoreLoadingPanel::HighScoreLoadingPanel( int inW, int inH )
        : Panel( inW, inH ),
          mMessage( "loading scores" ),
          mStatusLight( inW / 2, inH / 2 ),
          mBlinkTime( 0 ) {
          
    mStatusLight.setColor( scoreLoadingGreen.copy() );
    }


    
HighScoreLoadingPanel::~HighScoreLoadingPanel() {
    }





void HighScoreLoadingPanel::step() {
    Panel::step();
    
    mStatusLight.step();


    mBlinkTime += 0.2;
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

        float glowVal = sin( mBlinkTime - M_PI / 2 ) * 0.5 + 0.5;

        mStatusLight.drawPieceCenter( mFadeProgress * glowVal );
        mStatusLight.drawPieceHalo( mFadeProgress * glowVal );
        }
    
    }



void HighScoreLoadingPanel::closePressed() {
    }


        
        
