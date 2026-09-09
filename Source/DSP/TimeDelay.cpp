#include "TimeDelay.h"

#include <cmath>

void TimeDelay::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate > 0.0 ? spec.sampleRate : 44100.0;
    maxDelaySamples = maxDelayMilliseconds * 0.001f * static_cast<float>(sampleRate);

    const auto ringSize = juce::jmax(2, static_cast<int>(std::ceil(maxDelaySamples)) + 4);
    const auto channels = juce::jmax(1, static_cast<int>(spec.numChannels));

    ring.setSize(channels, ringSize, false, false, true);
    delaySamples.reset(sampleRate, 0.08);
    reset();
}

void TimeDelay::reset()
{
    ring.clear();
    writePos = 0;
    delaySamples.setCurrentAndTargetValue(delaySamples.getTargetValue());
}

void TimeDelay::setDelayMilliseconds(float delayMs) noexcept
{
    const auto samples = juce::jlimit(0.0f, maxDelaySamples, delayMs * 0.001f * static_cast<float>(sampleRate));
    delaySamples.setTargetValue(samples);
}

void TimeDelay::process(juce::AudioBuffer<float>& buffer) noexcept
{
    const auto numChannels = juce::jmin(buffer.getNumChannels(), ring.getNumChannels());
    const auto numSamples = buffer.getNumSamples();
    const auto ringSize = ring.getNumSamples();

    if (numChannels <= 0 || numSamples <= 0 || ringSize <= 1)
        return;

    for (int i = 0; i < numSamples; ++i)
    {
        const auto delay = juce::jlimit(0.0f, maxDelaySamples, delaySamples.getNextValue());
        const auto delayInt = static_cast<int>(delay);
        const auto frac = delay - static_cast<float>(delayInt);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            ring.setSample(channel, writePos, buffer.getSample(channel, i));

            auto indexA = writePos - delayInt;
            while (indexA < 0)
                indexA += ringSize;

            auto indexB = indexA - 1;
            if (indexB < 0)
                indexB += ringSize;

            const auto sampleA = ring.getSample(channel, indexA);
            const auto sampleB = ring.getSample(channel, indexB);
            buffer.setSample(channel, i, sampleA + frac * (sampleB - sampleA));
        }

        writePos = (writePos + 1) % ringSize;
    }
}
