#pragma once

#include "DspFilters/Butterworth.h"
#include "qmath.h"

namespace util
{

template<size_t MAX_POLES, size_t CHANNELS>
class Butterworth : q::util::Noncopyable
{
public:
    auto setup(size_t poles, double rate, double cutoff_frequency) -> bool
    {
        if (poles > MAX_POLES)
        {
            return false;
        }
        m_dsp.setup(poles, rate, cutoff_frequency);
        m_rate = rate;
        return true;
    }

    void reset(double const* channels)
    {
        QASSERT(channels);
        m_dsp.reset(channels);
        double channels_copy[CHANNELS];
        for (size_t i = 0; i < static_cast<size_t>(m_rate) * 100; i++)
        {
            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            memcpy(channels_copy, channels, sizeof(double) * CHANNELS);
            m_dsp.process(channels_copy);

            bool is_equal = true;
            for (size_t ch = 0; ch < CHANNELS; ch++)
            {
                if (!math::equals(channels_copy[ch], channels[ch], std::numeric_limits<double>::epsilon()))
                {
                    is_equal = false;
                    break;
                }
            }
            if (is_equal)
            {
                return;
            }
        }
    }
    void process(double* channels)
    {
        QASSERT(channels);
        if (m_needs_reset)
        {
            m_needs_reset = false;
            reset(channels);
        }
        m_dsp.process(channels);
    }

private:
    double m_rate = 0;
    bool m_needs_reset = true;
    Dsp::SimpleFilter <Dsp::Butterworth::LowPass<MAX_POLES>, CHANNELS> m_dsp;
};



}
