/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "PlotComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent(){
    datosGrafica = new Buffer(300);
    float* bufferData = datosGrafica->getData();
    for (int i = 0; i < datosGrafica->getSize(); i++){
        float x = jlimit (0.0f, 1.0f,  i/(float)datosGrafica->getSize());
        //bufferData[i] = 0.5*sin(20*float_Pi*x)+  0.7*cos(10*float_Pi*x)+0.2;
        bufferData[i] = sinc(20*float_Pi*x);
    }
    plotComponent = new PlotComponent(datosGrafica);
    addAndMakeVisible (plotComponent);
    setSize (1200, 400);
}

MainContentComponent::~MainContentComponent(){
    
}

void MainContentComponent::paint (Graphics& g){
    float gap = 5.0;
    
    GuiHelpers::drawBevel (g, plotComponent->getBounds().toFloat(), gap, Colours::grey);
}

void MainContentComponent::resized(){
    const int w = getWidth();
    const int h = getHeight();
    const int gap = 50;
    
    plotComponent->setBounds (gap,gap,w - 2*gap, h - 2*gap);
}
