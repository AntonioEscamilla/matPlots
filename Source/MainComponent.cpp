/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


#define N 50                 // N es la la tasa de resampling del AudioOverview
#define M 1                 // M es la proporcion que se toma respecto al AudioBufferLength

//==============================================================================
MainContentComponent::MainContentComponent():readAheadThread("read Ahead thread"){

    // Format manager
    audioFormatManager.registerBasicFormats();
    audioSourcePlayer.setSource(&audioTransportSource);
    // Device manager
    audioDeviceManager = new AudioDeviceManager();
    audioDeviceManager->addAudioCallback(&audioSourcePlayer);
    audioDeviceManager->initialise(0, 2, 0, true);
    readAheadThread.startThread(3);
    // Read file & Playback preparation
    File sfile (File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("lyd3_000_ortf_48k.wav"));
    AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(sfile);
    audioFormatReaderSource = new AudioFormatReaderSource(audioFormatReader, true);
    audioTransportSource.setSource(audioFormatReaderSource, 32768, &readAheadThread, audioFormatReader->sampleRate, 2);
    
    Logger::writeToLog ("Total length: --> " + String(audioFormatReader->lengthInSamples));
    
    ScopedPointer<AudioSampleBuffer> audioBuffer = new AudioSampleBuffer(1, audioFormatReader->lengthInSamples); //audioBuffer para leer archivo wav
    bufferWaveform = new Buffer(audioFormatReader->lengthInSamples/(M*N));  //buffer para downSamplig con el que se pinta waveForm
    audioFormatReader->read(audioBuffer, 0, audioFormatReader->lengthInSamples, 0, true, false);
    audioDownSamplig(audioBuffer,bufferWaveform,N,M);
        
    for (int i=0;i<4;i++) {
        Buffer* bufferO = new Buffer(31);
        float* dataO = bufferO->getData();
        for (int i = 0; i < bufferO->getSize(); i++){
            dataO[i] = random();
        }
        octavaTimeParametersBuffers.add(bufferO);
    }
    
    for (int i=0;i<3;i++) {
        Buffer* bufferO = new Buffer(10);
        float* dataO = bufferO->getData();
        for (int i = 0; i < bufferO->getSize(); i++){
            dataO[i] = random();
        }
        octavaEnergyParametersBuffers.add(bufferO);
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
    //se pasa el puntero del OwnedArray "&octavaTimeParametersBuffers", donde estan los buffers con los parametros de tiempo
    tabsComponent->addTab("Parametros Energeticos", Colour(0xff2f2f2f),new timeParamComponent(&octavaEnergyParametersBuffers,botonTextEnergy), true);
    tabsComponent->addTab("Parametros Temporales", Colour(0xff2f2f2f), new timeParamComponent(&octavaTimeParametersBuffers,botonTextTime), true);
    tabsComponent->addTab("Respuesta al Impulso", Colour(0xff2f2f2f), new AudioWaveForm(bufferWaveform,true), true);
    
    setSize (1200, 400);
}

MainContentComponent::~MainContentComponent(){
    
}

void MainContentComponent::paint (Graphics& g){
    float gap = 5.0;
    
    GuiHelpers::drawBevel (g, tabsComponent->getBounds().toFloat(), gap, Colours::darkgrey);
}

void MainContentComponent::resized(){
    const int w = getWidth();
    const int h = getHeight();
    const int gap = 250;
    
    startButton->setBounds(0, 0, 30, 20);
    paintButton->setBounds(40, 0, 30, 20);
    
    tabsComponent->setBounds(gap, 0, w-gap-5, h-5);
}

void MainContentComponent::audioDownSamplig(AudioSampleBuffer* input, Buffer* output,int downSampligFactor, int audioFileProportion){
    
    float* dataWaveform = output->getData();
    ScopedPointer<Buffer>  auxBuf = new Buffer(N);
    float* auxBufData = auxBuf->getData();
    int sampleIndexModulo;
    int maxLoc;
    float maxVal;
    
    for(int j = 0; j < input->getNumSamples(); j++){
        float sample = *input->getReadPointer(0, j);
        sampleIndexModulo = j%N;
        auxBufData[sampleIndexModulo] = sample;
        if(sampleIndexModulo==0){
            sampleCounter++;
            if (sampleCounter < output->getSize()) {
                if (sample > 0.0f) findMax(auxBufData, auxBuf->getSize(), maxLoc, maxVal);
                else findMin(auxBufData, auxBuf->getSize(), maxLoc, maxVal);
                dataWaveform[sampleCounter] = maxVal;
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
