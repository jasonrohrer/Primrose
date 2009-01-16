#include "Panel.h"


#include <GL/gl.h>



Panel::Panel( int inW, int inH )
        : mW( inW ), mH( inH ),
          mFadeProgress( 0 ),
          mCloseButton( 19 + 21, 19 + 21, "back" ),
          mVisible( false ) {

    mCloseButton.setVisible( false );
    
    addButton( &mCloseButton );
    }


    
Panel::~Panel() {
    
    }

        
        
void Panel::setVisible( char inIsVisible ) {
    mVisible = inIsVisible;
    
    int i;
    
    for( i=0; i<mButtons.size(); i++ ) {
        Button *b = *( mButtons.getElement( i ) );
        b->setVisible( mVisible );
        }
    }



void Panel::forceVisible() {
    mVisible = true;
    mFadeProgress = 1;
    
    int i;
    
    for( i=0; i<mButtons.size(); i++ ) {
        Button *b = *( mButtons.getElement( i ) );
        b->forceVisible();
        }
    }



char Panel::pointerUp( int inX, int inY ) {
    
    int i;
    
    char somePanelVisible = false;
    
    for( i=0; i<mSubPanels.size(); i++ ) {
        Panel *p = *( mSubPanels.getElement( i ) );
        if( p->isVisible() ) {
            somePanelVisible = true;
            char consumed = p->pointerUp( inX, inY );

            // stop as soon as consumed
            if( consumed ) {
                return true;
                }
            }
        }

    
    if( !somePanelVisible ) {
        
        if( mCloseButton.isInside( inX, inY ) ) {
            // inform subclass of close
            closePressed();
            
            setVisible( false );
            
            return true;
            }
        }
    
    return false;
    }



void Panel::step() {
   
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

    int i;
    
    for( i=0; i<mButtons.size(); i++ ) {
        Button *b = *( mButtons.getElement( i ) );
        b->step();
        }

    for( i=0; i<mSubPanels.size(); i++ ) {
        Panel *p = *( mSubPanels.getElement( i ) );
        p->step();
        }
    
    }



void Panel::addSubPanel( Panel *inPanel ) {
    mSubPanels.push_back( inPanel );
    }



void Panel::addButton( Button *inButton ) {
    mButtons.push_back( inButton );
    }



char Panel::isSubPanelVisible() {
    int i;
    for( i=0; i<mSubPanels.size(); i++ ) {
        Panel *p = *( mSubPanels.getElement( i ) );
        if( p->isVisible() ) {
            return true;
            }
        }
    
    return false;
    }


void Panel::draw() {
    drawBase();
    drawSubPanels();
    }


        
void Panel::drawBase() {
    
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

        
        int i;
        
        for( i=0; i<mButtons.size(); i++ ) {
            Button *b = *( mButtons.getElement( i ) );
            b->draw();
            }
        }
    }

    
void Panel::drawSubPanels() {

    int i;
    
    for( i=0; i<mSubPanels.size(); i++ ) {
        Panel *p = *( mSubPanels.getElement( i ) );
        p->draw();
        }

    }


        
        
