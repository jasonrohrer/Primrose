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
// 15 for each color clearing (for chain lengths up to 15)
// = 112 
#define numSamplesInBank 112


SoundSample sampleBank[ numSamplesInBank ];

// which octaves are on for each of 15 supported chain lengths
char onMap[15][6] = 
{ { 1, 1, 0, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 0 },
  { 1, 1, 1, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0 },
  { 1, 1, 0, 1, 0, 0 },
  { 1, 1, 1, 1, 0, 0 },
  { 1, 0, 0, 0, 1, 0 },
  { 1, 1, 0, 0, 1, 0 },
  { 1, 1, 1, 0, 1, 0 },
  { 1, 1, 1, 1, 1, 0 },
  { 1, 0, 0, 0, 0, 1 },
  { 1, 1, 0, 0, 0, 1 },
  { 1, 1, 1, 0, 0, 1 },
  { 1, 1, 1, 1, 0, 1 },
  { 1, 1, 1, 1, 1, 1 } };



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
int halfstepMap[ 7 ] = { 0, 3, 7, 10, 12, 17, 22 };


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



void fillTone( SoundSample *inSample, float inFrequency, int inNumSamples ) {
    SoundSample *s = inSample;
    
    s->mNumSamples = inNumSamples;
    
    float *l = new float[ inNumSamples ];
    float *r = new float[ inNumSamples ];
        
    s->mLeftChannel = l;
    s->mRightChannel = r;
    

    float sinFactor = (1.0f / gameSoundSampleRate) * inFrequency * 2 * M_PI;
    
    float envSinFactor = 1.0f / inNumSamples * M_PI;
        
    for( int i=0; i<inNumSamples; i++ ) {
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
        float freq = baseFreq * pow( twelthRootOfTwo, halfstepMap[i] );
        //float freq = baseFreq * pow( twelthRootOfTwo, i );
        
        //printf( "Filling sound bank %d\n", i );
        
        SoundSample *s = &( sampleBank[i] );
        
        int numSamples = (int)( 0.2 * gameSoundSampleRate );

        fillTone( s, freq, numSamples );
        
        
        
        int o;
        
        // construct octaves for clearing sounds
        SoundSample octaves[6];
        for( o=0; o<6; o++ ) {
            SoundSample *s = &( octaves[o] );
        
            float octFreq = freq * pow( 2, o );
            int numSamples = (int)( 0.4 * gameSoundSampleRate );
            
            fillTone( s, octFreq, numSamples );
            }
        

        // now clearing sounds
        int baseClearIndex = 7 + i * 15;
        

        for( int j=0; j<15; j++ ) {
            int bankIndex = baseClearIndex + j;
            
            //printf( "Filling sound bank %d\n", bankIndex );

            SoundSample *s = &( sampleBank[ bankIndex ] );
            
            int numSamples = (int)( 0.4 * gameSoundSampleRate );
            
            s->mNumSamples = numSamples;
            
            float *l = new float[ numSamples ];
            float *r = new float[ numSamples ];
            
            s->mLeftChannel = l;
            s->mRightChannel = r;            


            char *thisOnMap = onMap[j];
            
            float loudness = 1.0f / 6;
            

            for( int k=0; k<numSamples; k++ ) {
                l[k] = 0;
                r[k] = 0;
                

                
                for( o=0; o<6; o++ ) {
                    SoundSample *sO = &( octaves[o] );

                    if( thisOnMap[o] ) {
                        l[k] += loudness * sO->mLeftChannel[k];
                        r[k] += loudness * sO->mRightChannel[k];
                        }
                    }
                }
            
            }
        
        }
    

    // now clearing sounds
    
    
    /*
    timbre = new Timbre( gameSoundSampleRate,
                         1.0,
                         baseFreq,
                         7,
                         &squareWave );
    
    for( int i=0; i<numSamplesInBank; i++ ) {
        currentNoteVolumes[i] = 0;
        targetNoteVolumes[i] = 0;

        currentWaveTablesPos[i] = 0;
        }
    */
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
    
    // same as for group size of 1
    float loudness = 0.25 / 6;
    
    activeSounds.push_back( new ActiveSound( inColor, loudness ) );
    }



void playClearingSound( int inColor, int inGroupSize, int inChainLength ) {
    

    // asymptotically approaches 1 as inGroupSize grows.
    // very close to 1 when inGroupSize is 49
    float loudness =
        0.75 * (1 - pow(10, (-inGroupSize/20) ) ) + 0.25;

    // nothing, yet
    if( inChainLength > 15 ) {
        inChainLength = 15;
        }
    
    int index = 7 + inColor * 15 + (inChainLength - 1 );
    
    printf( "playing sound from bank %d with loudness %f\n", index, loudness );
    
    activeSounds.push_back( new ActiveSound( index, loudness ) );
    }
  


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





