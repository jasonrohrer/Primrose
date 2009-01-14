#include "game.h"
#include "spriteBank.h"

#include "GridSpace.h"
#include "NextPieceDisplay.h"
#include "numeral.h"
#include "ColorPool.h"


#include <stdio.h>
#include <math.h>

#include <GL/gl.h>


#include "minorGems/util/random/StdRandomSource.h"


StdRandomSource randSource;


int screenW, screenH;


float pointerX, pointerY;


#define gridW 7
#define gridH 7
#define numGridSpaces 49

Color scoreColor( 255/255.0, 255/255.0, 160/255.0 );


GridSpace *spaces[gridW][gridH];
GridSpace *allSpaces[ numGridSpaces ];

ColorPool *colorPool;

NextPieceDisplay *nextPiece;


// true if placement in progress
char piecePlaced = false;
// track the level of the piece just place (in case the level increments
// due to this placement)
int levelOfPlacement = 1;


// grid spot where last piece placed
int lastGridX = -1;
int lastGridY = -1;

int score = 0;

// for smooth transition between scores
int lastScore = 0;
float scoreTransitionProgress = 1;


int chainLength = 1;



void newGame() {
    
    lastGridX = -1;
    lastGridY = -1;
    score = 0;

    lastScore = 0;
    scoreTransitionProgress = 1;
    
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
    
    colorPool = new ColorPool( spaces[6][3]->mX,
                               screenH - 20 - 19 - 1 );
    
    nextPiece = new NextPieceDisplay( spaces[6][3]->mX,
                                      spaces[6][3]->mY + 40 + 20 + 19 + 1,
                                      colorPool );
    
    }


void endGame() {
    
    for( int i=0; i<numGridSpaces; i++ ) {
        delete allSpaces[i];
        }
    delete nextPiece;
    delete colorPool;
    }





void initFrameDrawer( int inWidth, int inHeight ) {
    screenW = inWidth;
    screenH = inHeight;
    
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
                            (int)( pow( chainLength, 3 ) )
                            * levelOfPlacement;
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
    
    colorPool->step();
    

    if( piecePlaced && animDone ) {
        // done with placement, all placement (or clearing) animations done

        
        /*
        // accumulate any points scored
        lastScore = score;
        
        for( i=0; i<numGridSpaces; i++ ) {
            if( allSpaces[i]->mScore > 0 ) {
                score += allSpaces[i]->mScore;
                
                allSpaces[i]->mScore = 0;
                }
            }
        
        if( lastScore != score ) {
            scoreTransitionProgress = 0;
            }
        */



        // check if any more clear (which will start more animations)
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
    
    colorPool->draw();
    
    
    // additive needed for smooth cross-fade between last score and new score
    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    if( scoreTransitionProgress < 1 ) {
        
        drawScore( lastScore, 320 - 19, nextPiece->mY + 41, &scoreColor,
                   1 - scoreTransitionProgress );
        }
    
    drawScore( score, 320 - 19, nextPiece->mY + 41, &scoreColor,
               scoreTransitionProgress );
    
    glDisable( GL_BLEND );
    

    if( scoreTransitionProgress < 1 ) {
        scoreTransitionProgress += 0.075;
        if( scoreTransitionProgress > 1 ) {
            scoreTransitionProgress = 1;
            }
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
                levelOfPlacement = colorPool->getLevel();
                
                colorPool->registerMove();
                            
                
                lastGridY = y;
                lastGridX = x;
                
                lastScore = score;
                

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




void addToScore( int inPointsToAdd ) {
    
    score += inPointsToAdd;
    
    scoreTransitionProgress = 0;
    }
