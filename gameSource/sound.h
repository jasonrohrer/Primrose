

// inits sound, constructing samples, etc.
void initSound();


// frees resources
void freeSound();


//void setColorVolumes( float inVolumes[7] );


void playPlacementSound( int inColor, 
                         float inLeftLoudness, float inRightLoudness );


void playClearingSound( int inColor, int inGroupSize, int inChainLength );



// implements getSoundSamples from game.h




