/*
  ==============================================================================

    timeParamComponent.cpp
    Created: 17 Aug 2015 4:44:36pm
    Author:  Tony Ducks

  ==============================================================================
*/

#include "timeParamComponent.h"

//==============================================================================
timeParamComponent::timeParamComponent(OwnedArray<Buffer>* _octavaTimeParamBuffers){
    octavaTimeParamBuffers = _octavaTimeParamBuffers;
    octaveBandComponent = new OctaveBandPlot(octavaTimeParamBuffers->getFirst(),false);
    addAndMakeVisible(octaveBandComponent);
    for(int i=0;i<4;i++){
        CustomButtonComponent* boton = new CustomButtonComponent(Colour(coloresActivacion[i]));
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
    octaveBandComponent->setBounds(140, 0, getWidth()-140, getHeight()-30);
}

//==============================================================================
void timeParamComponent::buttonClicked(CustomButtonComponent* boton){
    octaveBandComponent->setActive(true);
    int botonIndex = botones.indexOf(boton);
    for(int i=0;i<botones.size();i++){
        if (i==botonIndex) {
            botones[i]->setActive(true);
            octaveBandComponent->setPlotColor(Colour(coloresActivacion[i]));
            octaveBandComponent->changeBuffer(octavaTimeParamBuffers->getUnchecked(i));
            octaveBandComponent->repaint();
        }else{
            botones[i]->setActive(false);
        }
        botones[i]->repaint();
    }
}
