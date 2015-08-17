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

//==============================================================================
/*
*/
class timeParamComponent    : public Component
{
public:
    timeParamComponent(Buffer* _buffer);
    ~timeParamComponent();

    void paint (Graphics&);
    void resized();

private:
    ScopedPointer<OctaveBandPlot>           octaveBandComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (timeParamComponent)
};


#endif  // TIMEPARAMCOMPONENT_H_INCLUDED
