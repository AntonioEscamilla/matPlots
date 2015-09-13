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
    
    // Read file
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
    bufferWaveform = new Buffer(audioFormatReader->lengthInSamples/(M*N));  //buffer para downSamplig con el que se pinta waveForm
    audioDownSamplig(audioBuffer,bufferWaveform,N,M);
        
    for (int i=0;i<4;i++) {
        Buffer* bufferO = new Buffer(31);        //se llenan 4 Buffer con datos aleatorios para simular los valores por 1/3 de octava (31 bandas)
        float* dataO = bufferO->getData();       //de parametros de tiempo. Uno a uno se agregan a un OwnedArray que luego se pasa a la clase...
        for (int i = 0; i < bufferO->getSize(); i++){
            dataO[i] = random();                 //..timeParamComponent que se encarga de manejar la seleccion y graficacion de parametros.
        }
        octavaTimeParametersBuffers.add(bufferO); //octavaTimeParametersBuffers es un OwnedArray y debe ser declarado como una variable
    }                                             //de la clase para que le pertenezca y sea la clase la que lo borre.
    
    for (int i=0;i<3;i++) {
        Buffer* bufferO = new Buffer(10);       //se llenan 3 Buffer con datos aleatorios para simular los valores por octava (10 bandas)
        float* dataO = bufferO->getData();      //de parametros energeticos. Uno a uno se agregan a un OwnedArray que luego se pasa a la clase...
        for (int i = 0; i < bufferO->getSize(); i++){
            dataO[i] = random();                //..timeParamComponent que se encarga de manejar la seleccion y graficacion de parametros.
        }
        octavaEnergyParametersBuffers.add(bufferO); //octavaEnergyParametersBuffers es un OwnedArray y debe ser declarado como una variable
    }                                               //de la clase para que le pertenezca y sea la clase la que lo borre
    
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
