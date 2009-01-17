#include "HighScorePanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"


#include <GL/gl.h>



HighScorePanel::HighScorePanel( int inW, int inH, Panel *inLoadingPanel )
        : Panel( inW, inH ),
          mLoadingPanel( inLoadingPanel ) {
          
        
    
    }


    
HighScorePanel::~HighScorePanel() {
    
    
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



char HighScorePanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {
        
        }
    
    return false;
    }




        
void HighScorePanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        
        if( mVisible && mFadeProgress == 1 ) {
            
            // fully visible
            // hide loading panel
            mLoadingPanel->setVisible( false );
            }
        
        }
    
    }



void HighScorePanel::closePressed() {

    }


        
        
