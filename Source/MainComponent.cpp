/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "PlotComponent.h"

#define N 2                 // N es la la tasa de resampling del AudioOverview
#define M 3                 // M es la proporcion que se toma respecto al AudioBufferLength

//==============================================================================
MainContentComponent::MainContentComponent():readAheadThread("read Ahead thread"),audioThumbnailCache(10),backgroundThread ("Waveform Thread"){

/******************************************************************************/
/* reproduce archivo de audio pero sin registrar un callback para la clase donde se pueda acceder a las muestras*/
/******************************************************************************/
//        // Format manager
//        audioFormatManager.registerBasicFormats();
//        audioSourcePlayer.setSource(&audioTransportSource);
//        // Device manager
//        audioDeviceManager = new AudioDeviceManager();
//        audioDeviceManager->addAudioCallback(&audioSourcePlayer);
//        audioDeviceManager->initialise(0, 2, 0, true);
//        readAheadThread.startThread(3);
//        // Play file
//        File sfile (File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("03-Bongo_bong.wav"));
//        AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(sfile);
//        audioFormatReaderSource = new AudioFormatReaderSource(audioFormatReader, true);
//        audioTransportSource.setSource(audioFormatReaderSource, 32768, &readAheadThread, audioFormatReader->sampleRate, 2);
//        audioTransportSource.start();
/******************************************************************************/
/* reproduce archivo de audio registrarndo un callback para la clase donde se pueda acceder a las muestras*/
/******************************************************************************/
    // Format manager
    audioFormatManager.registerBasicFormats();
    // Device manager
    audioDeviceManager = new AudioDeviceManager();
    //audioDeviceManager->addAudioCallback(this);
    audioDeviceManager->initialise(0, 1, 0, true);
    readAheadThread.startThread(3);
    // Play file
    audioTransportSource.stop();
    audioTransportSource.setSource (nullptr);
    audioFormatReaderSource = nullptr;
    File sfile (File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("lyd3_000_ortf_48k.wav"));
    AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(sfile);
    audioFormatReaderSource = new AudioFormatReaderSource(audioFormatReader, true);
    audioTransportSource.setSource(audioFormatReaderSource, 32768, &readAheadThread, audioFormatReader->sampleRate, 2);
    //audioTransportSource.start();
/******************************************************************************/
/* version simplificada de dRowAudio para reproducir un archivo de audio*/
/******************************************************************************/
//    // Manager de Formatos de Audio
//    audioFormatManager.registerBasicFormats();
//    
//    // Audio file player ----> Combina la funcionalidad de AudioTransportSource, AudioFormatReader and AudioFormatReaderSource
//    audioFilePlayer = new AudioFilePlayer();
//    audioSourcePlayer.setSource (audioFilePlayer);
//    
//    // Manager del dispositivo de Audio
//    audioDeviceManager = new AudioDeviceManager();
//    audioDeviceManager->addAudioCallback(&audioSourcePlayer);
//    audioDeviceManager->initialise(0, 2, 0, true);
//    
//    // Reproducción de archivo
//    File soundfile (File::getSpecialLocation (File::userDocumentsDirectory).getChildFile("lyd3_000_ortf_48k.wav"));
//    audioFilePlayer->setFile(soundfile.getFullPathName());
//    audioFilePlayer->start();
//
/******************************************************************************/
    
    Logger::writeToLog ("Total length: --> " + String(audioTransportSource.getTotalLength()));
    
    bufferWaveform = new Buffer(audioTransportSource.getTotalLength()/(M*N));
    dataWaveform = bufferWaveform->getData();
    
    bufferOctava = new Buffer(10);
    dataOctava = bufferOctava->getData();
    for (int i = 0; i < bufferOctava->getSize(); i++){
        dataOctava[i] = random();
    }
 
/******************************************************************************/
/* funcion de dRowAudio para para mostrar forma de onda usando Thumbnail y otro Thread*/
/******************************************************************************/
//    // Mostrar foma de onda
//    audioThumbnail = new ColouredAudioThumbnail(50, *audioFilePlayer->getAudioFormatManager(), audioThumbnailCache);
//    audioThumbnailImage = new AudioThumbnailImage (*audioFilePlayer, backgroundThread,*audioThumbnail, 50); //sourceSamplesPerThumbnailSample
//    audioThumbnailImage->setResolution(1.0f);
//    positionableWaveDisplay = new PositionableWaveDisplay (*audioThumbnailImage, backgroundThread);
//    positionableWaveDisplay->setCursorDisplayed(false);
//    addAndMakeVisible (positionableWaveDisplay);
//    backgroundThread.startThread (1);
/******************************************************************************/
    
    addAndMakeVisible (startButton  = new TextButton ("Start"));
    startButton->addListener (this);
    startButton->setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
    startButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible (paintButton  = new TextButton ("Paint"));
    paintButton->addListener (this);
    paintButton->setColour (TextButton::buttonColourId, Colours::grey);
    paintButton->setColour (TextButton::textColourOnId, Colours::black);
    
    addAndMakeVisible(tabsComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop));
    tabsComponent->addTab("Parametros Energeticos", Colour(0xff2f2f2f), new OctaveBandPlot(bufferOctava), true);
    tabsComponent->addTab("Parametros Temporales", Colour(0xff2f2f2f), new timeParamComponent(bufferOctava), true);
    
    setSize (1200, 400);
}

