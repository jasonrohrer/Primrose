

#include "GridSpace.h"

#include "spriteBank.h"


GridSpace::GridSpace( int inX, int inY )
        :mX( inX ), mY( inY ) {
    }


void GridSpace::draw() {
    
    drawSprite( gridSpace, mX, mY, 32, 32 );
        
    }

