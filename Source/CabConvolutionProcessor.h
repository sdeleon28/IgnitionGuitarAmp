#pragma once

#include <JuceHeader.h>

using namespace dsp;


class CabConvolutionProcessor
{
public:
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


    void prepare (const ProcessSpec& spec);

    void process (ProcessContextReplacing<float> context);

    void reset();

    void updateParameters();

    double sampleRate = 0.0;
    bool bypass = false;

    MemoryBlock currentCabinetData;
    Convolution convolution;
    Gain<float> gain;

    BufferTransfer bufferTransfer;
};
