#pragma once

#include "PluginProcessor.h"
#include "NameLabel.h"
#include "CustomTheme.h"
#include "RotarySlider.h"

//==============================================================================
class SimpleReverbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit SimpleReverbAudioProcessorEditor (SimpleReverbAudioProcessor&);
    ~SimpleReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleReverbAudioProcessor& processorRef;

    NameLabel sizeLabel;
    NameLabel dampLabel;
    NameLabel widthLabel;
    NameLabel dwLabel;

    RotarySlider sizeSlider;
    RotarySlider dampSlider;
    RotarySlider widthSlider;
    RotarySlider dwSlider;

    juce::TextButton freezeButton;
    juce::AudioProcessorValueTreeState::SliderAttachment sizeSliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment dampSliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment widthSliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment dwSliderAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment freezeButtonAttachment;

    CustomTheme customTheme;

    juce::Colour blue = juce::Colour::fromFloatRGBA(0.43f, 0.83f, 1.0f, 1.0f);
    juce::Colour offWhite = juce::Colour::fromFloatRGBA(0.83f, 0.84f, 0.9f, 1.0f);
    juce::Colour grey = juce::Colour::fromFloatRGBA(0.42f, 0.42f, 0.42f, 1.0f);
    juce::Colour black = juce::Colour::fromFloatRGBA(0.08f, 0.08f, 0.08f, 1.0f);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleReverbAudioProcessorEditor)
};
