

#include "GridSpace.h"

#include "spriteBank.h"


#include <math.h>

#include <GL/gl.h>



GridSpace::GridSpace( int inX, int inY )
        :mX( inX ), mY( inY ), mActive( false ),
         mPieceColor( NULL ), mLastColor( NULL ),
         mColorShiftProgress( 0 ),
         mDrawColor( NULL ) {

    for( int n=0; n<4; n++ ) {
        mNeighbors[n] = NULL;
        }
    
    }


GridSpace::~GridSpace() {
    if( mLastColor != NULL ) {
        delete mLastColor;
        }
    if( mPieceColor != NULL ) {
        delete mPieceColor;
        }
    if( mDrawColor != NULL ) {
        delete mDrawColor;
        }
    }



char GridSpace::isInside( int inX, int inY ) {
    if( fabs( inX - mX ) < 20
        &&
        fabs( inY - mY ) < 20 ) {
        
        return true;
        }
    
    return false;
    }



void GridSpace::setColor( Color *inColor ) {
    
    if( mLastColor != NULL ) {
        delete mLastColor;
        }
    
    mLastColor = mPieceColor;
    
    mPieceColor = inColor;
    
    mColorShiftProgress = 0;
    }




void GridSpace::drawGrid() {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    drawSprite( gridLineTop, mX, mY, 32, 32 );
    
    drawSprite( gridLineLeft, mX, mY, 32, 32 );
    

    if( mNeighbors[bottom] == NULL ) {
        drawSprite( gridLineBottom, mX, mY, 32, 32 );
        }
    
    if( mNeighbors[right] == NULL ) {
        drawSprite( gridLineRight, mX, mY, 32, 32 );
        }
    
    if( mActive ) {
        drawSprite( plus, mX, mY, 16, 16 );
        }
    

    glDisable( GL_BLEND );
    
    }



void GridSpace::drawPieceCenter() {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    if( mDrawColor != NULL ) {
        drawSprite( pieceCenter, mX, mY, 32, 32, mDrawColor, mDrawColor->a );
        }

    glDisable( GL_BLEND );
    }



void GridSpace::drawPieceHalo() {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    if( mDrawColor != NULL ) {
        drawSprite( pieceHalo, mX, mY, 32, 32, mDrawColor, mDrawColor->a );
        }

    glDisable( GL_BLEND );
    }



void GridSpace::step() {

    if( mDrawColor != NULL ) {
        delete mDrawColor;
        mDrawColor = NULL;
        }
    
    if( mLastColor != NULL || mPieceColor != NULL ) {
        
        Color *blendA;
        if( mLastColor == NULL ) {
            // piece color with total transparency
            blendA = mPieceColor->copy();
            blendA->a = 0;
            }
        else {
            blendA = mLastColor->copy();
            }
        
        
        Color *blendB;
        if( mPieceColor == NULL ) {
            // last color with total transparency
            blendB = mLastColor->copy();
            blendB->a = 0;
            }
        else {
            blendB = mPieceColor->copy();
            }
        
                
        
        mDrawColor = Color::linearSum( blendB, blendA, mColorShiftProgress );
        

        delete blendA;
        delete blendB;
        
        mColorShiftProgress += 0.2;
        
        if( mColorShiftProgress > 1 ) {
            mColorShiftProgress = 1;
            }
        
        }
    
            
    }



char GridSpace::isAnimationDone() { 

    if( mDrawColor == NULL ) {
        return true;
        }
    
    else if( mColorShiftProgress == 1 ) {
        return true;
        }

    return false;
    }
