#include "minorGems/util/SimpleVector.h"

#include "game.h"
//#include "Timbre.h"


#include <math.h>

#include <stdio.h>


class SoundSample {
    public:
        SoundSample() 
                : mNumSamples( -1 ), 
                  mLeftChannel( NULL ), mRightChannel( NULL ) {
            }
        
        ~SoundSample() {
            if( mLeftChannel != NULL ) {
                delete [] mLeftChannel;
                }
            if( mRightChannel != NULL ) {
                delete [] mRightChannel;
                }
            }
        
        int mNumSamples;
        
        // samples in [-1, +1]
        float *mLeftChannel;
        float *mRightChannel;
    };




// 7, one for each placement
// +
// 7 for chain steps
#define numSamplesInBank 14


SoundSample sampleBank[ numSamplesInBank ];

// which octaves are on for each of 15 supported chain lengths
char onMap[15][6] = 
{ { 0, 1, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 1, 0, 0 },
  { 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0 },
  { 0, 1, 0, 0, 1, 0 },
  { 0, 1, 1, 0, 1, 0 },
  { 0, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 1 },
  { 0, 1, 0, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 1 },
  { 0, 1, 1, 1, 0, 1 },
  { 0, 1, 1, 1, 1, 1 } };



// indexed as  [chainLength % 5][octave]
/*
float octaveVolMap[5][5] = 
{ { 0.0, 0.6, 1.0, 0.0, 0.3 },
  { 0.3, 0.0, 1.0, 0.6, 0.0 },
  { 0.0, 0.6, 0.0, 0.6, 0.3 },
  { 0.3, 0.0, 1.0, 0.0, 0.3 },
  { 0.3, 0.6, 0.0, 0.6, 0.0 } };
*/
float octaveVolMap[5][5] = 
{ { 3.0, 0.0, 0.0, 1.0, 0.0 },
  { 0.0, 1.0, 0.0, 0.0, 3.0 },
  { 3.0, 0.0, 1.0, 0.0, 0.0 },
  { 0.0, 1.0, 0.0, 1.0, 0.0 },
  { 0.0, 0.0, 1.0, 0.0, 3.0 } };

/*
// 6 octave version
float octaveVolMap[3][5] = 
{ { 0.3, 0.0, 0.0, 1.0, 0.0, 0.0 },
  { 0.0, 0.6, 0.0, 0.0, 0.6, 0.0 },
  { 0.0, 0.0, 1.0, 0.0, 0.0, 0.3 } };

float octaveVolMap[3][5] = 
{ { 0.3, 0.0, 1.0, 0.0, 0.6, 0.0 },
  { 0.0, 0.6, 0.0, 1.0, 0.0, 0.3 },
  { 0.0, 0.6, 1.0, 0.0, 0.0, 0.3 } };
 */


// timbre with 7 notes in it
//Timbre *timbre;

float currentNoteVolumes[ numSamplesInBank ];
float targetNoteVolumes[ numSamplesInBank ];

int currentWaveTablesPos[ numSamplesInBank ];




class ActiveSound {
    public:
        
        ActiveSound( int inBankIndex, float inLoudness ) 
                : mBankIndex( inBankIndex ), mSamplesPlayed( 0 ),
                  mLoundness( inLoudness ) {
            }
        

        int mBankIndex;        
        int mSamplesPlayed;
        float mLoundness;
    };


SimpleVector<ActiveSound*> activeSounds;





double twelthRootOfTwo = pow( 2, 1.0/12 );

// for major scale
// W, W, H, W, W, W, H
//int halfstepMap[ 7 ] = { 0, 2, 4, 5, 7, 9, 11 };

// minor scale
// W,H,W,W,H,W,W
//int halfstepMap[ 7 ] = { 0, 2, 3, 5, 7, 8, 10 };


// our haunting chord
//int halfstepMap[ 7 ] = { 0, 3, 7, 10, 12, 17, 22 };


// packed into one octave (and not in order!
//int halfstepMap[ 7 ] = { 3, 6, 11, 8, 4, 10, 1 };

