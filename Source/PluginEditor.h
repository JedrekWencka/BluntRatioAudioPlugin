#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DelaySliderLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    void setImages(juce::Image track, juce::Image fill, juce::Image thumb);

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override;

    int getSliderThumbRadius(juce::Slider&) override { return 32; }

    void advanceSmoke();

private:
    void drawSmoke(juce::Graphics&, juce::Point<float> origin) const;

    juce::Image trackImage;
    juce::Image fillImage;
    juce::Image thumbImage;
    float smokePhase = 0.0f;
};

class InvisibleHitArea final : public juce::Component
{
public:
    std::function<void()> onClick;

    void mouseEnter(const juce::MouseEvent&) override { setMouseCursor(juce::MouseCursor::PointingHandCursor); }
    void mouseUp(const juce::MouseEvent& event) override
    {
        if (onClick != nullptr && event.mouseWasClicked())
            onClick();
    }

    void paint(juce::Graphics&) override {}
};

class AboutOverlay final : public juce::Component
{
public:
    AboutOverlay();

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseUp(const juce::MouseEvent&) override;

private:
    juce::Rectangle<int> cardBounds;
    juce::TextButton closeButton { "OK" };
};

class BluntRatioAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                             private juce::Timer
{
public:
    explicit BluntRatioAudioProcessorEditor(BluntRatioAudioProcessor&);
    ~BluntRatioAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void showAbout();

    BluntRatioAudioProcessor& audioProcessor;
    DelaySliderLookAndFeel sliderLookAndFeel;

    juce::Image background;
    juce::Slider delaySlider;
    InvisibleHitArea infoHit;
    AboutOverlay aboutOverlay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluntRatioAudioProcessorEditor)
};
