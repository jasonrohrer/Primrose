

#include "GridSpace.h"

#include "spriteBank.h"
#include "numeral.h"

#include "gameControl.h"



#include <math.h>

#include <GL/gl.h>






GridSpace::GridSpace( int inX, int inY )
        :mX( inX ), mY( inY ), mActive( false ), mVisited( false ),
         mChecked( false ),
         mMarkedForClearing( false ),
         mScore( 0 ),
         mDrawColor( NULL ),
         mPieceColor( NULL ), mLastColor( NULL ),
         mColorShiftProgress( 0 ),
         mBrightHalo( false ), 
         mBrightHaloProgress( 0 ),
         mActiveProgress( 0 ),
         mScoreFade( 0 ),
         mScoreSent( false ),
         mSavedColor( NULL ),
         mSavedActive( false ) {

    for( int n=0; n<4; n++ ) {
        mNeighbors[n] = NULL;
        }
    
    }


GridSpace::~GridSpace() {
    if( mLastColor != NULL ) {
        delete mLastColor;
        }
    if( mPieceColor != NULL ) {
        delete mPieceColor;
        }
    if( mDrawColor != NULL ) {
        delete mDrawColor;
        }
    }



char GridSpace::isInside( int inX, int inY ) {
    if( fabs( inX - mX ) < 20
        &&
        fabs( inY - mY ) < 20 ) {
        
        return true;
        }
    
    return false;
    }



void GridSpace::setColor( Color *inColor ) {
    
    if( mLastColor != NULL ) {
        delete mLastColor;
        }
    
    mLastColor = mPieceColor;
    
    mPieceColor = inColor;
    
    mColorShiftProgress = 0;
    }



void GridSpace::saveState() {
    if( mSavedColor != NULL ) {
        delete mSavedColor;
        }
    if( mPieceColor != NULL ) {
        mSavedColor = mPieceColor->copy();
        }
    else {
        mSavedColor = NULL;
        }
            
    mSavedActive = mActive;
    }
        
        

void GridSpace::rewindState() {
    mActive = mSavedActive;
            
    if( mSavedColor != NULL ) {
        setColor( mSavedColor->copy() );
        }
    else {
        setColor( NULL );
        }
    }



Color gridLineColor( 1, 1, 1 );


void GridSpace::drawGrid( float inAlpha ) {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    drawSprite( gridLineTop, mX, mY, 32, 32, &gridLineColor, inAlpha );
    
    drawSprite( gridLineLeft, mX, mY, 32, 32, &gridLineColor, inAlpha );
    

    if( mNeighbors[bottom] == NULL ) {
        drawSprite( gridLineBottom, mX, mY, 32, 32, &gridLineColor, inAlpha );
        }
    
    if( mNeighbors[right] == NULL ) {
        drawSprite( gridLineRight, mX, mY, 32, 32, &gridLineColor, inAlpha );
        }
    
    if( mActiveProgress > 0 ) {
                
        drawSprite( plus, mX, mY, 16, 16, &gridLineColor, 
                    inAlpha * mActiveProgress );
        }
    

    glDisable( GL_BLEND );
    
    }


Color scorePipColor( 255/255.0, 255/255.0, 160/255.0 );



void GridSpace::drawPieceCenter() {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    if( mDrawColor != NULL ) {
        drawSprite( pieceCenter, mX, mY, 32, 32, mDrawColor, mDrawColor->a );
        }

    if( mPieceColor == NULL && mScore > 0 ) {
        
        drawScorePip( mScore, mX, mY, &scorePipColor, 
                      mColorShiftProgress * mScoreFade );
        
        }
    

    glDisable( GL_BLEND );
    }



void GridSpace::drawPieceHalo() {
    

    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    if( mDrawColor != NULL ) {
        drawSprite( pieceHalo, mX, mY, 32, 32, mDrawColor, mDrawColor->a );
    
        if( mBrightHalo ) {
            //for( int i=0; i<4; i++ ) {
                
                drawSprite( pieceBrightHalo, mX, mY, 32, 32, mDrawColor, 
                            mBrightHaloProgress * mDrawColor->a );
                //}
                
                Color white( 1, 1, 1, 1 );
                
                glBlendFunc( GL_SRC_ALPHA, GL_ONE );
                drawSprite( pieceBrightCenter, mX, mY, 32, 32, &white, 
                            mBrightHaloProgress * mDrawColor->a );

            
            }
        }
    
    

    glDisable( GL_BLEND );
    }



