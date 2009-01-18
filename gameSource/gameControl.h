#include "ScoreBundle.h"



// add to global game score
void addToScore( int inPointsToAdd );


// schedules a game reset
void restartGame();


char isGameOver();



// returns actual 8-character array (9 chars, including \0 terminator)
// edit name by changing characters in the array
char *getName();


// forces name to be saved for future sessions
// should be called after editing name
void saveName();


// start playback of a new game
// will be destroyed by game framework
void playbackGame( ScoreBundle *inBundle );