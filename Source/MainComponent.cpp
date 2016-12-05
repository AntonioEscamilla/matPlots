/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


#define N 5                 // N es la la tasa de resampling del AudioOverview
#define M 1                 // M es la proporcion que se toma respecto al AudioBufferLength

//==============================================================================
MainContentComponent::MainContentComponent():readAheadThread("read Ahead thread"){

    // Format manager
    audioFormatManager.registerBasicFormats();
    
    // Device manager
    audioDeviceManager = new AudioDeviceManager();
    audioDeviceManager->initialise(0, 2, 0, true);
    readAheadThread.startThread(3);
    
    // Read file... small impulse response in Documents folder
    File sfile (File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("lyd3_000_ortf_48k.wav"));
    AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(sfile);
    ScopedPointer<AudioSampleBuffer> audioBuffer = new AudioSampleBuffer(1, audioFormatReader->lengthInSamples); //audioBuffer para leer archivo wav
    audioFormatReader->read(audioBuffer, 0, audioFormatReader->lengthInSamples, 0, true, false);
    
    // Cambiar Fs del AudioDevice segun sea la del archivo wav
    juce::AudioDeviceManager::AudioDeviceSetup newAudioSetup;
    audioDeviceManager->getAudioDeviceSetup(newAudioSetup);
    newAudioSetup.sampleRate=audioFormatReader->sampleRate;
    audioDeviceManager->setAudioDeviceSetup(newAudioSetup, true);
    
    //Playback preparation
    audioFormatReaderSource = new AudioFormatReaderSource(audioFormatReader, true);
    audioTransportSource.setSource(audioFormatReaderSource, 32768, &readAheadThread, 0, 2);   //el 0 es para que no haga resamplig....
    audioSourcePlayer.setSource(&audioTransportSource);                                       //..depende de haber cambiado Fs en AudioDevice
    audioDeviceManager->addAudioCallback(&audioSourcePlayer);
    
    Logger::writeToLog ("Total length: --> " + String(audioFormatReader->lengthInSamples));
    
    bufferWaveform = new AudioSampleBuffer(1,audioFormatReader->lengthInSamples/(M*N));
    bufferWaveform->clear();
    audioDownSamplig(audioBuffer,bufferWaveform,N);

    octavaTimeParametersBuffers = new AudioSampleBuffer(4,31);
    for (int i=0; i<octavaTimeParametersBuffers->getNumChannels(); i++) {
        for (int j=0; j<octavaTimeParametersBuffers->getNumSamples(); j++){
            octavaTimeParametersBuffers->setSample(i, j, random());
        }
    }
    
    octavaEnergyParametersBuffers = new AudioSampleBuffer(3,10);
    for (int i=0; i<octavaEnergyParametersBuffers->getNumChannels(); i++) {
        for (int j=0; j<octavaEnergyParametersBuffers->getNumSamples(); j++){
            octavaEnergyParametersBuffers->setSample(i, j, random());
        }
    }
    
    addAndMakeVisible (startButton  = new TextButton ("Start"));
    startButton->addListener (this);
    startButton->setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
    startButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible (paintButton  = new TextButton ("Paint"));
    paintButton->addListener (this);
    paintButton->setColour (TextButton::buttonColourId, Colours::grey);
    paintButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible(tabsComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabsComponent->addTab("Parametros Energeticos", Colour(0xff2f2f2f),new timeParamComponent(octavaEnergyParametersBuffers,botonTextEnergy), true);
    tabsComponent->addTab("Parametros Temporales", Colour(0xff2f2f2f), new timeParamComponent(octavaTimeParametersBuffers,botonTextTime), true);
    tabsComponent->addTab("Respuesta al Impulso", Colour(0xff2f2f2f), new AudioWaveForm(bufferWaveform,true), true);
    tabsComponent->addTab("Respuesta al Impulso", Colour(0xff2f2f2f), new AudioWaveForm(bufferWaveform,true), true);

    setSize (1200, 400);
}

MainContentComponent::~MainContentComponent(){
    
}

void MainContentComponent::paint (Graphics& g){
    
}

void MainContentComponent::resized(){
    const int w = getWidth();
    const int h = getHeight();
    const int gap = 250;
    
    startButton->setBounds(0, 0, 30, 20);
    paintButton->setBounds(40, 0, 30, 20);
    
    tabsComponent->setBounds(gap, 0, w-gap-5, h-5);
}

void MainContentComponent::audioDownSamplig(AudioSampleBuffer* input, AudioSampleBuffer* output,int downSampligFactor){
    
    int sampleCounter=0;
    float* dataWaveform = output->getWritePointer(0);
    ScopedPointer<AudioSampleBuffer>  auxBuf = new AudioSampleBuffer(1, downSampligFactor);
    auxBuf->clear();
    int sampleIndexModulo;
    
    for(int j = 0; j < input->getNumSamples(); j++){
        float sample = input->getSample(0, j);
        sampleIndexModulo = j%downSampligFactor;
        auxBuf->setSample(0, sampleIndexModulo, sample);
        if(sampleIndexModulo==0){
            sampleCounter++;
            if (sampleCounter < output->getNumSamples()) {
                if (sample > 0.0f)
                    dataWaveform[sampleCounter] = auxBuf->findMinMax(0, 0, auxBuf->getNumSamples()).getEnd();
                else
                    dataWaveform[sampleCounter] = auxBuf->findMinMax(0, 0, auxBuf->getNumSamples()).getStart();
            }
        }
    }
}

void MainContentComponent::buttonClicked (Button* buttonThatWasClicked){
    if (buttonThatWasClicked == startButton){
        audioTransportSource.start();
    }else if(buttonThatWasClicked == paintButton){
        
    }
}
