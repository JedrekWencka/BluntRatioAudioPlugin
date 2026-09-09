#pragma once

#include <JuceHeader.h>
#include "DSP/TimeDelay.h"

class BluntRatioAudioProcessor final : public juce::AudioProcessor
{
public:
    BluntRatioAudioProcessor();
    ~BluntRatioAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return TimeDelay::maxDelayMilliseconds * 0.001; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    static constexpr const char* delayParameterId = "delay";

private:
    juce::AudioProcessorValueTreeState parameters;
    TimeDelay delay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluntRatioAudioProcessor)
};
