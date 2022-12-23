#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleReverbAudioProcessor::SimpleReverbAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
}

SimpleReverbAudioProcessor::~SimpleReverbAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SimpleReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SimpleReverbAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SimpleReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleReverbAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleReverbAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SimpleReverbAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SimpleReverbAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void SimpleReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec{};

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<unsigned int>(samplesPerBlock);
    spec.numChannels = 1;

    reverbL.prepare(spec);
    reverbR.prepare(spec);
}

void SimpleReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SimpleReverbAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void SimpleReverbAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                              juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    params.roomSize = *apvts.getRawParameterValue("size");
    params.damping = *apvts.getRawParameterValue("damp");
    params.width = *apvts.getRawParameterValue("width");
    params.wetLevel = *apvts.getRawParameterValue("dw");
    params.dryLevel = 1.0f - *apvts.getRawParameterValue("dw");
    params.freezeMode = *apvts.getRawParameterValue("freeze");

    reverbL.setParameters(params);
    reverbR.setParameters(params);

    juce::dsp::AudioBlock<float> block(buffer);
    auto blockL = block.getSingleChannelBlock(0);
    auto blockR = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> contextL(blockL);
    juce::dsp::ProcessContextReplacing<float> contextR(blockR);

    reverbL.process(contextL);
    reverbR.process(contextR);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *channelData = buffer.getWritePointer(channel);
        juce::ignoreUnused(channelData);
        // ..do something to the data...
    }
}

//==============================================================================
bool SimpleReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *SimpleReverbAudioProcessor::createEditor()
{
    // return new juce::GenericAudioProcessorEditor (*this);
    return new SimpleReverbAudioProcessorEditor(*this);
}

//==============================================================================
void SimpleReverbAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void SimpleReverbAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "size",
        "size",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int)
        {
                                                               if (value * 100 < 10.0f)
                                                                   return juce::String (value * 100, 2);
                                                               else if(value * 100 < 100.0f)
                                                                   return juce::String (value * 100, 1);
                                                               else
                                                                   return juce::String (value * 100, 0); },
        nullptr));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "damp",
        "damp",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int)
        {
                                                               if (value * 100 < 10.0f)
                                                                   return juce::String (value * 100, 2);
                                                               else if(value * 100 < 100.0f)
                                                                   return juce::String (value * 100, 1);
                                                               else
                                                                   return juce::String (value * 100, 0); },
        nullptr));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "width",
        "width",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int)
        {
                                                               if (value * 100 < 10.0f)
                                                                   return juce::String (value * 100, 2);
                                                               else if(value * 100 < 100.0f)
                                                                   return juce::String (value * 100, 1);
                                                               else
                                                                   return juce::String (value * 100, 0); },
        nullptr));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "dw",
        "dw",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int)
        {
                                                               if (value * 100 < 10.0f)
                                                                   return juce::String (value * 100, 2);
                                                               else if(value * 100 < 100.0f)
                                                                   return juce::String (value * 100, 1);
                                                               else
                                                                   return juce::String (value * 100, 0); },
        nullptr));
    layout.add(std::make_unique<juce::AudioParameterBool>("freeze", "freeze", false));
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleReverbAudioProcessor();
}
