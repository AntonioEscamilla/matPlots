/*
  ==============================================================================

    CustomButtonComponent.h
    Created: 17 Aug 2015 6:04:31pm
    Author:  Tony Ducks

  ==============================================================================
*/

#ifndef CUSTOMBUTTONCOMPONENT_H_INCLUDED
#define CUSTOMBUTTONCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class CustomButtonComponent    : public Component
{
public:
    CustomButtonComponent();
    ~CustomButtonComponent();

    void paint (Graphics&);
    void resized();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomButtonComponent)
};


#endif  // CUSTOMBUTTONCOMPONENT_H_INCLUDED
