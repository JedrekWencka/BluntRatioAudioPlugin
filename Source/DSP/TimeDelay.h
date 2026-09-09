#pragma once

#include <juce_dsp/juce_dsp.h>

/** Simple delay of the whole signal. Does not report host latency, so other tracks stay ahead. */
class TimeDelay
{
public:
    static constexpr float maxDelayMilliseconds = 250.0f;
    static constexpr float delayStepMilliseconds = 0.1f;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setDelayMilliseconds(float delayMs) noexcept;
    void process(juce::AudioBuffer<float>& buffer) noexcept;

    float getMaxDelayMilliseconds() const noexcept { return maxDelayMilliseconds; }

private:
    juce::AudioBuffer<float> ring;
    int writePos = 0;
    double sampleRate = 44100.0;
    float maxDelaySamples = 0.0f;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> delaySamples { 0.0f };
};
