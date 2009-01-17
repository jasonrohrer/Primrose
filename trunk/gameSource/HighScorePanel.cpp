#include "HighScorePanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"


#include <GL/gl.h>



HighScorePanel::HighScorePanel( int inW, int inH, Panel *inLoadingPanel )
        : Panel( inW, inH ),
          mLoadingPanel( inLoadingPanel ) {
    

    int i=0;
    
    int xStep = inW / 2 - 10;
    int xStart = inW / 2 - (21 + 9);
    
    
    int yStart = 19 + 21 + 40;
    int yStep = ( inH / 2 - yStart - 21 ) / 3;
    

    int x, y;
    
    // grid of 2x4
    for( x=0; x<2; x++ ) {
        for( y=0; y<4; y++ ) {
                            
            mAllTimeButtons[i] = new Button( xStart + x * xStep,
                                             yStart + y * yStep,
                                             "play" );
    
            addButton( mAllTimeButtons[i] );
            
            i++;
            }
        }



    i = 0;
    
    yStart += inH / 2 - 18;
    
    // grid of 2x4
    for( x=0; x<2; x++ ) {
        for( y=0; y<4; y++ ) {
                        
            mTodayButtons[i] = new Button( xStart + x * xStep,
                                             yStart + y * yStep,
                                             "play" );
    
            addButton( mTodayButtons[i] );
            
            i++;
            }
        }

    
    }


    
HighScorePanel::~HighScorePanel() {
    int i;
    
    for( i=0; i<8; i++ ) {
        delete mAllTimeButtons[i];
        
        delete mTodayButtons[i];
        }
    
    clearScores();
    }



void HighScorePanel::clearScores() {
    int i;

    for( i=0; i<mAllTimeScores.size(); i++ ) {
        delete *( mAllTimeScores.getElement( i ) );
        }
    for( i=0; i<mTodayScores.size(); i++ ) {
        delete *( mTodayScores.getElement( i ) );
        }

    mAllTimeScores.deleteAll();
    mTodayScores.deleteAll();
    }


        
void HighScorePanel::addAllTimeScore( ScoreBundle *inScore ) {
    mAllTimeScores.push_back( inScore );
    }



void HighScorePanel::addTodayScore( ScoreBundle *inScore ) {
    mTodayScores.push_back( inScore );
    }


void HighScorePanel::setVisible( char inIsVisible ) {
    Panel::setVisible( inIsVisible );

    
    // disable buttons for empty slots
    
    int i;
    
    for( i = mAllTimeScores.size(); i<8; i++ ) {
        mAllTimeButtons[i]->setVisible( false );
        }
    for( i = mTodayScores.size(); i<8; i++ ) {
        mTodayButtons[i]->setVisible( false );
        }
    
    }



char HighScorePanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {
        
        }
    
    return false;
    }




Color scoreHeaderColor( 76/255.0, 76/255.0, 255/255.0 );

Color scoreRuleColor( 1, 1, 1 );

Color scoreNumberColor( 255/255.0, 255/255.0, 160/255.0 );


        
void HighScorePanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        int headerX = mW / 2;
        

        drawStringBig( "all time", center, 
                       headerX,
                       mAllTimeButtons[0]->mY - 40,
                       &scoreHeaderColor, mFadeProgress );
        
        int i;
        
        for( i=0; i<mAllTimeScores.size() && i<8; i++ ) {
            ScoreBundle *b = *( mAllTimeScores.getElement( i ) );
            
            int textX = mAllTimeButtons[i]->mX - 21 - 10;
            
            drawScore( b->mScore,
                       textX,
                       mAllTimeButtons[i]->mY - 2,
                       &scoreNumberColor, mFadeProgress );
            
            drawString( b->mName, right,
                        textX,
                        mAllTimeButtons[i]->mY + 5 + 2,
                        &scoreHeaderColor, mFadeProgress );
            }


        int ruleY = mTodayButtons[0]->mY - 40 + 21;
        
        drawSprite( gridLineTop, headerX - mW/4, ruleY, 32, 32, 
                    &scoreRuleColor, mFadeProgress );
        
        drawSprite( gridLineTop, headerX + mW/4, ruleY, 32, 32, 
                    &scoreRuleColor, mFadeProgress );


        drawStringBig( "today", center, 
                       headerX,
                       mTodayButtons[0]->mY - 40,
                       &scoreHeaderColor, mFadeProgress );

        for( i=0; i<mTodayScores.size() && i<8; i++ ) {
            ScoreBundle *b = *( mTodayScores.getElement( i ) );
            
            int textX = mTodayButtons[i]->mX - 21 - 10;
            
            drawScore( b->mScore,
                       textX,
                       mTodayButtons[i]->mY - 2,
                       &scoreNumberColor, mFadeProgress );
            
            drawString( b->mName, right,
                        textX,
                        mTodayButtons[i]->mY + 5 + 2,
                        &scoreHeaderColor, mFadeProgress );
            }


        glDisable( GL_BLEND );


        
        if( mVisible && mFadeProgress == 1 ) {
            
            // fully visible
            // hide loading panel
            mLoadingPanel->setVisible( false );
            }
        
        }
    
    }



void HighScorePanel::closePressed() {

    }


        
        
