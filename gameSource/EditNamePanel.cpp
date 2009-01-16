#include "EditNamePanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"


#include <GL/gl.h>



EditNamePanel::EditNamePanel( int inW, int inH )
        : Panel( inW, inH ),
          mName( getName() ),
          mOverrideName( NULL ) {
          
    
    int i=0;
    
    int xStep = 41 + 19;
    int xStart = ( inW - xStep * 4 ) / 2;
    
    
    int yStep = xStep;
    
    int yStart = inH - (19 + 21) - ( yStep * 5 );

    // grid of 6x5, but stop at 28 buttons
    for( int y=0; y<6 && i<28; y++ ) {
        for( int x=0; x<5 && i<28; x++ ) {
            char *s;

            if( i < 25 ) {
                s = new char[2];
                s[0] = 'a' + i;
                s[1] = '\0';
                }
            else if( i == 25 ) {
                s = stringDuplicate( "bksp" );
                }
            else if( i == 26 ) {
                s = stringDuplicate( "z" );
                }
            else {
                s = stringDuplicate( "clr" );
                }
            
                
            if( i<26) {
                
                mKeyButtons[i] = new Button( xStart + x * xStep,
                                             yStart + y * yStep,
                                             s );
                }
            else if( i==26 ){
                // center
                mKeyButtons[i] = new Button( xStart + (x+1) * xStep,
                                             yStart + y * yStep,
                                             s );
                }
            else if( i==27 ){
                // right
                mKeyButtons[i] = new Button( xStart + (x+2) * xStep,
                                             yStart + y * yStep,
                                             s );
                }
            

            addButton( mKeyButtons[i] );
            
            i++;
            }
        }
    
    
    
    }


    
EditNamePanel::~EditNamePanel() {
    for( int i=0; i<28; i++ ) {
        delete mKeyButtons[i];
        }

    if( mOverrideName != NULL ) {
        delete [] mOverrideName;
        mOverrideName = NULL;
        }
    }



void EditNamePanel::setVisible( char inIsVisible ) {
    Panel::setVisible( inIsVisible );
    
    if( inIsVisible ) {
        // switch to displaying actual global name
        if( mOverrideName != NULL ) {
            delete [] mOverrideName;
            mOverrideName = NULL;
            }
        }
    }



char EditNamePanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {


        for( int i=0; i<28; i++ ) {
            if( mKeyButtons[i]->isInside( inX, inY ) ) {
                
                char *buttonString = mKeyButtons[i]->mString;
                
                int oldNameLength = strlen( mName );

                if( strlen( buttonString ) == 1 ) {
                    // a letter
                    
                    
                    
                    if( oldNameLength < 8 ) {
                        
                        mName[ oldNameLength ] = buttonString[0];
                        mName[ oldNameLength + 1 ] = '\0';
                        }
                    }
                else if( i == 25 ) {
                    // backspace
                    mName[ oldNameLength - 1 ] = '\0';
                    }
                else if( i==27 ) {
                    // clear
                    mName[0] = '\0';
                    }
                
                return true;
                }
            }
        } 
    
    return false;
    }



Color nameDisplayColor( 76/255.0, 76/255.0, 255/255.0 );
Color nameCapColor( 1, 1, 1 );

        
void EditNamePanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        int textY = 75;
        
        // draw end caps around 8-char wide text area
        /*
        drawSprite( gridLineTop, mKeyButtons[1]->mX, textY, 32, 32, 
                    &nameCapColor, mFadeProgress );
        */
        drawSprite( gridLineLeft, mKeyButtons[1]->mX, textY, 32, 32, 
                    &nameCapColor, mFadeProgress );
        /*
        drawSprite( gridLineBottom, mKeyButtons[1]->mX, textY, 32, 32, 
                    &nameCapColor, mFadeProgress );
        */
        /*
        drawSprite( gridLineTop, mKeyButtons[3]->mX, textY, 32, 32, 
                    &nameCapColor, mFadeProgress );
        */
        drawSprite( gridLineRight, mKeyButtons[3]->mX, textY, 32, 32, 
                    &nameCapColor, mFadeProgress );
        /*
        drawSprite( gridLineBottom, mKeyButtons[3]->mX, textY, 32, 32, 
                    &nameCapColor, mFadeProgress );
        */

        if( mOverrideName == NULL ) {
            
            drawStringBig( mName, center, 
                           mW / 2,
                           textY,
                           &nameDisplayColor, mFadeProgress );        
            }
        else {
            drawStringBig( mOverrideName, center, 
                           mW / 2,
                           textY,
                           &nameDisplayColor, mFadeProgress );        
            }
        
            
        glDisable( GL_BLEND );
        
        


        for( int i=0; i<28; i++ ) {
            mKeyButtons[i]->draw();
            }

        }
    
    }



void EditNamePanel::closePressed() {

    if( strlen( mName ) == 0 ) {
        // setting it to blank
        // game system will override this instantly when we save it

        // this is jaring (as panel fades out)
        
        // override to blank
        if( mOverrideName != NULL ) {
            delete [] mOverrideName;
                        }
        mOverrideName = stringDuplicate( mName );
        }
    

    saveName();
    }


        
        
