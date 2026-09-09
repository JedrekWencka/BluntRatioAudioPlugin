#include "DSP/TimeDelay.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace
{
int failures = 0;

void expectTrue(bool condition, const char* message)
{
    if (!condition)
    {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void expectNear(float actual, float expected, float tolerance, const char* message)
{
    if (std::abs(actual - expected) > tolerance)
    {
        std::cerr << "FAIL: " << message << " (got " << actual << ", expected " << expected << ")\n";
        ++failures;
    }
}
}

int main()
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 1000.0; // 1 sample == 1 ms
    spec.maximumBlockSize = 32;
    spec.numChannels = 1;

    TimeDelay delay;
    delay.prepare(spec);

    juce::AudioBuffer<float> buffer(1, 8);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
        buffer.setSample(0, i, static_cast<float>(i + 1));

    delay.setDelayMilliseconds(0.0f);
    delay.reset();
    delay.process(buffer);

    expectNear(buffer.getSample(0, 0), 1.0f, 1.0e-5f, "0 ms delay is transparent");
    expectNear(buffer.getSample(0, 7), 8.0f, 1.0e-5f, "0 ms delay keeps later samples");

    delay.prepare(spec);
    delay.setDelayMilliseconds(2.0f);
    delay.reset();

    juce::AudioBuffer<float> delayed(1, 8);
    delayed.clear();
    delayed.setSample(0, 0, 1.0f);

    delay.process(delayed);

    expectNear(delayed.getSample(0, 0), 0.0f, 1.0e-5f, "2 ms delay silences the first sample");
    expectNear(delayed.getSample(0, 2), 1.0f, 1.0e-5f, "impulse arrives 2 samples later");
    expectTrue(std::isfinite(delayed.getSample(0, 7)), "output is finite");

    if (failures > 0)
    {
        std::cerr << failures << " test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "TimeDelay tests passed\n";
    return EXIT_SUCCESS;
}
