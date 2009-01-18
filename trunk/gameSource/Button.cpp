#include "Button.h"
#include "numeral.h"

#include "minorGems/util/stringUtils.h"


#include <GL/gl.h>
#include <math.h>



Button::Button( int inX, int inY, char *inString )
        : mX( inX ), mY( inY ),
          mString( stringDuplicate( inString ) ),
          mSpace( inX, inY ),
          mVisible( false ),
          mFadeProgress( 0 ) {

    }


    
Button::~Button() {
    delete [] mString;
    }



void Button::setString( char *inString ) {
    delete [] mString;
    mString = stringDuplicate( inString );
    }



char Button::isInside( int inX, int inY ) {
    // slightly bigger than underlying grid
    // to make it easier to press

    if( fabs( inX - mX ) < 23
        &&
        fabs( inY - mY ) < 23 ) {
        
        return true;
        }
    
    return false;
    }


        
void Button::setVisible( char inIsVisible ) {
    mVisible = inIsVisible;
    }



void Button::forceVisible() {
    setVisible( true );
    
    mFadeProgress = 1;
    }

void Button::forceInvisible() {
    setVisible( false );

    mFadeProgress = 0;
    }



void Button::step() {
    if( mVisible && mFadeProgress < 1 ) {
        mFadeProgress += 0.1;
        
        if( mFadeProgress > 1 ) {
            mFadeProgress = 1;
            }
        }
    else if( !mVisible && mFadeProgress > 0 ) {
        mFadeProgress -= 0.1;
        
        if( mFadeProgress < 0 ) {
            mFadeProgress = 0;
            }
        }
    }

    
Color buttonStringColor( 76/255.0, 76/255.0, 255/255.0 );

        
void Button::draw() {
    
    if( mFadeProgress > 0 ) {
        

        mSpace.drawGrid( mFadeProgress );


        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        if( strlen( mString ) < 3 ) {
            drawStringBig( mString, center, mSpace.mX, mSpace.mY,  
                           &buttonStringColor, mFadeProgress );
            }
        else {
            drawString( mString, center, mSpace.mX, mSpace.mY,  
                        &buttonStringColor, mFadeProgress );
            }

        glDisable( GL_BLEND );
        }
    
    }


        
        
