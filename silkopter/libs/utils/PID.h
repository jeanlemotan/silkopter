#pragma once 

#include "utils/Butterworth.h"


namespace util
{

namespace pid
{
template<class Value, class Factor> Factor sub(Value const& a, Value const& b)
{
    return Factor(a - b);
}
}

template<class Scalar, class Value, class Factor>
class PID
{
public:
    typedef Scalar Scalar_t;
    typedef Value Value_t;
    typedef Factor Factor_t;

    struct Params
    {
        Scalar_t kp = 0;
        Scalar_t ki = 0;
        Scalar_t kd = 0;
        Factor_t max_i = Factor_t();
        uint32_t filter_poles = 0; //zero - filter disabled
        Scalar_t filter_cutoff_frequency = 0;
        Scalar_t rate = 0;
    };

    explicit PID(Params const& params = Params());

    auto set_params(Params const& params) -> bool;
    auto process(Value_t const& input, Value_t const& target) -> Factor_t;
    void reset();

private:
    util::Butterworth<Value_t> m_dsp;
    Params m_params;
    double m_dts = 0;
    double m_dts_inv = 0;
    bool m_has_ki = false;
    bool m_has_kd = false;
    bool m_has_filter = false;
    Value_t m_integrator;
    Value_t m_last_input;
    bool m_is_last_input_valid = false;
};


template<class Scalar, class Value, class Factor>
PID<Scalar, Value, Factor>::PID(Params const& params)
 : m_integrator()
 , m_last_input()
{
    set_params(params);
}

template<class Scalar, class Value, class Factor>
auto PID<Scalar, Value, Factor>::process(Value_t const& input, Value_t const& target) -> Factor_t
{
    // Compute proportional component
    auto error = pid::sub<Value_t, Factor_t>(target, input);
    auto output = error * m_params.kp;

    // Compute integral component if time has elapsed
    if (m_has_ki)
    {
        m_integrator = math::clamp(m_integrator + Factor_t(error * m_dts), -m_params.max_i, m_params.max_i);
        output += m_integrator * m_params.ki;
    }

    // Compute derivative component if time has elapsed
    if (m_has_kd)
    {
        Factor_t derivative;
        if (m_has_filter)
        {
            auto filtered_input = input;
            m_dsp.process(filtered_input);
            if (!m_is_last_input_valid)
            {
                m_last_input = filtered_input;
                m_is_last_input_valid = true;
            }
            derivative = pid::sub<Value_t, Factor_t>(filtered_input, m_last_input) * m_dts_inv;
            m_last_input = filtered_input;
        }
        else
        {
            if (!m_is_last_input_valid)
            {
                m_last_input = input;
                m_is_last_input_valid = true;
            }
            derivative = pid::sub<Value_t, Factor_t>(input, m_last_input) * m_dts_inv;
            m_last_input = input;
        }

        // add in derivative component
        //m_output += m_params.kd * derivative;
        output -= m_params.kd * derivative;
    }

    return output;
}

template<class Scalar, class Value, class Factor>
void PID<Scalar, Value, Factor>::reset()
{
    m_integrator = Value_t();
    m_is_last_input_valid = false;
    m_last_input = Value_t();
}

template<class Scalar, class Value, class Factor>
auto PID<Scalar, Value, Factor>::set_params(Params const& params) -> bool
{
    if (params.rate <= 0)
    {
        return false;
    }

    if (params.filter_poles <= 0 || params.filter_cutoff_frequency <= 0)
    {
        m_has_filter = false;
    }
    else
    {
        m_has_filter = true;
        if (!m_dsp.setup(params.filter_poles, params.rate, params.filter_cutoff_frequency))
        {
            return false;
        }
        m_dsp.reset();
    }

    m_params = params;
    m_has_ki = !math::is_zero(params.ki, math::epsilon<Scalar>());
    m_has_kd = !math::is_zero(params.kd, math::epsilon<Scalar>());
    m_dts = 1.0 / params.rate;
    m_dts_inv = 1.0 / m_dts;
    return true;
}

}