// new minor scale, not in order
int placementStepMap[ 7 ] = { 0, 3, 7, 10, 8, 5, 2 };


// major scale
int clearingStepMap[ 7 ] = { 0, 2, 4, 5, 7, 9, 11 };




/*
// gets frequency of note in our scale
double getFrequency( double inBaseFrequency, int inScaleNoteNumber ) {
    int octavesUp = inScaleNoteNumber / 7;
    
    int numHalfsteps = halfstepMap[ inScaleNoteNumber % 7 ] + octavesUp * 12;
    
    return inBaseFrequency * pow( twelthRootOfTwo, numHalfsteps );
    }
*/


// some primitive wave functions

// This produces fine results (almost perfect square wave)
//int nLimit = 40;
int nLimit = 20;
//int nLimit = 10;
//int nLimit = 80;
//int nLimit = 5;

// square wave with period of 2pi
double squareWave( double inT ) {
    double sum = 0;
    
    for( int n=1; n<nLimit; n+=2 ) {
        sum += 1.0/n * sin( n * inT );
        }
    return sum;
    }



// sawtoot wave with period of 2pi
double sawWave( double inT ) {
    double sum = 0;
    
    for( int n=1; n<nLimit; n++ ) {
        sum += 1.0/n * sin( n * inT );
        }
    return sum;
    }



void fillTone( SoundSample *inSample, float inFrequency, int inNumSamples,
               int inNumSamplesToSkip = 0, int inTotalSamples = -1 ) {

    SoundSample *s = inSample;
    
    if( inTotalSamples == -1 ) {
        inTotalSamples = inNumSamples + inNumSamplesToSkip;
        }
    

    if( s->mNumSamples == -1 ) {
        
        s->mNumSamples = inTotalSamples;    
        
        s->mLeftChannel = new float[ inTotalSamples ];
        s->mRightChannel = new float[ inTotalSamples ];
        }

    float *l = s->mLeftChannel;
    float *r = s->mRightChannel;
        
    

    float sinFactor = (1.0f / gameSoundSampleRate) * inFrequency * 2 * M_PI;
    
    float envSinFactor = 1.0f / inNumSamples * M_PI;
    
    int limit = inNumSamplesToSkip + inNumSamples;
    
    for( int i=inNumSamplesToSkip; i<limit; i++ ) {
        //float value = sawWave( i * sinFactor );
        float value = squareWave( i * sinFactor );
        //float value = sin( i * sinFactor );
        
        // apply another sin as an envelope
        value *= sin( i * envSinFactor );
        
        l[i] = value;
        r[i] = value;
        //printf( "Value = %f\n", value );
        }
    }






