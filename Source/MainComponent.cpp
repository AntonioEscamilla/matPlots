/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "PlotComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
                        :plotComponent ()
{
    addAndMakeVisible (&plotComponent);
    setSize (1200, 400);
}

MainContentComponent::~MainContentComponent(){
    
}

void MainContentComponent::paint (Graphics& g){
    float gap = 5.0;
    
    GuiHelpers::drawBevel (g, plotComponent.getBounds().toFloat(), gap, Colours::grey);
}

void MainContentComponent::resized(){
    const int w = getWidth();
    const int h = getHeight();
    const int gap = 50;
    
    plotComponent.setBounds (gap,gap,w - 2*gap, h - 2*gap);
}
