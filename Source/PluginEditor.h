#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BluntRationAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit BluntRationAudioProcessorEditor(BluntRationAudioProcessor&);
    ~BluntRationAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    BluntRationAudioProcessor& processor;

    juce::Label titleLabel;
    juce::Slider gainSlider;
    juce::Label gainLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluntRationAudioProcessorEditor)
};
