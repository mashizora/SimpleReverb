//
// Created by UMA on 2022/03/26.
//

#include "RotarySlider.h"

RotarySlider::RotarySlider() {
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    setLookAndFeel(&customTheme);
    setColour(juce::Slider::rotarySliderFillColourId, blue);
    setColour(juce::Slider::textBoxTextColourId, blackGrey);
    setColour(juce::Slider::textBoxOutlineColourId, grey);
    setVelocityBasedMode(true);
    setVelocityModeParameters(0.5, 1, 0.09, false);
    setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                        juce::MathConstants<float>::pi * 2.75f,
                        true);
    setWantsKeyboardFocus(true);
    setTextValueSuffix(" %");
}

RotarySlider::~RotarySlider() {
    setLookAndFeel(nullptr);
}

void RotarySlider::paint(juce::Graphics &g) {
    juce::Slider::paint(g);
    if (hasKeyboardFocus(false)){
        auto length = (getHeight() > 15) ? 5.0f : 4.0f;
        auto thick = (getHeight() > 15) ? 3.0f : 2.5f;
        auto fHeight = static_cast<float>(getHeight());
        auto fWidth = static_cast<float>(getWidth());
        g.setColour(findColour(juce::Slider::textBoxOutlineColourId));
        g.drawLine(0,      0,       0,               length,           thick);
        g.drawLine(0,      0,       length,          0,                thick);
        g.drawLine(0,      fHeight, 0,               fHeight - length, thick);
        g.drawLine(0,      fHeight, length,          fHeight,          thick);
        g.drawLine(fWidth, fHeight, fWidth - length, fHeight,          thick);
        g.drawLine(fWidth, fHeight, fWidth,          fHeight - length, thick);
        g.drawLine(fWidth, 0,       fWidth - length, 0,                thick);
        g.drawLine(fWidth, 0,       fWidth,          length,           thick);
    }
}

void RotarySlider::mouseDown(const juce::MouseEvent &event) {
    juce::Slider::mouseDown(event);
    setMouseCursor(juce::MouseCursor::NoCursor);
}

void RotarySlider::mouseUp(const juce::MouseEvent &event) {
    juce::Slider::mouseUp(event);
    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition(event.source.getLastMouseDownPosition());
    setMouseCursor(juce::MouseCursor::NormalCursor);
}
