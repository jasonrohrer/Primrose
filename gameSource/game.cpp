#include "game.h"
#include "spriteBank.h"

#include "GridSpace.h"
#include "NextPieceDisplay.h"
#include "numeral.h"


#include <stdio.h>
#include <math.h>

#include <GL/gl.h>


#include "minorGems/util/random/StdRandomSource.h"


StdRandomSource randSource;



float pointerX, pointerY;


#define gridW 7
#define gridH 7
#define numGridSpaces 49

Color scoreColor( 255/255.0, 255/255.0, 160/255.0 );


GridSpace *spaces[gridW][gridH];
GridSpace *allSpaces[ numGridSpaces ];


NextPieceDisplay *nextPiece;


// true if placement in progress
char piecePlaced = false;


// grid spot where last piece placed
int lastGridX = -1;
int lastGridY = -1;

int score = 0;

int chainLength = 1;



void newGame() {
    
    lastGridX = -1;
    lastGridY = -1;
    score = 0;
    chainLength = 1;
    

    pointerX = -100;
    pointerY = -100;


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
                spaces[y][x]->mNeighbors[ GridSpace::top ] = spaces[y-1][x];
                }
            if( y < gridH-1 ) {
                spaces[y][x]->mNeighbors[ GridSpace::bottom ] = spaces[y+1][x];
                }
            if( x > 0 ) {
                spaces[y][x]->mNeighbors[ GridSpace::left ] = spaces[y][x-1];
                }
            if( x < gridW-1 ) {
                spaces[y][x]->mNeighbors[ GridSpace::right ] = spaces[y][x+1];
                }
            }
        }


    /*
      nextPiece = new NextPieceDisplay( 19 + 21,
                                      inHeight - (19 + 41) );
    */

    // center below grid
    nextPiece = new NextPieceDisplay( spaces[6][3]->mX,
                                      spaces[6][3]->mY + 40 + 20 + 19 + 1 );
    
    }


void endGame() {
    
    for( int i=0; i<numGridSpaces; i++ ) {
        delete allSpaces[i];
        }
    delete nextPiece;
    }





void initFrameDrawer( int inWidth, int inHeight ) {

    initSpriteBank();

    newGame();
    }




void freeFrameDrawer() {
    freeSpriteBank();
    
    endGame();
    }




// one round of non-chained clearing
// returns true if some pieces cleared
char checkAndClear() {
    char someCleared = false;
    
    int i;
    int j;

    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->mVisited =  false;
        allSpaces[i]->mChecked =  false;
        allSpaces[i]->mMarkedForClearing =  false;
        }


    for( j=0; j<numGridSpaces; j++ ) {
        // clear visited flags so that they only mark one group
        // at a time
        for( i=0; i<numGridSpaces; i++ ) {
            allSpaces[i]->mVisited =  false;
            }
        

        GridSpace *space = allSpaces[j];
        
        if( ! space->mChecked ) {
            
            Color *c = space->checkSurrounded();
        
            if( c != NULL ) {
                delete c;
                
                // all visited spaces part of group
                
                // tally score
                int score = 0;
                
                for( i=0; i<numGridSpaces; i++ ) {
                    if( allSpaces[i]->mVisited ) {
                        allSpaces[i]->mMarkedForClearing = true;
                        score ++;
                        }
                    }
                
                // set score for them all
                for( i=0; i<numGridSpaces; i++ ) {
                    if( allSpaces[i]->mVisited ) {
                        allSpaces[i]->mScore = 
                            (int)( pow( score, 2 ) )
                            *
                            (int)( pow( chainLength, 3 ) );
                        }
                    }
                
                
                }            
            }
        }

    // clear any that are marked 
    for( i=0; i<numGridSpaces; i++ ) {
        if( allSpaces[i]->mMarkedForClearing ) {
            allSpaces[i]->flipToClear();
            someCleared = true;
            }
        }

    if( someCleared ) {
        chainLength ++;
        }
    
    return someCleared;
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

        if( !checkAndClear() ) {
            

            nextPiece->update();
        
            piecePlaced = false;

        
            int i;
        

            if( nextPiece->isSecondPiece() ) {
            
                // set grid spaces in same row/column to active
                int x;
                int y;
            
            
                char anyActive = false;
            
                for( y=0; y<gridH; y++ ) {
                    GridSpace *space = spaces[y][lastGridX];
                
                    if( space->isEmpty() ) {
                        space->mActive = true;
                        anyActive = true;
                        }
                    }
            
                for( x=0; x<gridW; x++ ) {
                    GridSpace *space = spaces[lastGridY][x];
                
                    if( space->isEmpty() ) {
                        space->mActive = true;
                        anyActive = true;
                        }
                    }

            
                if( !anyActive ) {
                    // all empty active
                    for( i=0; i<numGridSpaces; i++ ) {
                        if( allSpaces[i]->isEmpty() ) {
                            allSpaces[i]->mActive = true;
                            }
                        }
                    }
                }

            }
        
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
    
    
    //drawScore( score, 320 - 19, nextPiece->mY );
    drawScore( 3409384, 320 - 19, nextPiece->mY + 41, &scoreColor );
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
    
    if( piecePlaced ) {
        // last placement still in progress
        // ignore
        return;
        }
    
    char considerNonActive = true;
    if( nextPiece->isSecondPiece() ) {
        considerNonActive = false;
        }
    

    int x;
    int y;
    
    for( y=0; y<gridH; y++ ) {
        for( x=0; x<gridW; x++ ) {
        

            GridSpace *space = spaces[y][x];
            
            if( space->isInside( (int)pointerX, (int)pointerY )
                &&
                space->isEmpty()
                && 
                ( considerNonActive || space->mActive ) ) {
                
                
                space->setColor( nextPiece->getNextPiece() );
                piecePlaced = true;
                
                
                lastGridY = y;
                lastGridX = x;
                
                // reset chain length counter
                chainLength = 1;
                
                
                if( !considerNonActive ) {
                    // just placed in an active spot (second piece)

                    int i;
        
                    // all to non-active
                    for( i=0; i<numGridSpaces; i++ ) {
                        allSpaces[i]->mActive = false;
                        }
                    }
                
                }
            }
        
        
        }
    
    }
