

#include "GridSpace.h"

#include "spriteBank.h"

#include <GL/gl.h>


GridSpace::GridSpace( int inX, int inY )
        :mX( inX ), mY( inY ) {

    for( int n=0; n<4; n++ ) {
        mNeighbors[n] = NULL;
        }
    
    }


void GridSpace::draw() {
    

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

