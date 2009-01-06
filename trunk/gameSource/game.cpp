#include "game.h"
#include "spriteBank.h"

#include "GridSpace.h"


#include <stdio.h>

#include <GL/gl.h>


float pointerX, pointerY;


void initFrameDrawer( int inWidth, int inHeight ) {
    pointerX = -100;
    pointerY = -100;

    initSpriteBank();
    
    }

void freeFrameDrawer() {
    freeSpriteBank();
    }



void drawFrame() {
    glColor4d( 1.0, 1.0, 1.0, 1.0 );
    glBegin( GL_LINES ); {
        
        glVertex2d( 0, 0 );
        glVertex2d( pointerX, pointerY );
        }
    glEnd();
    

    GridSpace g( pointerX, pointerY );
    
    g.draw();
    

    }



void pointerDown( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    }


void pointerMove( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    printf( "Pointer %f,%f\n", pointerX, pointerY );
    
    }

void pointerUp( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    }
