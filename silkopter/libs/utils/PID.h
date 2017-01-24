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
        Factor_t d_filter = Factor_t();
        Scalar_t rate = 0;
    };

    explicit PID(Params const& params = Params());

    auto set_params(Params const& params) -> bool;
    auto process(Value_t const& input, Value_t const& target) -> Factor_t;
    auto process_ex(Value_t const& input, Value_t const& target, Value_t const& derivative) -> Factor_t;
    void reset();

private:
    Params m_params;
    float m_dts = 0;
    float m_dts_inv = 0;
    bool m_has_ki = false;
    bool m_has_kd = false;
    Value_t m_integrator;
    Value_t m_last_input;
    Factor_t m_last_derivative = Factor_t();
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
auto PID<Scalar, Value, Factor>::process_ex(Value_t const& input, Value_t const& target, Value_t const& derivative) -> Factor_t
{
    // Compute proportional component
    auto error = pid::sub<Value_t, Factor_t>(target, input);
    auto output = error * m_params.kp;

    // Compute integral component if time has elapsed
    if (m_has_ki)
    {
        m_integrator = math::clamp(m_integrator + Factor_t(error * m_params.ki * m_dts), -m_params.max_i, m_params.max_i);
        output += m_integrator;
    }

    // Compute derivative component if time has elapsed
    if (m_has_kd)
    {
        //low pass filter
        //Note - a butterworth doesn't work here due to some weird ringing
        Value_t d = m_last_derivative + (m_dts / (m_params.d_filter + m_dts)) * (derivative - m_last_derivative);
        m_last_derivative = d;

        // add in derivative component
        //m_output += m_params.kd * derivative;
        output -= m_params.kd * d;
    }

    return output;
}

template<class Scalar, class Value, class Factor>
auto PID<Scalar, Value, Factor>::process(Value_t const& input, Value_t const& target) -> Factor_t
{
    // Compute derivative component if time has elapsed
    Factor_t derivative = Factor_t();
    if (m_has_kd)
    {
        if (!m_is_last_input_valid || math::is_nan(m_last_derivative) || math::is_nan(m_last_input))
        {
            m_last_input = input;
            m_is_last_input_valid = true;
            m_last_derivative = Factor_t();
        }
        else
        {
            derivative = pid::sub<Value_t, Factor_t>(input, m_last_input) * m_dts_inv;
        }
        m_last_input = input;
    }

    return process_ex(input, target, derivative);
}

template<class Scalar, class Value, class Factor>
void PID<Scalar, Value, Factor>::reset()
{
    m_integrator = Value_t();
    m_is_last_input_valid = false;
    m_last_input = Value_t();
    m_last_derivative = Factor_t();
}

template<class Scalar, class Value, class Factor>
auto PID<Scalar, Value, Factor>::set_params(Params const& params) -> bool
{
    if (params.rate <= 0)
    {
        return false;
    }

    m_params = params;
    m_has_ki = !math::is_zero(params.ki, math::epsilon<Scalar>());
    m_has_kd = !math::is_zero(params.kd, math::epsilon<Scalar>());
    m_dts = 1.f / params.rate;
    m_dts_inv = 1.f / m_dts;
    return true;
}

}


