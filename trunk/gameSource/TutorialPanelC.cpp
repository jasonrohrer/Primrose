#include "TutorialPanelC.h"

#include "numeral.h"

#include "gameControl.h"


#include <GL/gl.h>


extern Color nextPieceDemoColors[3];


TutorialPanelC::TutorialPanelC( int inW, int inH )
        : Panel( inW, inH ),
          mColorPool( inW/2, inH - 20 - 19 - 1 - 20 ),
          mDemoStage( 0 ),
          mStepsBetweenStages( 50 ), mStepCount( 0 ) {



    int x, y, i;
    
    i=0;
    
    int gridH = 4;
    int gridW = 7;
    
    int yOffset = inH / 3 - 12;
    
    for( y=0; y<gridH; y++ ) {
        for( x=0; x<gridW; x++ ) {
            
            mAllDemoSpaces[i] = 
            mGridDemo[y][x] =
                new GridSpace( x * 40 + 19 + 21, y * 40 + yOffset + 21 );
          
            mAllDemoSpaces[i]->mAddToGlobalScore = false;
            
            
            i++;
            }
        }
    
    // set neighbor relationships
    for( y=0; y<gridH; y++ ) {
        for( x=0; x<gridW; x++ ) {
        
            
            if( y > 0 ) {
                mGridDemo[y][x]->mNeighbors[ GridSpace::top ] = 
                    mGridDemo[y-1][x];
                }
            if( y < gridH-1 ) {
                mGridDemo[y][x]->mNeighbors[ GridSpace::bottom ] = 
                    mGridDemo[y+1][x];
                }
            if( x > 0 ) {
                mGridDemo[y][x]->mNeighbors[ GridSpace::left ] = 
                    mGridDemo[y][x-1];
                }
            if( x < gridW-1 ) {
                mGridDemo[y][x]->mNeighbors[ GridSpace::right ] = 
                    mGridDemo[y][x+1];
                }
            }
        }
    
    mInnerSpacesA[0] = mGridDemo[1][2];

    mInnerSpacesB[0] = mGridDemo[1][4];
    mInnerSpacesB[1] = mGridDemo[1][5];
    mInnerSpacesB[2] = mGridDemo[1][6];
    mInnerSpacesB[3] = mGridDemo[2][6];

    mOuterSpacesA[0] = mGridDemo[0][2];
    mOuterSpacesA[1] = mGridDemo[1][3];
    mOuterSpacesA[2] = mGridDemo[2][2];

    mOuterSpacesB[0] = mGridDemo[0][4];
    mOuterSpacesB[1] = mGridDemo[0][5];
    mOuterSpacesB[2] = mGridDemo[0][6];
    mOuterSpacesB[3] = mGridDemo[3][6];
    mOuterSpacesB[4] = mGridDemo[2][5];
    mOuterSpacesB[5] = mGridDemo[2][4];
    
    mKeySpace = mGridDemo[1][1];

    setStageZero();
    }



void TutorialPanelC::setStageZero() {
    
    int i;
    
    for( i=0; i<1; i++ ) {
        mInnerSpacesA[i]->setColor( nextPieceDemoColors[1].copy() );
        }
    for( i=0; i<4; i++ ) {
        mInnerSpacesB[i]->setColor( nextPieceDemoColors[2].copy() );
        }

    for( i=0; i<3; i++ ) {
        mOuterSpacesA[i]->setColor( nextPieceDemoColors[0].copy() );
        }
    for( i=0; i<6; i++ ) {
        mOuterSpacesB[i]->setColor( nextPieceDemoColors[1].copy() );
        }

    mKeySpace->setColor( NULL );
    
    }


    
TutorialPanelC::~TutorialPanelC() {
    for( int i=0; i<28; i++ ) {
        delete mAllDemoSpaces[i];
        }
    }



void TutorialPanelC::setVisible( char inIsVisible ) {
    Panel::setVisible( inIsVisible );
    
    }



char TutorialPanelC::pointerUp( int inX, int inY ) {
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {
        /*
        if( mNextButton.isInside( inX, inY ) ) {
            
            //mNextPanel.setVisible( true );
            
            return true;
            }
        */
        }
    
    return false;
    }