void GridSpace::step() {

    if( mDrawColor != NULL ) {
        delete mDrawColor;
        mDrawColor = NULL;
        }
    
    if( mLastColor != NULL || mPieceColor != NULL ) {
        
        Color *blendA;
        if( mLastColor == NULL ) {
            // piece color with total transparency
            blendA = mPieceColor->copy();
            blendA->a = 0;
            }
        else {
            blendA = mLastColor->copy();
            }
        
        
        Color *blendB;
        if( mPieceColor == NULL ) {
            // last color with total transparency
            blendB = mLastColor->copy();
            blendB->a = 0;
            }
        else {
            blendB = mPieceColor->copy();
            }
        
                
        
        mDrawColor = Color::linearSum( blendB, blendA, mColorShiftProgress );
        

        delete blendA;
        delete blendB;
        
        if( mLastColor != NULL && mPieceColor != NULL ) {
            // slow shift between colors
            mColorShiftProgress += 0.1;
            }
        else {
            // faster shift to on or off
            mColorShiftProgress += 0.2;
            }
        
        
        if( mColorShiftProgress > 1 ) {
            mColorShiftProgress = 1;
            }
        
        }
    

    if( mBrightHalo ) {

        if( mBrightHaloProgress == 1 ) {
            
            if( mPieceColor != NULL ) {
                
                


                setColor( NULL );

                
                }
            else if( mColorShiftProgress == 1 ) {
                // completely faded out
                
                mBrightHaloProgress = 0;
                mBrightHalo = false;
                }
            }
        else {
            // slower than color switch time
            mBrightHaloProgress += 0.1;
            
            if( mBrightHaloProgress > 1 ) {
                mBrightHaloProgress = 1;
                }
            }
        
        }

    if( mScoreFade > 0 && !mBrightHalo && mColorShiftProgress == 1 ) {
        
        // only add to score once
        if( !mScoreSent ) {
            addToScore( mScore );
            
            mScoreSent = true;
            }
        

        mScoreFade -= 0.075;
        
        if( mScoreFade < 0 ) {
            mScoreFade = 0;
            }
        }
    
    

    if( mActive && mActiveProgress < 1 ) {
        mActiveProgress += 0.2;
        
        if( mActiveProgress > 1 ) {
            mActiveProgress = 1;
            }
        }
    else if( !mActive && mActiveProgress > 0 ) {
        mActiveProgress -= 0.2;
        
        if( mActiveProgress < 0 ) {
            mActiveProgress = 0;
            }
        }
    

            
    }



char GridSpace::isAnimationDone() { 

    if( mDrawColor == NULL ) {
        return true;
        }    
    else if( mColorShiftProgress == 1 && ! mBrightHalo && mScoreFade == 0 ) {
        return true;
        }

    return false;
    }



char GridSpace::colorMatches( Color *inColor ) {
    if( isEmpty() ) {
        if( inColor == NULL ) {
            return true;
            }
        else {
            return false;
            }
        }
    else if( inColor != NULL ) {
        return inColor->equals( mPieceColor );
        }
    else {
        return false;
        }
    }



Color *GridSpace::checkSurrounded() {
    mVisited = true;
    mChecked = true;
    
    
    if( isEmpty() ) {
        return NULL;
        }
    

    int n;
    
    // first, make sure all neighbors have our color or a single other color
    Color *surroundColor = NULL;
    
    for( n=0; n<4; n++ ) {
        GridSpace *space = mNeighbors[n];
        
        if( space != NULL ) {
            

            if( space->isEmpty() ) {
                // neighbor empty, not surrounded

                if( surroundColor != NULL ) {
                    delete surroundColor;
                    }
                return NULL;
                }
                    
            if( ! space->colorMatches( mPieceColor ) ) {
                            
                if( surroundColor == NULL ) {
                    surroundColor = space->getColor();
                    }
                else if( ! space->colorMatches( surroundColor ) ) {
                    // surrounded by more than one color
                    
                    if( surroundColor != NULL ) {
                        delete surroundColor;
                        }
                    return NULL;
                    }
                }
            }
        }
    

    // have surround color, or NULL if surrounded by like-colored pieces
    // and edges

    // no neighbors are empty

    // check each like-colored, unvisited neighbor
        
    for( n=0; n<4; n++ ) {
        GridSpace *space = mNeighbors[n];
        
        if( space != NULL ) {

            if( ! space->mVisited ) {
                
                if( space->colorMatches( mPieceColor ) ) {
                    
                    Color *c = space->checkSurrounded();
                    
                    if( c == NULL ) {
                        if( surroundColor != NULL ) {
                            delete surroundColor;
                            }
                        return NULL;
                        }
                    

                    if( ! c->equals( mPieceColor ) ) {
                        
                        if( surroundColor == NULL ) {
                            surroundColor = c;
                            }
                        else if( ! surroundColor->equals( c ) ) {
                            delete surroundColor;
                            delete c;
                        
                            // color mismatch
                            return NULL;
                            }
                        }
                    }
                
                }
            
            }
        
        }



    // done checking neighbors... if we got here, we have our surrounding
    // color... or NULL if surrounded by edges

    if( surroundColor == NULL ) {
        return mPieceColor->copy();
        }
    else {
        return surroundColor;
        }
    }



void GridSpace::flipToClear() {
    mBrightHaloProgress = 0;
    mBrightHalo = true;
    
    mScoreFade = 1;
    mScoreSent = false;
    

    // flip any other-colored neighbors (if they are not also clearing)

    for( int n=0; n<4; n++ ) {
        GridSpace *space = mNeighbors[n];
        
        if( space != NULL ) {
            if( ! space->isEmpty() 
                && 
                ! space->mMarkedForClearing
                &&
                ! space->colorMatches( mPieceColor ) ) {
                
                space->setColor( mPieceColor->copy() );
                
                }                        
            }
        }
    }