void initSound() {
    float baseFreq = 60;

    int i;
    
    // for each color
    for( i=0; i<7; i++ ) {
        
        // placement sounds
        float freq = baseFreq * pow( twelthRootOfTwo, placementStepMap[i] );
        //float freq = baseFreq * pow( twelthRootOfTwo, i );
        
        //printf( "Filling sound bank %d\n", i );
        
        SoundSample *s = &( sampleBank[i] );
        
        int numSamples = (int)( 0.2 * gameSoundSampleRate );

        fillTone( s, freq, numSamples );
        
        }
    
    // now clearing sounds
    // shepard tones

    
    for( i=0; i<7; i++ ) {
        float freqA = baseFreq * pow( twelthRootOfTwo, clearingStepMap[i] );
        // second, 7 steps up
        float freqB = baseFreq * pow( twelthRootOfTwo, 
                                      clearingStepMap[i] + 7 );
        //freqB = 0;
        

        #define numShepParts 4
        int t;
        
        SoundSample shepardParts[numShepParts];
        int numSamples = (int)( 0.4 * gameSoundSampleRate );

        for( t=0; t<numShepParts; t++ ) {
            // first note in sequence
            fillTone( &shepardParts[t], freqA, 
                      numSamples / 2, 0, numSamples );
            
            // second
            fillTone( &shepardParts[t], freqB, 
                      numSamples/2, numSamples/2, numSamples );
            
            // raise for next shep part
            freqA *= 2;
            freqB *= 2;
            }
        

        float partWeights[4];
        
        // 0.0 .. 0.5
        partWeights[0] = i / 12.0f;

        // 0.5 .. 1.0
        partWeights[1] = 0.5 + partWeights[0];
        
        // 1.0 .. 0.5
        partWeights[2] = 1 - partWeights[0];
        
        // 0.5 .. 0.0
        partWeights[3] = 0.5 - partWeights[0];
        

        // vol sum at any point = 2
        
        // weight so sum = 1
        for( int t=0; t<numShepParts; t++ ) {
            //partWeights[t] *= 0.5;
            printf( "Step %d, part weight %d = %f\n", i, t, partWeights[t] );
            
            }
        
        int index = i+7;
        
        SoundSample *s = &( sampleBank[index] );

        s->mNumSamples = numSamples;

        s->mLeftChannel = new float[ numSamples ];
        s->mRightChannel = new float[ numSamples ];
        

        float *l = s->mLeftChannel;
        float *r = s->mRightChannel;
        

        for( int j=0; j<numSamples; j++ ) {
            l[j] = 0;
            r[j] = 0;

            for( t=0; t<numShepParts; t++ ) {
                l[j] += partWeights[t] * shepardParts[t].mLeftChannel[j];
                r[j] += partWeights[t] * shepardParts[t].mRightChannel[j];
                }
            }
        }
    }



void freeSound() {
    
    for( int i=0; i<activeSounds.size(); i++ ) {
        delete *( activeSounds.getElement( i ) );
        }
    
    //delete timbre;
    }


/*
void setColorVolumes( float inVolumes[7] ) {
    for( int i=0; i<numSamplesInBank; i++ ) {
        targetNoteVolumes[i] = inVolumes[i];
        }

    }
*/


void playPlacementSound( int inColor ) {
    printf( "Playing sound\n" );
    
    // louder than that of clearing group size of 1
    // (clearing sound ends up sounding louder)
    float loudness = 0.25;
    
    activeSounds.push_back( new ActiveSound( inColor, loudness ) );
    }



void playClearingSound( int inColor, int inGroupSize, int inChainLength ) {
    

    // asymptotically approaches 1 as inGroupSize grows.
    // very close to 1 when inGroupSize is 49
    float loudness =
        0.92 * (1 - pow(10, (-inGroupSize/20) ) ) + 0.08;


    inChainLength -= 1;
    
    inChainLength = inChainLength % 7;
    
    /*
    if( inChainLength > 15 ) {
        inChainLength = 15;
        }
    */
    int index = 7 + inChainLength;
    
    printf( "playing sound from bank %d with loudness %f\n", index, loudness );
    
    activeSounds.push_back( new ActiveSound( index, loudness ) );
    }
  


int numTestSoundsPlayed = 0;
int stepsBetweenTestPlays = 0;