MainContentComponent::~MainContentComponent(){

}

void MainContentComponent::paint (Graphics& g){
    float gap = 5.0;
    
    GuiHelpers::drawBevel (g, tabsComponent->getBounds().toFloat(), gap, Colour(0xff3f3f3f));
}

void MainContentComponent::resized(){
    const int w = getWidth();
    const int h = getHeight();
    const int gap = 250;
    
    startButton->setBounds(0, 0, 30, 20);
    paintButton->setBounds(40, 0, 30, 20);
    
    tabsComponent->setBounds(gap, 0, w-gap-5, h-5);
}

void MainContentComponent::audioDeviceIOCallback(const float** inputData,int InputChannels,float** outputData,int OutputChannels,int numSamples){

    AudioSampleBuffer buffer(OutputChannels, numSamples);
    AudioSourceChannelInfo info;
    info.buffer = &buffer;
    info.numSamples = numSamples;
    info.startSample = 0;
    
    audioTransportSource.getNextAudioBlock(info);
    
    ScopedPointer<Buffer>  auxBuf = new Buffer(N);
    float* auxBufData = auxBuf->getData();
    int sampleIndexModulo;
    int maxLoc;
    float maxVal;
    
    for(int i = 0; i < OutputChannels; i++){
        for(int j = 0; j < numSamples; j++){
            float sample =  *buffer.getReadPointer(i, j);
            outputData[i][j] =sample;
            sampleIndexModulo = j%N;
            auxBufData[sampleIndexModulo] = sample;
            if(sampleIndexModulo==0){
                sampleCounter++;
                if (sampleCounter < bufferWaveform->getSize()) {
                    if (sample > 0.0f) findMax(auxBufData, auxBuf->getSize(), maxLoc, maxVal);
                    else findMin(auxBufData, auxBuf->getSize(), maxLoc, maxVal);
                    dataWaveform[sampleCounter] = maxVal;
                }
            }
        }
    }
    
}

void MainContentComponent::audioDeviceAboutToStart (AudioIODevice* device){
    audioTransportSource.prepareToPlay (device->getCurrentBufferSizeSamples(),device->getCurrentSampleRate());
}

void MainContentComponent::buttonClicked (Button* buttonThatWasClicked){
    if (buttonThatWasClicked == startButton){
        audioDeviceManager->addAudioCallback(this);
        audioTransportSource.start();
    }else if(buttonThatWasClicked == paintButton){
        tabsComponent->addTab("Respuesta al Impulso", Colour(0xff2f2f2f), new AudioWaveForm(bufferWaveform), true);
    }
}
