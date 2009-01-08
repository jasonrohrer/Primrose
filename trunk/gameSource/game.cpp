#include "game.h"
#include "spriteBank.h"

#include "GridSpace.h"
#include "NextPieceDisplay.h"


#include <stdio.h>

#include <GL/gl.h>


#include "minorGems/util/random/StdRandomSource.h"


StdRandomSource randSource;



float pointerX, pointerY;


#define gridW 7
#define gridH 7
#define numGridSpaces 49

GridSpace *spaces[gridW][gridH];
GridSpace *allSpaces[ numGridSpaces ];


NextPieceDisplay *nextPiece;


// true if placement in progress
char piecePlaced = false;



void initFrameDrawer( int inWidth, int inHeight ) {
    pointerX = -100;
    pointerY = -100;

    initSpriteBank();

    int i=0;
    int x;
    int y;
    
    for( y=0; y<gridH; y++ ) {
        for( x=0; x<gridW; x++ ) {
            
            allSpaces[i] = 
            spaces[y][x] =
                new GridSpace( x * 40 + 19 + 21, y * 40 + 19 + 21 );
            
            i++;
            }
        }
    
    // set neighbor relationships
    for( y=0; y<gridH; y++ ) {
        for( x=0; x<gridW; x++ ) {
        
            
            if( y > 0 ) {
                spaces[y][x]->mNeighbors[ GridSpace::top ] = spaces[x][y-1];
                }
            if( y < gridH-1 ) {
                spaces[y][x]->mNeighbors[ GridSpace::bottom ] = spaces[x][y+1];
                }
            if( x > 0 ) {
                spaces[y][x]->mNeighbors[ GridSpace::left ] = spaces[x-1][y];
                }
            if( x < gridW-1 ) {
                spaces[y][x]->mNeighbors[ GridSpace::right ] = spaces[x+1][y];
                }
            }
        }


    /*
      nextPiece = new NextPieceDisplay( 19 + 21,
                                      inHeight - (19 + 41) );
    */

    // center below grid
    nextPiece = new NextPieceDisplay( spaces[6][3]->mX,
                                      spaces[6][3]->mY + 40 + 20 + 19 );
    }




void freeFrameDrawer() {
    freeSpriteBank();

    for( int i=0; i<numGridSpaces; i++ ) {
        delete allSpaces[i];
        }
    delete nextPiece;
    }





void drawFrame() {
    

    int i;
    
    char animDone = true;
    
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->step();

        animDone &= allSpaces[i]->isAnimationDone();
        }
    nextPiece->step();
    
    if( piecePlaced && animDone ) {
        // done with placement, all placement animations done

        nextPiece->update();
        
        piecePlaced = false;
        }
    


    
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->drawGrid();
        }
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->drawPieceCenter();
        }
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->drawPieceHalo();
        }

    nextPiece->draw();
    
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





int nextColor = 0;



void pointerUp( float inX, float inY ) {
    pointerX = inX;
    pointerY = inY;
    
    if( piecePlaced ) {
        // last placement still in progress
        // ignore
        return;
        }
    

    for( int i=0; i<numGridSpaces; i++ ) {
        
        if( allSpaces[i]->isInside( (int)pointerX, (int)pointerY )
            &&
            allSpaces[i]->isEmpty() ) {
            
                        
            allSpaces[i]->setColor( nextPiece->getNextPiece() );
            piecePlaced = true;
            
            nextColor++;
            
            nextColor %= 8;
            
            }
        
        }
    
    }
