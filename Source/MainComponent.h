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

static const char* botonTextTime[4] = {"EDT[s]","RT20[s]","RT30[s]","RT60[s]"};
static const char* botonTextEnergy[3] = {"Ts[ms]","C80[dB]","D50[-]"};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
                                public ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    void paint (Graphics&);
    void resized();
    
    void buttonClicked (Button* buttonThatWasClicked);
    void audioDownSamplig(AudioSampleBuffer* input, AudioSampleBuffer* output,int downSampligFactor);
    
private:
    //==============================================================================
    ScopedPointer<AudioSampleBuffer>        bufferWaveform;
    ScopedPointer<AudioSampleBuffer>        bufferWaveform2;
    ScopedPointer<AudioSampleBuffer>        octavaTimeParametersBuffers;
    ScopedPointer<AudioSampleBuffer>        octavaEnergyParametersBuffers;
    
    AudioFormatManager                      audioFormatManager;
    ScopedPointer<AudioFormatReaderSource>  audioFormatReaderSource;
    ScopedPointer<AudioDeviceManager>       audioDeviceManager;
    AudioTransportSource                    audioTransportSource;
    AudioSourcePlayer                       audioSourcePlayer;
    TimeSliceThread                         readAheadThread;

    ScopedPointer<TabbedComponent>          tabsComponent;
    ScopedPointer<TextButton>               startButton;
    ScopedPointer<TextButton>               paintButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
