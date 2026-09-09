#include "PluginEditor.h"
#include <BinaryData.h>

#include <cmath>

void DelaySliderLookAndFeel::setImages(juce::Image track, juce::Image fill, juce::Image thumb)
{
    trackImage = std::move(track);
    fillImage = std::move(fill);
    thumbImage = std::move(thumb);
}

void DelaySliderLookAndFeel::advanceSmoke()
{
    smokePhase += 0.085f;
    if (smokePhase > juce::MathConstants<float>::twoPi)
        smokePhase -= juce::MathConstants<float>::twoPi;
}

void DelaySliderLookAndFeel::drawSmoke(juce::Graphics& g, juce::Point<float> origin) const
{
    g.setColour(juce::Colours::white.withAlpha(0.10f));
    g.fillEllipse(origin.x - 10.0f, origin.y - 22.0f, 20.0f, 16.0f);

    for (int wisp = 0; wisp < 5; ++wisp)
    {
        const auto phase = smokePhase + static_cast<float>(wisp) * 0.75f;
        const auto drift = static_cast<float>(wisp) - 2.0f;
        juce::Path smoke;
        const auto start = origin.translated(drift * 1.4f, -14.0f);
        smoke.startNewSubPath(start);
        smoke.cubicTo(start.x + 14.0f * std::sin(phase),
                      start.y - 28.0f,
                      start.x - 18.0f * std::sin(phase + 0.9f),
                      start.y - 56.0f,
                      start.x + 10.0f * std::cos(phase * 0.65f) + drift * 3.0f,
                      start.y - 88.0f - static_cast<float>(wisp) * 6.0f);

        g.setColour(juce::Colours::white.withAlpha(0.32f - 0.04f * static_cast<float>(wisp)));
        g.strokePath(smoke, juce::PathStrokeType(5.2f - 0.45f * static_cast<float>(wisp),
                                                 juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));
    }
}

void DelaySliderLookAndFeel::drawLinearSlider(juce::Graphics& g,
                                              int,
                                              int,
                                              int,
                                              int,
                                              float sliderPos,
                                              float,
                                              float,
                                              juce::Slider::SliderStyle,
                                              juce::Slider& slider)
{
    auto bounds = slider.getLocalBounds().toFloat();
    const auto trackHeight = juce::jmax(16.0f, bounds.getHeight() * 0.22f);
    const auto trackY = bounds.getY() + bounds.getHeight() * 0.50f;
    auto track = juce::Rectangle<float>(bounds.getX() + 6.0f,
                                        trackY,
                                        bounds.getWidth() - 12.0f,
                                        trackHeight);

    if (trackImage.isValid())
        g.drawImage(trackImage, track, juce::RectanglePlacement::centred | juce::RectanglePlacement::stretchToFit);

    auto inner = track.reduced(6.0f, track.getHeight() * 0.22f);
    const auto fillRight = juce::jlimit(inner.getX(), inner.getRight(), sliderPos);

    if (fillImage.isValid() && fillRight - inner.getX() > 1.5f)
    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(juce::Rectangle<float>(inner.getX(), inner.getY(), fillRight - inner.getX(), inner.getHeight())
                               .toNearestInt());
        g.drawImage(fillImage, inner, juce::RectanglePlacement::centred | juce::RectanglePlacement::stretchToFit);
    }

    const auto thumbCentre = juce::Point<float>(sliderPos, track.getCentreY());

    if (thumbImage.isValid())
    {
        const auto thumbSize = juce::jlimit(26.0f, 34.0f, trackHeight * 1.15f);
        auto thumbArea = juce::Rectangle<float>(thumbSize, thumbSize).withCentre(thumbCentre);
        g.drawImage(thumbImage, thumbArea, juce::RectanglePlacement::centred);
    }

    drawSmoke(g, thumbCentre);
}

namespace
{
constexpr const char* aboutBody =
    "Relax the pocket.\n\n"
    "Delay the whole track by 0 to 250 ms in 0.1 ms steps so it sits later than tracks without the plugin. "
    "Latency compensation is intentionally off. The delay is the effect.";

juce::Rectangle<int> aboutCardBounds(juce::Rectangle<int> parent)
{
    return parent.withSizeKeepingCentre(juce::jmin(520, parent.getWidth() - 40), 280);
}
}

AboutOverlay::AboutOverlay()
{
    closeButton.onClick = [this] { setVisible(false); };
    addAndMakeVisible(closeButton);
    setAlwaysOnTop(true);
    setVisible(false);
}

