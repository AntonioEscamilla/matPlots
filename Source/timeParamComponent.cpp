/*
  ==============================================================================

    timeParamComponent.cpp
    Created: 17 Aug 2015 4:44:36pm
    Author:  Tony Ducks

  ==============================================================================
*/

#include "timeParamComponent.h"

//==============================================================================
timeParamComponent::timeParamComponent(AudioSampleBuffer* _parametersBufferArray,const char* botonTexto[]){
    parametersBufferArray = _parametersBufferArray;
    if(parametersBufferArray->getNumSamples()==10){
        octaveResolution = true;
        octaveBandComponent = new OctaveBandPlot(parametersBufferArray,false);
        addAndMakeVisible(octaveBandComponent);
    }else{
        octaveResolution = false;
        thirdBandComponent = new ThirdBandPlot(parametersBufferArray,false);
        addAndMakeVisible(thirdBandComponent);
    }
    
    for(int i=0;i<parametersBufferArray->getNumChannels();i++){
        CustomButtonComponent* boton = new CustomButtonComponent(Colour(coloresActivacion[i]),botonTexto[i]);
        boton->addListener(this);
        botones.add(boton);
        addAndMakeVisible(boton);
    }
}

//==============================================================================
timeParamComponent::~timeParamComponent(){
}

//==============================================================================
void timeParamComponent::paint (Graphics& g){
}

//==============================================================================
void timeParamComponent::resized(){
    for(int i=0;i<botones.size();i++){
        botones[i]->setBounds(0,33*i, 120, 30);
    }
    if(octaveResolution){
        octaveBandComponent->setBounds(140, 0, getWidth()-140, getHeight()-30);
    }else{
        thirdBandComponent->setBounds(140, 0, getWidth()-140, getHeight()-30);
    }
    
}

//==============================================================================
void timeParamComponent::buttonClicked(CustomButtonComponent* boton){
    
    int botonIndex = botones.indexOf(boton);
    for(int i=0;i<botones.size();i++){
        if (i==botonIndex) {
            botones[i]->setActive(true);
            if(octaveResolution){
                octaveBandComponent->setActive(true);
                octaveBandComponent->setPlotColor(Colour(coloresActivacion[i]));
                octaveBandComponent->changeActiveChannel(i);
                octaveBandComponent->setYlabelOffset(i);
                octaveBandComponent->repaint();
            }else{
                thirdBandComponent->setActive(true);
                thirdBandComponent->setPlotColor(Colour(coloresActivacion[i]));
                thirdBandComponent->changeActiveChannel(i);
                thirdBandComponent->setYlabelOffset(i);
                thirdBandComponent->repaint();
            }
            
        }else{
            botones[i]->setActive(false);
        }
        botones[i]->repaint();
    }
}
