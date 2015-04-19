#pragma once

#include "qmath.h"

namespace util
{

namespace dsp
{

template<class T> bool equals(T const& a, T const& b)
{
    return math::equals(a, b);
}

template<class T> void apply_coefficients(T& x, T& w0, T& w1, T& w2, float d1, float d2, float A)
{
    w0 = d1*w1 + d2*w2 + x;
    x = A*(w0 + 2.f*w1 + w2);
    w2 = w1;
    w1 = w0;
}

}



template<class T>
class Butterworth : q::util::Noncopyable
{
public:
    auto setup(size_t order, float rate, float cutoff_frequency) -> bool
    {
        if (rate < math::epsilon<float>() ||
            cutoff_frequency < math::epsilon<float>())
        {
            return false;
        }
//        m_dsp.setup(poles, rate, cutoff_frequency);
        m_rate = rate;
        m_order = order;

//        printf("  n = filter order 2,4,6,...\n");
//        printf("  s = sampling frequency\n");
//        printf("  f = half power frequency\n");

//        float s = rate;
//        float f = cutoff_frequency;
        float a = math::tan(math::anglef::pi*cutoff_frequency/rate);
        float a2 = a*a;
        A.resize(m_order);
        d1.resize(m_order);
        d2.resize(m_order);
        w0.resize(m_order);
        w1.resize(m_order);
        w2.resize(m_order);

        for(size_t i = 0; i < m_order; ++i)
        {
            float r = math::sin(math::anglef::pi*(2.f*i+1.f)/(4.f*m_order));
            float s = a2 + 2.f*a*r + 1.f;
            A[i] = a2/s;
            d1[i] = 2.f*(1.f-a2)/s;
            d2[i] = -(a2 - 2.f*a*r + 1.f)/s;
        }

        return true;
    }

    void reset(T const& t)
    {
        for(size_t i = 0; i < m_order; ++i)
        {
            w0[i] = t;
            w1[i] = t;
            w2[i] = t;
        }
        for (size_t i = 0; i < static_cast<size_t>(m_rate) * 100; i++)
        {
            T copy = t;
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            process(copy);
            if (dsp::equals(copy, t))
            {
                break;
            }
        }
    }
    void reset()
    {
        auto t = m_last; //need to make a copy as reset will change it
        reset(t);
    }

    void process(T& t)
    {
        if (m_needs_reset)
        {
            m_needs_reset = false;
            reset(t);
        }
        for(size_t i = 0; i < m_order; ++i)
        {
            dsp::apply_coefficients(t, w0[i], w1[i], w2[i], d1[i], d2[i], A[i]);
        }
        m_last = t;
    }

private:
    size_t m_order = 0;
    float m_rate = 0;
    bool m_needs_reset = true;
    std::vector<float> A;
    std::vector<float> d1;
    std::vector<float> d2;
    std::vector<T> w0;
    std::vector<T> w1;
    std::vector<T> w2;
    T m_last;
};



}
