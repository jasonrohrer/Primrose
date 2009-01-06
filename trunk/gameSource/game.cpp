#include "game.h"

#include <stdio.h>

#include <GL/gl.h>


int pointerX, pointerY;


void initFrameDrawer( int inWidth, int inHeight ) {
    pointerX = -100;
    pointerY = -100;
    }

void freeFrameDrawer() {}



void drawFrame() {
    glColor4d( 1.0, 1.0, 1.0, 1.0 );
    glBegin( GL_LINES ); {
        
        glVertex2d( 0, 0 );
        glVertex2d( pointerX, pointerY );
        }
    glEnd();
    


    }



void pointerDown( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    }


void pointerMove( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    printf( "Pointer %d,%d\n", pointerX, pointerY );
    
    }

void pointerUp( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    }
