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

template<class T> T add(T const& a, T const& b)
{
    return a + b;
}

template<class T> T scale(T const& a, double scale)
{
    return a * scale;
}

template<class T> void fix(T& a)
{
    ; //nothing
}


}



template<class T>
class Butterworth : q::util::Noncopyable
{
public:
    auto setup(size_t poles, double rate, double cutoff_frequency) -> bool
    {
        if (poles % 2 == 1 ||
            math::is_zero(rate, std::numeric_limits<double>::epsilon()))
        {
            return false;
        }
//        m_dsp.setup(poles, rate, cutoff_frequency);
        m_rate = rate;
        m_poles = poles;

//        printf("  n = filter order 2,4,6,...\n");
//        printf("  s = sampling frequency\n");
//        printf("  f = half power frequency\n");

        size_t n = poles / 2;
//        double s = rate;
//        double f = cutoff_frequency;
        double a = math::tan(math::angled::pi*cutoff_frequency/rate);
        double a2 = a*a;
        A.resize(n);
        d1.resize(n);
        d2.resize(n);
        w0.resize(n);
        w1.resize(n);
        w2.resize(n);

        for(size_t i = 0; i < n; ++i)
        {
            double r = math::sin(math::angled::pi*(2.0*i+1.0)/(4.0*n));
            double s = a2 + 2.0*a*r + 1.0;
            A[i] = a2/s;
            d1[i] = 2.0*(1-a2)/s;
            d2[i] = -(a2 - 2.0*a*r + 1.0)/s;
        }

        return true;
    }

    void reset(T const& t)
    {
        for(size_t i = 0; i < m_poles / 2; ++i)
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
//        m_dsp.process(t);
        for(size_t i=0; i < m_poles / 2; ++i)
        {
            w0[i] = dsp::add(dsp::add(dsp::scale(w1[i], d1[i]), dsp::scale(w2[i], d2[i])), t);
            t = dsp::scale(dsp::add(dsp::add(w0[i], dsp::scale(w1[i], 2.0)), w2[i]), A[i]);
            w2[i] = w1[i];
            w1[i] = w0[i];
        }
        dsp::fix(t);
        m_last = t;
    }

private:
    size_t m_poles = 0;
    double m_rate = 0;
    bool m_needs_reset = true;
    std::vector<double> A;
    std::vector<double> d1;
    std::vector<double> d2;
    std::vector<T> w0;
    std::vector<T> w1;
    std::vector<T> w2;
    T m_last;
};



}
