/*
  ==============================================================================

    PlotComponent.h
    Created: 11 Jul 2015 10:39:09pm
    Author:  Tony Ducks

  ==============================================================================
*/

#ifndef PLOTCOMPONENT_H_INCLUDED
#define PLOTCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "matPlotsHeader.h"

//==============================================================================
/*
*/
class PlotComponent    : public Component/*,
                         public Buffer::Listener*/
{
public:
    PlotComponent();
    ~PlotComponent();

    void paint (Graphics&);
    void resized();
    void refreshPath();
    void refillBuffer();
//    void bufferChanged (Buffer* changedBuffer);

private:
    bool            isInitialized;
    Image           background;
    Buffer          buffer;
    Path            path;
    const int       GAP=100;
    std::vector<String>  yLabels;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};


#endif  // PLOTCOMPONENT_H_INCLUDED
