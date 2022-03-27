#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

class NameLabel : public juce::Label{
public:
    NameLabel()
    {
        setFont(20.0f);
        setColour(juce::Label::textColourId, grey);
        setJustificationType(juce::Justification::centred);
    }
    ~NameLabel() override= default;

private:
    juce::Colour grey = juce::Colour::fromFloatRGBA(0.42f, 0.42f, 0.42f, 1.0f);
};

