#include "game.h"
#include "spriteBank.h"

#include "GridSpace.h"


#include <stdio.h>

#include <GL/gl.h>


float pointerX, pointerY;


#define gridW 7
#define gridH 7
#define numGridSpaces 49

GridSpace *spaces[gridW][gridH];
GridSpace *allSpaces[ numGridSpaces ];


void initFrameDrawer( int inWidth, int inHeight ) {
    pointerX = -100;
    pointerY = -100;

    initSpriteBank();

    int i=0;
    int x;
    int y;
    
    for( x=0; x<gridW; x++ ) {
        for( y=0; y<gridH; y++ ) {
            
            allSpaces[i] = 
            spaces[x][y] =
                new GridSpace( x * 40 + 13 + 21, y * 40 + 13 + 21 );
            
            i++;
            }
        }
    
    // set neighbor relationships
    for( x=0; x<gridW; x++ ) {
        for( y=0; y<gridH; y++ ) {
            
            if( y > 0 ) {
                spaces[x][y]->mNeighbors[ GridSpace::top ] = spaces[x][y-1];
                }
            if( y < gridH-1 ) {
                spaces[x][y]->mNeighbors[ GridSpace::bottom ] = spaces[x][y+1];
                }
            if( x > 0 ) {
                spaces[x][y]->mNeighbors[ GridSpace::left ] = spaces[x-1][y];
                }
            if( x < gridW-1 ) {
                spaces[x][y]->mNeighbors[ GridSpace::right ] = spaces[x+1][y];
                }
            }
        }
    }

void freeFrameDrawer() {
    freeSpriteBank();

    for( int i=0; i<numGridSpaces; i++ ) {
        delete allSpaces[i];
        }
    }



void drawFrame() {
    glColor4d( 1.0, 1.0, 1.0, 1.0 );
    glBegin( GL_LINES ); {
        
        glVertex2d( 0, 0 );
        glVertex2d( pointerX, pointerY );
        }
    glEnd();
    

    for( int i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->draw();
        }
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
