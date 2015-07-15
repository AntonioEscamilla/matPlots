/*
  ==============================================================================

    PlotComponent.cpp
    Created: 11 Jul 2015 10:39:09pm
    Author:  Tony Ducks

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlotComponent.h"

/*************************************************************************/
PlotComponent::PlotComponent(Buffer* buffer_) :isInitialized (false){
    buffer=buffer_;
    float* bufferData = buffer->getData();
    const int bufferSize = buffer->getSize();
    float outMax;
    float outMin;
    
    myNormalise(bufferData, bufferSize,outMin,outMax);
    float yScale = (outMax - outMin)/5.0f;
    for (int i = 0; i < 6; i++){
        yLabels.push_back(String(outMax - (i * yScale),2));
    }
    //refillBuffer();
}

/*************************************************************************/
PlotComponent::~PlotComponent(){
//    buffer.removeListener(this);
}

/*************************************************************************/
void PlotComponent::paint (Graphics& g){
    const int w = getWidth()-GAP;
    const int h = getHeight()-GAP;
    
    g.fillAll(Colours::black);
    if ( w > 0 && h > 0){
        g.setOrigin(GAP/2, GAP/2);              //translacion de ejes segun la mitad del GAP
        g.drawImageAt (background, 0, 0);       //imagen de fondo con la cuadricula
        g.setColour (Colours::white);
        g.strokePath (path, PathStrokeType (2.0f));     //curva
        
        float yScale = h / 5.0f;
        g.setColour (Colour::greyLevel (0.5f));
        for (int i = 0; i < 6; i++){
            g.drawText(yLabels[i], 0 - GAP/2.0, (int) (i * yScale - GAP/4.0),GAP/2.0,GAP/2.0, Justification::horizontallyCentred);
        }
    }
}

/*************************************************************************/
void PlotComponent::resized(){
    const int w = getWidth()-GAP;
    const int h = getHeight()-GAP;
    
    if (! isInitialized && w > 0 && h > 0){
        //resetPoints();
        isInitialized = true;
    }
    
    background = Image (Image::RGB, jmax (1, w), jmax (1, h), false);
    Graphics g (background);
    g.fillAll (Colours::black);
    g.setColour (Colour::greyLevel (0.25f));
    g.drawRect(0, 0, w, h);
    float xScale = w / 10.0f;
    float yScale = h / 10.0f;
    for (int i = 1; i < 10; i++){
        g.drawHorizontalLine ((int) (i * yScale), 0.0f, (float) w);
        g.drawVerticalLine ((int) (i * xScale), 0.0f, (float) h);
    }
    refreshPath();
}

/*************************************************************************/
void PlotComponent::refreshPath(){
    float* bufferData = buffer->getData();
    const int bufferSize = buffer->getSize();
    const int w = getWidth()-GAP;
    const int h = getHeight()-GAP;
    
    const float xScale = (float) w / (float) bufferSize;
    const float yScale = (float) h ;
    
    path.clear();
    for (int i = 0; i < bufferSize; i++){
        if (i==0) {
            path.startNewSubPath (i * xScale , h  - (bufferData[i] * yScale));
        }else{
            path.lineTo (i * xScale , h  - (bufferData[i] * yScale));
        }
    }
    repaint();
}

/*************************************************************************/
void PlotComponent::refillBuffer (){
    float* bufferData = buffer->getData();
    const int bufferSize = buffer->getSize();
    const float bufferScale = 1.0f / (float) bufferSize;
    
    for (int i = 0; i < bufferSize; i++){
        float x = jlimit (0.0f, 1.0f, i * bufferScale);
        bufferData[i] = 0.5*sin(20*float_Pi*x)+  0.7*cos(10*float_Pi*x)+0.2;
    }
    float outMax;
    float outMin;
    myNormalise(bufferData, bufferSize,outMin,outMax);
    float yScale = (outMax - outMin)/5.0f;
    for (int i = 0; i < 6; i++){
        yLabels.push_back(String(outMax - (i * yScale),2));
    }
    //buffer.updateListeners();
}

///*************************************************************************/
//void PlotComponent::bufferChanged (Buffer* changedBuffer){
//    if (changedBuffer == &buffer){
//        refreshPath();
//    }
//}
