#pragma once 

namespace util
{

    class Nop_LPF
    {
    public:
        float process(float t)
        {
            return t;
        }
    };

    class Butterworth_500_20_LPF
    {
    public:
        /* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
           Command line: /www/usr/fisher/helpers/mkfilter -Bu -Lp -o 4 -a 1.0000000000e-02 0.0000000000e+00 -l */

        constexpr static size_t NZEROS = 4;
        constexpr static size_t NPOLES = 4;
        constexpr static float GAIN = 1.112983215e+06f;

        float xv[NZEROS+1], yv[NPOLES+1];

        float process(float t)
        {
            xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
            xv[4] = t / GAIN;
            yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4];
            yv[4] =   (xv[0] + xv[4]) + 4.f * (xv[1] + xv[3]) + 6.f * xv[2]
                        + ( -0.8485559993f * yv[0]) + (  3.5335352195f * yv[1])
                        + ( -5.5208191366f * yv[2]) + (  3.8358255406f * yv[3]);
            return yv[4];
        }
    };

    struct PID_Params
    {
        PID_Params() = default;
        PID_Params(float kp, float ki, float kd, float max)
            : kp(kp), ki(ki), kd(kd), max(max) {}

        float kp = 0;
        float ki = 0;
        float kd = 0;
        float max = 0;
    };

    template<class LPF = Nop_LPF>
    class PID
    {
    public:
        typedef PID_Params Params;

        explicit PID(LPF const& lpf = LPF());
        explicit PID(Params const& params, LPF const& lpf = LPF());

        void set_target(float target);
        auto get_target() const -> float;

        void set_input(float input);
        auto get_input() const -> float;

        auto get_output() const -> float;

        float process(q::Clock::duration dt);

        void reset();

        auto get_params() const -> Params const&;
        void set_params(Params const& params);

    private:
        LPF m_lpf;
        Params m_params;
        bool m_has_ki = false;
        bool m_has_kd = false;
        float m_integrator = 0;
        //float m_last_error = 0;
        float m_last_filtered_input = 0;
        float m_target = 0;
        float m_input = 0;
        float m_error = 0;
        float m_output = 0;
        float m_clamped_output = 0;
        bool m_is_last_filtered_input_valid = false;
        q::Clock::duration m_duration;
    };


    template<class LPF>
    PID<LPF>::PID(LPF const& lpf)
        : m_lpf(lpf)
    {
    }

    template<class LPF>
    PID<LPF>::PID(Params const& params, LPF const& lpf)
        : m_lpf(lpf)
    {
        set_params(params);
    }

    template<class LPF>
    void PID<LPF>::set_target(float target)
    {
        m_target = target;
    }
    template<class LPF>
    auto PID<LPF>::get_target() const -> float
    {
        return m_target;
    }
    template<class LPF>
    void PID<LPF>::set_input(float input)
    {
        m_input = input;
    }
    template<class LPF>
    auto PID<LPF>::get_input() const -> float
    {
        return m_input;
    }
    template<class LPF>
    auto PID<LPF>::get_output() const -> float
    {
        return m_clamped_output;
    }

    template<class LPF>
    float PID<LPF>::process(q::Clock::duration dt)
    {
        // Compute proportional component
        m_error = m_target - m_input;

        bool is_saturated = m_output <= -m_params.max || m_output >= m_params.max;

        m_output = 0;
        m_output += m_error * m_params.kp;

        auto dts = q::Seconds(dt).count();
        if (dts > 0)
        {
            // Compute integral component if time has elapsed
            if (m_has_ki && !is_saturated)
            {
                m_integrator += m_error * dts;
                m_output += m_params.ki * m_integrator;
            }

            // Compute derivative component if time has elapsed
            if (m_has_kd)
            {
                float filtered_input = m_lpf.process(m_input);
                if (!m_is_last_filtered_input_valid)
                {
                    m_last_filtered_input = filtered_input;
                    m_is_last_filtered_input_valid = true;
                }

                //float derivative = (m_error - m_last_error) / dt.count;
                float derivative = (filtered_input - m_last_filtered_input) / dts;

                m_last_filtered_input = filtered_input;

                // add in derivative component
                //m_output += m_params.kd * derivative;
                m_output -= m_params.kd * derivative;
            }
        }

        m_clamped_output = math::clamp(m_output, -m_params.max, m_params.max);
        return m_clamped_output;
    }

    template<class LPF>
    void PID<LPF>::reset()
    {
        m_integrator = 0;
        m_is_last_filtered_input_valid = false;
        //m_last_error = 0;
        m_last_filtered_input = 0;
        m_error = 0;
        m_target = 0;
        m_input = 0;
        m_output = 0;
        m_clamped_output = 0;
    }

    template<class LPF>
    auto PID<LPF>::get_params() const -> Params const&
    {
        return m_params;
    }
    template<class LPF>
    void PID<LPF>::set_params(Params const& params)
    {
        m_params = params;
        m_has_ki = !math::is_zero(params.ki, math::epsilon<float>());
        m_has_kd = !math::is_zero(params.kd, math::epsilon<float>());
    }

}