// implements getSoundSamples from game.h
void getSoundSamples( Uint8 *inBuffer, int inLengthToFillInBytes ) {
    //printf( "Audio callback\n" );
    
    // 2 16-bit samples per frame
    int numFrames = inLengthToFillInBytes / 4;
    

    float *leftMix = new float[ numFrames ];
    float *rightMix = new float[ numFrames ];
    
    int f;

    for( f=0; f!=numFrames; f++ ) {
        leftMix[f] = 0;
        rightMix[f] = 0;
        }
    /*
    // max piece volume is 1/58, and we want the on/off volume change
    // to happen in 1/5 of a second (so 5 per second)
    float volumeChangeDelta = 5.0f /58.0f;
    
    volumeChangeDelta /= gameSoundSampleRate;
    


    for( int i=0; i<numSamplesInBank; i++ ) {
        Sint16 *waveTable = timbre->mWaveTable[i];
        int waveTableLength = timbre->mWaveTableLengths[i];
    

        int waveTablePos = currentWaveTablesPos[i];
        
        float currVol = currentNoteVolumes[i];
        float volTarget = targetNoteVolumes[i];
        
        float volDelta = - volumeChangeDelta;
        if( currVol < volTarget ) {
            volDelta = - volDelta;
            }
        int stepsUntilVolTargetMet = (int)( (volTarget - currVol) / volDelta );


        for( f=0; f!=numFrames; f++ ) {
            
            float sample = currVol * waveTable[ waveTablePos ]; 
            leftMix[f] += sample;
            rightMix[f] += sample;
            
                

            waveTablePos++;
            if( waveTablePos >= waveTableLength ) {
                waveTablePos = 0;
                }
            
            if( stepsUntilVolTargetMet != 0  ) {
                currVol += volDelta;
                stepsUntilVolTargetMet--;
                }
            }

        if( stepsUntilVolTargetMet == 0 && currVol != volTarget ) {
            // undershot a bit
            currVol = volTarget;
            }
        
        currentNoteVolumes[i] = currVol;
        currentWaveTablesPos[i] = waveTablePos;
        }
    */

      
    int i = 0;
    
    // we may be removing sounds from the buffer as we use them up
    // i is adjusted inside the while loop
    while( i<activeSounds.size() ) {
        
        ActiveSound *a = *( activeSounds.getElement( i ) );

        int samplesToSkip = a->mSamplesPlayed;

        
        SoundSample *s = &( sampleBank[ a->mBankIndex ] );
        

        int mixLength = s->mNumSamples - samplesToSkip;

        if( mixLength > numFrames ) {
            mixLength = numFrames;
            }

        
        float loudness = a->mLoundness;

        float *sampleLeft = s->mLeftChannel;
        float *sampleRight = s->mRightChannel;
        

        for( int j=0; j != mixLength; j++ ) {
            leftMix[j] += loudness * sampleLeft[j + samplesToSkip];
            rightMix[j] += loudness * sampleRight[j + samplesToSkip];
            }
        
        a->mSamplesPlayed += mixLength;
        
        if( a->mSamplesPlayed >= s->mNumSamples ) {
            // sound done playing
        
            delete a;
            activeSounds.deleteElement( i );
        
            // don't increment i
            }
        else {
            // next active sound
            i++;
            }
        }

    if(  activeSounds.size() == 0 && numTestSoundsPlayed < numSamplesInBank ) {
        if( stepsBetweenTestPlays > 10 ) {
            stepsBetweenTestPlays = 0;
            
            // play next test sound
            printf( "Test play of bank sound %d\n", numTestSoundsPlayed );
            
         
            float loudness = 0.25;
            if( numTestSoundsPlayed >=7 ) {
                loudness = 0.08;
                }
            
            activeSounds.push_back( 
                new ActiveSound( numTestSoundsPlayed, loudness ) );
            
            //playClearingSound( 0, 1, numTestSoundsPlayed + 1 );
            
            numTestSoundsPlayed ++;
            }
        else {
            stepsBetweenTestPlays++;
            }
        
        }
    
        

    #define Sint16Max 32767

    // now copy samples into Uint8 buffer (converting them to Sint16s)
    int streamPosition = 0;
    for( f=0; f != numFrames; f++ ) {
        Sint16 intSampleL = (Sint16)( leftMix[f] * Sint16Max );
        Sint16 intSampleR = (Sint16)( rightMix[f] * Sint16Max );
        //printf( "Outputting samples %d, %d\n", intSampleL, intSampleR );

        inBuffer[ streamPosition ] = (Uint8)( intSampleL & 0xFF );
        inBuffer[ streamPosition + 1 ] = (Uint8)( ( intSampleL >> 8 ) & 0xFF );
        
        inBuffer[ streamPosition + 2 ] = (Uint8)( intSampleR & 0xFF );
        inBuffer[ streamPosition + 3 ] = (Uint8)( ( intSampleR >> 8 ) & 0xFF );
        
        streamPosition += 4;
        }

    }





