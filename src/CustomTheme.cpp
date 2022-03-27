//
// Created by UMA on 2022/03/26.
//

#include "CustomTheme.h"

juce::Slider::SliderLayout CustomTheme::getSliderLayout(juce::Slider &slider) {
    auto localBounds = slider.getLocalBounds();
    juce::Slider::SliderLayout layout;
    layout.textBoxBounds = localBounds;
    layout.sliderBounds = localBounds;

    return layout;
}

void CustomTheme::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                                   float rotaryStartAngle, float rotaryEndAngle, juce::Slider & slider) {
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<float> (static_cast<float>(x),
                                                          static_cast<float>(y),
                                                          static_cast<float>(width),
                                                          static_cast<float>(height)).reduced(2.0f);

    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.085f;
    auto arcRadius = radius - lineW * 1.5f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);

    g.setColour(blackGrey);
    g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(),
                           bounds.getCentreY(),
                           arcRadius,
                           arcRadius,
                           0.0f,
                           rotaryStartAngle,
                           toAngle,
                           true);

    auto alpha = 0.1f + static_cast<float>(slider.getValue()) * 0.9f;
    auto brightness = 0.4f + static_cast<float>(slider.getValue()) * 0.6f;

    g.setColour(fill.withAlpha(alpha).brighter(brightness));
    g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto thumbWidth = lineW * 2.0f;
    juce::Path thumb;
    thumb.addRectangle(-thumbWidth / 2, -thumbWidth / 2, thumbWidth, radius +lineW);
    g.setColour(offWhite);
    g.fillPath(thumb, juce::AffineTransform::rotation(toAngle + 3.12f).translated(bounds.getCentre()));
    g.fillEllipse(bounds.reduced(8.0f));
}

juce::Label *CustomTheme::createSliderTextBox(juce::Slider &slider) {
    auto label = new juce::Label;
    label->setJustificationType(juce::Justification::centred);
    label->setColour(juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    label->setColour(juce::Label::textWhenEditingColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    label->setColour(juce::Label::outlineWhenEditingColourId, slider.findColour(juce::Slider::textBoxOutlineColourId));
    label->setInterceptsMouseClicks(false, false);
    label->setFont(15);
    return label;
}

juce::Font CustomTheme::getTextButtonFont(juce::TextButton &, int buttonHeight) {
    return juce::Font { "Avenir Next Medium", 90.0f, 0 };
}

void CustomTheme::drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
}
