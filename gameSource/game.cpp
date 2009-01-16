#include "game.h"
#include "spriteBank.h"

#include "GridSpace.h"
#include "NextPieceDisplay.h"
#include "numeral.h"
#include "ColorPool.h"

#include "Button.h"

#include "Panel.h"
#include "MenuPanel.h"



#include <stdio.h>
#include <math.h>

#include <GL/gl.h>


#include "minorGems/util/random/CustomRandomSource.h"
#include "minorGems/util/SettingsManager.h"


unsigned int gameSeed = time( NULL );

CustomRandomSource randSource( gameSeed );


int screenW, screenH;


float pointerX, pointerY;


#define gridW 7
#define gridH 7
#define numGridSpaces 49

Color scoreColor( 255/255.0, 255/255.0, 160/255.0 );


GridSpace *spaces[gridW][gridH];
GridSpace *allSpaces[ numGridSpaces ];


#define numButtons 3

Button *allButtons[ numButtons ];

Button *undoButton;
Button *menuButton;
Button *doneButton;


#define numPanels 1

Panel *allPanels[ numPanels ];

Panel *menuPanel;



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

// for rewind
int savedScore = 0;


int chainLength = 1;


// for scheduling a restart
char mustRestart = false;


char playerName[9];



// a subset of base-64 encoding
// converts numbers in range [0,48] to alphabetical characters
// uses the first 49 digits of base64 as described here:
//   http://tools.ietf.org/html/rfc4648
char base49Encode( int inNumber ) {
    if( inNumber < 26 ) {
        return 'A' + inNumber;
        }
    else {
        return 'a' + (inNumber - 26);
        }
    }


SimpleVector<char> moveHistory;





void newGame() {
    moveHistory.deleteAll();
    
    gameSeed = time( NULL );
    randSource.reseed( gameSeed );
    
    
    lastGridX = -1;
    lastGridY = -1;
    score = 0;

    lastScore = 0;
    scoreTransitionProgress = 1;
    
    savedScore = 0;
    

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
    
    undoButton = new Button( nextPiece->mX - 60, nextPiece->mY - 20, "undo" );
    
    
    menuButton = new Button( spaces[0][0]->mX, colorPool->mY - 41 - 19, "p" );

    doneButton = new Button( undoButton->mX, 
                             menuButton->mY, "done" );

    allButtons[0] = undoButton;
    allButtons[1] = menuButton;
    allButtons[2] = doneButton;

    undoButton->setVisible( false );
    menuButton->forceVisible();
    doneButton->setVisible( false );
    

    menuPanel = new MenuPanel( screenW, screenH );
    
    allPanels[0] = menuPanel;
    
    

    if( mustRestart ) {
        // previous menu panel requested a restart
        // show this new one at full blend at first, then fade out to 
        // restarted game
        menuPanel->forceVisible();
        }
    
    menuPanel->setVisible( false );
    }



void endGame() {
    
    int i;
    for( i=0; i<numGridSpaces; i++ ) {
        delete allSpaces[i];
        }
    for( i=0; i<numButtons; i++ ) {
        delete allButtons[i];
        }
    for( i=0; i<numPanels; i++ ) {
        delete allPanels[i];
        }
    
    delete nextPiece;
    delete colorPool;
    }



void restartGame() {
    mustRestart = true;
    }






void initFrameDrawer( int inWidth, int inHeight ) {
    for( int i=0; i<10; i++ ) {
        printf( "Rand output: %u\n", randSource.getRandomInt() );
        }
    

    screenW = inWidth;
    screenH = inHeight;
    
    initSpriteBank();

    
    char *nameSetting = SettingsManager::getStringSetting( "name" );
    
    if( nameSetting != NULL ) {
        int nameLength = strlen( nameSetting );
        
        if( nameLength > 8 ) {
            nameLength = 8;
            }
        
        memcpy( playerName, nameSetting, nameLength );

        playerName[ nameLength ] = '\0';
        
        delete nameSetting;
        }
    else {
        memcpy( playerName, "anon", 5 );
        }


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
    if( mustRestart ) {
        endGame();
        newGame();

        mustRestart = false;
        }
    

    int i;
    
    char animDone = true;
    
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->step();

        animDone &= allSpaces[i]->isAnimationDone();
        }
    for( i=0; i<numButtons; i++ ) {
        allButtons[i]->step();
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
        lastScore = score;
        
        if( !checkAndClear() ) {
            
            // check if board full
            char full = true;
            
            for( i=0; i<numGridSpaces && full; i++ ) {
                if( allSpaces[i]->isEmpty() ) {
                    full = false;
                    }
                }
            
            if( full ) {
                // game over
                doneButton->setVisible( true );
                }

            

            nextPiece->update();
            
            piecePlaced = false;

        
            
        

            if( nextPiece->isSecondPiece() ) {
                // allow undo of first piece
                
                if( !full ) {
                    undoButton->setVisible( true );
                    }
                
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
            else {
                // placing new first piece
                undoButton->setVisible( false );
                }
            

            }
        
        }
    


    
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->drawGrid();
        }

    for( i=0; i<numButtons; i++ ) {
        allButtons[i]->draw();
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
    

    for( i=0; i<numPanels; i++ ) {
        allPanels[i]->step();
        }
    
    for( i=0; i<numPanels; i++ ) {
        allPanels[i]->draw();
        }
    
    }



void undoMove() {
    int i;
    
    for( i=0; i<numGridSpaces; i++ ) {
        allSpaces[i]->rewindState();
        }
    nextPiece->rewindState();
    
    colorPool->deRegisterMove();
    
    randSource.rewindState();


    if( moveHistory.size() > 0 ) {
        moveHistory.deleteElement( moveHistory.size() - 1 );
        }
    

    int temScore = score;
    score = savedScore;
    lastScore = temScore;
    
    scoreTransitionProgress = 0;
    
    
    // only one step of undo allowed
    undoButton->setVisible( false );
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
    
    
    // pass through to visible panels
    char somePanelVisible = false;
    
    int i;
    
    for( i=0; i<numPanels; i++ ) {
        if( allPanels[i]->isVisible() ) {
            somePanelVisible = true;
            allPanels[i]->pointerUp( (int)pointerX, (int)pointerY );
            }
        }

    if( somePanelVisible ) {
        // don't pass click to grid
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
                
                int i;
                
                i = y * gridW + x;
                
                char encodedMove = base49Encode( i );
                
                moveHistory.push_back( encodedMove );


                // save previous state for undo
                for( i=0; i<numGridSpaces; i++ ) {
                    allSpaces[i]->saveState();
                    }
                nextPiece->saveState();
                
                randSource.saveState();
                
                savedScore = score;
                
                
                space->setColor( nextPiece->getNextPiece() );
                piecePlaced = true;
                levelOfPlacement = colorPool->getLevel();
                
                colorPool->registerMove();
                            
                
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

    for( i=0; i<numButtons; i++ ) {
        if( allButtons[i]->isVisible() && 
            allButtons[i]->isInside( (int)pointerX, (int)pointerY ) ) {
            
            if( allButtons[i] == undoButton ) {
                undoMove();
                }
            else if( allButtons[i] == menuButton ) {
                menuPanel->setVisible( true );
                }
            
            }
        }
    

    
    
    }




void addToScore( int inPointsToAdd ) {
    
    score += inPointsToAdd;
    
    scoreTransitionProgress = 0;
    }



char *getName() {
    return playerName;
    }



void saveName() {
    
    if( strlen( playerName ) == 0 ) {
        // default to anon
        memcpy( playerName, "anon", 5 );
        }
    

    SettingsManager::setSetting( "name", playerName );
    }
