/*
  ==============================================================================

    timeParamComponent.h
    Created: 17 Aug 2015 4:44:36pm
    Author:  Tony Ducks

  ==============================================================================
*/

#ifndef TIMEPARAMCOMPONENT_H_INCLUDED
#define TIMEPARAMCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "matPlotsHeader.h"
#include "PlotComponent.h"
#include "CustomButtonComponent.h"

//==============================================================================
/*
*/

static const uint32 coloresActivacion[]={0xffaf40ff,0xffff7840,0xffffff40,0xff40ffff};

class timeParamComponent    : public Component,
                              public CustomButtonComponent::Listener
{
public:
    timeParamComponent(OwnedArray<Buffer>* );//constructor de la clase apartir de puntero a un "OwnedArray<Buffer>" donde estan parametros tiempo
    ~timeParamComponent();

    void paint (Graphics&);
    void resized();
    
    void buttonClicked (CustomButtonComponent*);

private:
    ScopedPointer<OctaveBandPlot>           octaveBandComponent;
    OwnedArray<CustomButtonComponent>       botones;
    OwnedArray<Buffer>*                     octavaTimeParamBuffers;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (timeParamComponent)
};


#endif  // TIMEPARAMCOMPONENT_H_INCLUDED
