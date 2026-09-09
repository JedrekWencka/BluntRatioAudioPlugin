#include "PluginEditor.h"

BluntRationAudioProcessorEditor::BluntRationAudioProcessorEditor(BluntRationAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    titleLabel.setText("BLUNT RATION", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setFont(juce::FontOptions(24.0f).withStyle("Bold"));
    addAndMakeVisible(titleLabel);

    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 24);
    gainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(gainSlider);

    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    gainAttachment = std::make_unique<SliderAttachment>(
        processor.getParameters(),
        BluntRationAudioProcessor::gainParameterId,
        gainSlider);

    setResizable(true, true);
    setResizeLimits(320, 260, 900, 700);
    setSize(420, 340);
}

void BluntRationAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(22, 22, 22));

    g.setColour(juce::Colour::fromRGB(235, 235, 230));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(12.0f), 12.0f, 1.0f);
}

void BluntRationAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(24);

    titleLabel.setBounds(area.removeFromTop(50));
    area.removeFromTop(12);

    gainLabel.setBounds(area.removeFromTop(28));
    gainSlider.setBounds(area.reduced(55, 0));
}