void TutorialPanelC::step() {
    Panel::step();
    
    mColorPool.step();
    
        
    int i;

    for( i=0; i<28; i++ ) {
        mAllDemoSpaces[i]->step();
        }
        
    
    if( isVisible() && ! isSubPanelVisible() ) {
        mStepCount++;
        }
    else {
        // return to step 0
        mDemoStage = 0;
        mStepCount = 0;
        }
    


    // check if we need to revert back to stage zero
    // this might happen after we close or after displaying a sub-panel
    if( mDemoStage == 0 
        && 
        ( ! mKeySpace->isEmpty() 
          || mInnerSpacesA[0]->isEmpty() 
          || mInnerSpacesB[0]->isEmpty() )
        && 
        mInnerSpacesA[0]->isAnimationDone()
        &&
        mInnerSpacesB[0]->isAnimationDone() ) {
        
        // lingering at end of score animation
        
        setStageZero();
        }
        
    

    if( mStepCount >= mStepsBetweenStages ) {
        

        mStepCount = 0;
        
        mDemoStage++;
        
        if( mDemoStage > 6 ) {
            mDemoStage = 0;
            }
        

        if( mDemoStage == 0 ) {
            setStageZero();
            }
        if( mDemoStage == 1 ) {
            // hold zero longer
            }
        else if( mDemoStage == 2 ) {
            mKeySpace->setColor( nextPieceDemoColors[0].copy() );
            }
        else if( mDemoStage == 3 ) {
            for( i=0; i<1; i++ ) {
                mInnerSpacesA[i]->mScore = 1;
                mInnerSpacesA[i]->flipToClear();
                }
            }
        else if( mDemoStage == 4 ) {
            for( i=0; i<4; i++ ) {
                mInnerSpacesB[i]->mScore = 256;
                mInnerSpacesB[i]->flipToClear();
                }
            }
        else if( mDemoStage == 5 ) {
            // hold stage 3 for extra step
            }
        else if( mDemoStage == 6 ) {
            // blank before next cycle
            for( i=0; i<28; i++ ) {
                mAllDemoSpaces[i]->setColor( NULL );
                }
            // this stage short
            mStepCount += mStepsBetweenStages / 2;
            }
        
        
            

        }
    
    }



extern Color tutorialTextColor;

        
void TutorialPanelC::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        drawString( "chain reactions are possible", left, 
                    19,
                    mGridDemo[0][0]->mY - 20 - 60,
                    &tutorialTextColor, mFadeProgress );        
        drawString( "and longer chains score even", left, 
                    19,
                    mGridDemo[0][0]->mY - 20 - 40,
                    &tutorialTextColor, mFadeProgress );        
        drawString( "more points", left, 
                    19,
                    mGridDemo[0][0]->mY - 20 - 20,
                    &tutorialTextColor, mFadeProgress );        

        drawString( "make use of grid edges too", left, 
                    19,
                    mGridDemo[3][0]->mY + 20 + 20,
                    &tutorialTextColor, mFadeProgress );        


        drawString( "three colors to start and", left, 
                    19,
                    mColorPool.mY - 20 - 40,
                    &tutorialTextColor, mFadeProgress );        

        drawString( "next color in ninety six moves", left, 
                    19,
                    mColorPool.mY - 20 - 20,
                    &tutorialTextColor, mFadeProgress );        

        drawString( "score even more in later phases", left, 
                    19,
                    mColorPool.mY + 20 + 20,
                    &tutorialTextColor, mFadeProgress );        
        

        glDisable( GL_BLEND );
        

        int i;
        
        int numGridSpaces = 28;
        
        for( i=0; i<numGridSpaces; i++ ) {
            mAllDemoSpaces[i]->drawGrid( mFadeProgress );
            }
        for( i=0; i<numGridSpaces; i++ ) {
            mAllDemoSpaces[i]->drawPieceCenter( mFadeProgress );
            }
        for( i=0; i<numGridSpaces; i++ ) {
            mAllDemoSpaces[i]->drawPieceHalo( mFadeProgress );
            }
        

        mColorPool.draw( mFadeProgress );
        
        }
    

    
    }



void TutorialPanelC::closePressed() {
    // return to step 0
    mDemoStage = 0;
    mStepCount = 0;
    

    // can't do this safely here because score animation might still be
    // running 
    // setStageZero();
    }


        
        