/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlotComponent.h"


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
    ScopedPointer<AudioWaveForm>            audioWave;
    ScopedPointer<Buffer>                   bufferWaveform;
    float*                                  dataWaveform;
    ScopedPointer<OctaveBandPlot>           octave;
    ScopedPointer<Buffer>                   bufferOctava;
    float*                                  dataOctava;
    
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
    
    TabbedComponent*                        tabsComponent;
    
    int                                     sampleCounter=0;
    bool                                    paintPlot=false;
    TextButton* startButton;
    TextButton* paintButton;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
