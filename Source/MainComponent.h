/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "matPlotsHeader.h"
#include "PlotComponent.h"
#include "timeParamComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
                                public ButtonListener,
                                public AudioIODeviceCallback
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    void paint (Graphics&);
    void resized();
    
    void audioDeviceIOCallback(const float** inputData,int InputChannels,float** outputData,int OutputChannels,int numSamples);
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped(){};
    
    void buttonClicked (Button* buttonThatWasClicked);

private:
    //==============================================================================
    ScopedPointer<Buffer>                   bufferWaveform;
    float*                                  dataWaveform;
    ScopedPointer<Buffer>                   bufferOctava;
    float*                                  dataOctava;
    OwnedArray<Buffer>                      octavaTimeParametersBuffers;
    
    AudioFormatManager                      audioFormatManager;
    ScopedPointer<AudioFormatReaderSource>  audioFormatReaderSource;
    ScopedPointer<AudioDeviceManager>       audioDeviceManager;
    AudioTransportSource                    audioTransportSource;
    AudioSourcePlayer                       audioSourcePlayer;
    TimeSliceThread                         readAheadThread;
    ScopedPointer<AudioFilePlayer>          audioFilePlayer;
    
    AudioThumbnailCache                     audioThumbnailCache;
    ScopedPointer<ColouredAudioThumbnail>   audioThumbnail;
    ScopedPointer<AudioThumbnailImage>      audioThumbnailImage;
    ScopedPointer<PositionableWaveDisplay>  positionableWaveDisplay;
    TimeSliceThread                         backgroundThread;
    
    ScopedPointer<TabbedComponent>          tabsComponent;
    
    int                                     sampleCounter=0;
    ScopedPointer<TextButton>               startButton;
    ScopedPointer<TextButton>               paintButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
