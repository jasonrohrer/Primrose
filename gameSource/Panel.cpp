#include "Panel.h"
#include "numeral.h"

#include "minorGems/util/stringUtils.h"


#include <GL/gl.h>



Panel::Panel( int inW, int inH )
        : mCloseButton( inW / 2, 19 + 21, "back" ),
          mW( inW ), mH( inH ),
          mVisible( false ),
          mFadeProgress( 0 ) {

    mCloseButton.setVisible( false );
    
    }


    
Panel::~Panel() {
    
    }

        
        
void Panel::setVisible( char inIsVisible ) {
    mVisible = inIsVisible;
    
    mCloseButton.setVisible( mVisible );
    }



void Panel::pointerUp( int inX, int inY ) {
    
    if( mCloseButton.isInside( inX, inY ) ) {
        setVisible( false );
        }
    
    }



void Panel::step() {
    mCloseButton.step();
    
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

    

        
void Panel::draw() {
    
    if( mFadeProgress > 0 ) {

        
        

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        

        glColor4f( 0, 0, 0, mFadeProgress );

        const GLfloat squareVertices[] = {
            0, 0,
            mW, 0,
            0,  mH,
            mW, mH,
            };


        glVertexPointer( 2, GL_FLOAT, 0, squareVertices );

        glEnableClientState( GL_VERTEX_ARRAY );
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
        glDisable( GL_BLEND );


        mCloseButton.draw();
        }
    }


        
        
