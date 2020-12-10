#pragma once

#include <JuceHeader.h>
#include "Utils.h"

using namespace dsp;

//==============================================================================
struct CabConvolutionProcessor
{
    void prepare (const ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        gain.setGainDecibels(6);
        convolution.prepare (spec);
        updateParameters();
    }

    void process (ProcessContextReplacing<float> context)
    {
        context.isBypassed = bypass;

        // Load a new IR if there's one available. Note that this doesn't lock or allocate!
        bufferTransfer.get ([this] (BufferWithSampleRate& buf)
        {
            convolution.loadImpulseResponse (std::move (buf.buffer),
                                             buf.sampleRate,
                                             Convolution::Stereo::yes,
                                             Convolution::Trim::yes,
                                             Convolution::Normalise::yes);
        });

        convolution.process (context);
        gain.process(context);
    }

    void reset()
    {
        convolution.reset();
    }

    void updateParameters()
    {
        /*
        auto* cabinetTypeParameter = dynamic_cast<ChoiceParameter*> (parameters[0]);

        if (cabinetTypeParameter == nullptr)
        {
            jassertfalse;
            return;
        }

        if (cabinetTypeParameter->getCurrentSelectedID() == 1)
        {
            bypass = true;
        }
        else
        {
        */
            bypass = false;

            //auto selectedType = cabinetTypeParameter->getCurrentSelectedID();
            //auto assetName = (selectedType == 2 ? "guitar_amp.wav" : "cassette_recorder.wav");
            auto assetName = "cabsim.wav";

            auto assetInputStream = createAssetInputStream (assetName);

            if (assetInputStream == nullptr)
            {
                jassertfalse;
                return;
            }

            AudioFormatManager manager;
            manager.registerBasicFormats();
            std::unique_ptr<AudioFormatReader> reader { manager.createReaderFor (std::move (assetInputStream)) };

            if (reader == nullptr)
            {
                jassertfalse;
                return;
            }

            AudioBuffer<float> buffer (static_cast<int> (reader->numChannels),
                                       static_cast<int> (reader->lengthInSamples));
            reader->read (buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

            bufferTransfer.set (BufferWithSampleRate { std::move (buffer), reader->sampleRate });
        //}
    }

    //==============================================================================
    struct BufferWithSampleRate
    {
        BufferWithSampleRate() = default;

        BufferWithSampleRate (AudioBuffer<float>&& bufferIn, double sampleRateIn)
            : buffer (std::move (bufferIn)), sampleRate (sampleRateIn) {}

        AudioBuffer<float> buffer;
        double sampleRate = 0.0;
    };

    class BufferTransfer
    {
    public:
        void set (BufferWithSampleRate&& p)
        {
            const SpinLock::ScopedLockType lock (mutex);
            buffer = std::move (p);
            newBuffer = true;
        }

        // Call `fn` passing the new buffer, if there's one available
        template <typename Fn>
        void get (Fn&& fn)
        {
            const SpinLock::ScopedTryLockType lock (mutex);

            if (lock.isLocked() && newBuffer)
            {
                fn (buffer);
                newBuffer = false;
            }
        }

    private:
        BufferWithSampleRate buffer;
        bool newBuffer = false;
        SpinLock mutex;
    };

    double sampleRate = 0.0;
    bool bypass = false;

    MemoryBlock currentCabinetData;
    Convolution convolution;
    Gain<float> gain;

    BufferTransfer bufferTransfer;

    //ChoiceParameter cabinetParam { { "Bypass", "Guitar amplifier 8''", "Cassette recorder" }, 1, "Cabinet Type" };

    //std::vector<DSPDemoParameterBase*> parameters { &cabinetParam };
};