void AboutOverlay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.withAlpha(0.55f));

    cardBounds = aboutCardBounds(getLocalBounds());
    auto card = cardBounds.toFloat();

    g.setColour(juce::Colour::fromRGB(18, 20, 18));
    g.fillRoundedRectangle(card, 12.0f);
    g.setColour(juce::Colour::fromRGB(210, 210, 200));
    g.drawRoundedRectangle(card, 12.0f, 1.0f);

    auto text = cardBounds.reduced(28, 22);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(22.0f).withStyle("Bold"));
    g.drawText(juce::String("BluntRatio v") + BLUNTRATIO_BUILD_VERSION,
               text.removeFromTop(36),
               juce::Justification::centredLeft);

    g.setColour(juce::Colour::fromRGB(210, 210, 200));
    g.setFont(juce::FontOptions(15.0f));
    g.drawFittedText(aboutBody, text.withTrimmedBottom(56), juce::Justification::topLeft, 10);
}

void AboutOverlay::resized()
{
    cardBounds = aboutCardBounds(getLocalBounds());
    closeButton.setBounds(cardBounds.removeFromBottom(52).withSizeKeepingCentre(88, 32));
}

void AboutOverlay::mouseUp(const juce::MouseEvent& event)
{
    if (!aboutCardBounds(getLocalBounds()).contains(event.getPosition()))
        setVisible(false);
}

BluntRatioAudioProcessorEditor::BluntRatioAudioProcessorEditor(BluntRatioAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    background = juce::ImageCache::getFromMemory(BinaryData::uipanel_jpg, BinaryData::uipanel_jpgSize);

    sliderLookAndFeel.setImages(
        juce::ImageCache::getFromMemory(BinaryData::slidertrack_png, BinaryData::slidertrack_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::sliderfill_png, BinaryData::sliderfill_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::sliderthumb_png, BinaryData::sliderthumb_pngSize));

    delaySlider.setLookAndFeel(&sliderLookAndFeel);
    delaySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delaySlider.setSliderSnapsToMousePosition(true);
    delaySlider.setMouseDragSensitivity(2500);
    delaySlider.setDoubleClickReturnValue(true, 0.0);
    delaySlider.setOpaque(false);
    delaySlider.setPaintingIsUnclipped(false);
    delaySlider.setBufferedToImage(false);
    addAndMakeVisible(delaySlider);

    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(),
        BluntRatioAudioProcessor::delayParameterId,
        delaySlider);

    infoHit.onClick = [this] { showAbout(); };
    addAndMakeVisible(infoHit);
    addAndMakeVisible(aboutOverlay);
    aboutOverlay.setVisible(false);

    startTimerHz(28);

    setOpaque(true);
    setResizable(true, true);
    const auto aspect = 1000.0 / 658.0;
    if (auto* constrainer = getConstrainer())
        constrainer->setFixedAspectRatio(aspect);
    setResizeLimits(640, 422, 1400, 921);
    setSize(1000, 658);
}

BluntRatioAudioProcessorEditor::~BluntRatioAudioProcessorEditor()
{
    stopTimer();
    delaySlider.setLookAndFeel(nullptr);
}

void BluntRatioAudioProcessorEditor::timerCallback()
{
    sliderLookAndFeel.advanceSmoke();
    delaySlider.repaint();
}

void BluntRatioAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(8, 10, 7));

    if (background.isValid())
        g.drawImage(background, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
}

void BluntRatioAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().toFloat();

    delaySlider.setBounds(juce::Rectangle<float>(bounds.getWidth() * 0.165f,
                                                 bounds.getHeight() * 0.548f,
                                                 bounds.getWidth() * 0.670f,
                                                 bounds.getHeight() * 0.280f)
                              .toNearestInt());

    infoHit.setBounds(juce::Rectangle<int>(0, 0, 48, 48)
                          .withCentre({ juce::roundToInt(bounds.getWidth() * 0.974f),
                                        juce::roundToInt(bounds.getHeight() * 0.043f) }));
    infoHit.toFront(false);

    aboutOverlay.setBounds(getLocalBounds());
    aboutOverlay.toFront(false);
}

void BluntRatioAudioProcessorEditor::showAbout()
{
    aboutOverlay.setVisible(true);
    aboutOverlay.toFront(false);
    aboutOverlay.grabKeyboardFocus();
}
