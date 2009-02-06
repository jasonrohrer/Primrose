#include "minorGems/util/SimpleVector.h"

#include "game.h"


#include <math.h>

#include <stdio.h>


class SoundSample {
    public:
        int mNumSamples;
        
        // samples in [-1, +1]
        float *mLeftChannel;
        float *mRightChannel;
    };





#define numSamplesInBank 7


SoundSample sampleBank[ numSamplesInBank ];


class ActiveSound {
    public:
        
        ActiveSound( int inBankIndex, float inLoudness ) 
                : mBankIndex( inBankIndex ), mSamplesPlayed( 0 ),
                  mLoundness( inLoudness ) {
            }
        

        int mBankIndex;        int mSamplesPlayed;
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



// gets frequency of note in our scale
double getFrequency( double inBaseFrequency, int inScaleNoteNumber ) {
    int octavesUp = inScaleNoteNumber / 7;
    
    int numHalfsteps = halfstepMap[ inScaleNoteNumber % 7 ] + octavesUp * 12;
    
    return inBaseFrequency * pow( twelthRootOfTwo, numHalfsteps );
    }



// some primitive wave functions

// This produces fine results (almost perfect square wave)
//int nLimit = 40;
int nLimit = 80;


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







void initSound() {
    float baseFreq = 60;
    
    for( int i=0; i<numSamplesInBank; i++ ) {
        
        float freq = baseFreq * pow( twelthRootOfTwo, halfstepMap[i] );
        //float freq = baseFreq * pow( twelthRootOfTwo, i );
        

        SoundSample *s = &( sampleBank[i] );
        
        int numSamples = (int)( 0.2 * gameSoundSampleRate );
        s->mNumSamples = numSamples;
    
        float *l = new float[ numSamples ];
        float *r = new float[ numSamples ];
        
        s->mLeftChannel = l;
        s->mRightChannel = r;
    

        float sinFactor = (1.0f / gameSoundSampleRate) * freq * 2 * M_PI;
    
        float envSinFactor = 1.0f / numSamples * M_PI;
        
        for( int i=0; i<numSamples; i++ ) {
            //float value = sawWave( i * sinFactor );
            float value = sin( i * sinFactor );

            // apply another sin as an envelope
            value *= sin( i * envSinFactor );
            
            l[i] = value;
            r[i] = value;
            //printf( "Value = %f\n", value );
            }
        }
    
    }



void freeSound() {
    for( int i=0; i<activeSounds.size(); i++ ) {
        delete *( activeSounds.getElement( i ) );
        }
    }



void playPlacementSound( int inColor ) {
    printf( "Playing sound\n" );
    
    // same as for group size of 1
    float loudness = 0.25;
    
    activeSounds.push_back( new ActiveSound( inColor, loudness ) );
    }



void playClearingSound( int inColor, int inGroupSize, int inChainLength ) {
    

    // asymptotically approaches 1 as inGroupSize grows.
    // very close to 1 when inGroupSize is 49
    float loudness =
        0.75 * (1 - pow(10, (-inGroupSize/20) ) ) + 0.25;

    // nothing, yet
    }
    


// implements getSoundSamples from game.h
void getSoundSamples( Uint8 *inBuffer, int inLengthToFillInBytes ) {
    //printf( "Audio callback\n" );
    
    // 2 16-bit samples per frame
    int numFrames = inLengthToFillInBytes / 4;
    

    float *leftMix = new float[ numFrames ];
    float *rightMix = new float[ numFrames ];
    
    for( int f=0; f<numFrames; f++ ) {
        leftMix[f] = 0;
        rightMix[f] = 0;
        }
    


    int i = 0;
    
    // we may be removing sounds from the buffer as we use them up
    // i is adjusted inside the while loop
    while( i<activeSounds.size() ) {
        
        ActiveSound *a = *( activeSounds.getElement( i ) );

        int samplesToSkip = a->mSamplesPlayed;

        // read-only here, so we can access a copy
        SoundSample s = sampleBank[ a->mBankIndex ];
        

        int mixLength = s.mNumSamples - samplesToSkip;

        if( mixLength > numFrames ) {
            mixLength = numFrames;
            }

        
        float loudness = a->mLoundness;


        for( int j=0; j != mixLength; j++ ) {
            leftMix[j] += loudness * s.mLeftChannel[j + samplesToSkip];
            rightMix[j] += loudness * s.mRightChannel[j + samplesToSkip];
            }
        
        a->mSamplesPlayed += mixLength;
        
        if( a->mSamplesPlayed >= s.mNumSamples ) {
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
    for( i=0; i != numFrames; i++ ) {
        Sint16 intSampleL = (Sint16)( leftMix[i] * 32767 );
        Sint16 intSampleR = (Sint16)( rightMix[i] * 32767 );
        //printf( "Outputting samples %d, %d\n", intSampleL, intSampleR );

        inBuffer[ streamPosition ] = (Uint8)( intSampleL & 0xFF );
        inBuffer[ streamPosition + 1 ] = (Uint8)( ( intSampleL >> 8 ) & 0xFF );
        
        inBuffer[ streamPosition + 2 ] = (Uint8)( intSampleR & 0xFF );
        inBuffer[ streamPosition + 3 ] = (Uint8)( ( intSampleR >> 8 ) & 0xFF );
        
        streamPosition += 4;
        }

    }





