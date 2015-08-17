/*
  ==============================================================================

    timeParamComponent.cpp
    Created: 17 Aug 2015 4:44:36pm
    Author:  Tony Ducks

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeParamComponent.h"

//==============================================================================
timeParamComponent::timeParamComponent(Buffer* _buffer){
    octaveBandComponent = new OctaveBandPlot(_buffer);
    addAndMakeVisible(octaveBandComponent);
}

timeParamComponent::~timeParamComponent(){
}

void timeParamComponent::paint (Graphics& g){
}

void timeParamComponent::resized(){
    octaveBandComponent->setBounds(250, 0, getWidth()-250, getHeight()-50);
}
