

#include "GridSpace.h"

#include "spriteBank.h"


#include <math.h>

#include <GL/gl.h>



GridSpace::GridSpace( int inX, int inY )
        :mX( inX ), mY( inY ), mPieceColor( NULL ) {

    for( int n=0; n<4; n++ ) {
        mNeighbors[n] = NULL;
        }
    
    }



char GridSpace::isInside( int inX, int inY ) {
    if( fabs( inX - mX ) < 19
        &&
        fabs( inY - mY ) < 19 ) {
        
        return true;
        }
    
    return false;
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
    
    

    glDisable( GL_BLEND );
    
    }



void GridSpace::drawPiece() {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    if( mPieceColor != NULL ) {
        drawSprite( piece, mX, mY, 32, 32, mPieceColor );
        }

    glDisable( GL_BLEND );
    }

