#pragma once 

namespace util
{

    template<class T>
    class Nop_LPF
    {
    public:
        T process(T const& t)
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

    template<class T, class LPF = Nop_LPF<T>>
    class PID
    {
    public:
        struct Params
        {
            Params() = default;
            Params(float kp, float ki, float kd, T const& max)
                : kp(kp), ki(ki), kd(kd), max(max) {}

            float kp = 0;
            float ki = 0;
            float kd = 0;
            T max = T();
        };

        PID(LPF const& lpf = LPF());
        PID(Params const& params, LPF const& lpf = LPF());

        void set_target(T const& target);
        auto get_target() const -> T const&;

        void set_input(T const& input);
        auto get_input() const -> T const&;

        auto get_output() const -> T const&;

        T process(q::Clock::duration dt);

        void reset();

        Params const& get_params() const;
        void set_params(Params const& params);

    private:
        LPF m_lpf;
        Params m_params;
        bool m_has_ki = false;
        bool m_has_kd = false;
        T m_integrator = T();
        //T m_last_error = T();
        T m_last_input = T();
        T m_target = T();
        T m_input = T();
        T m_error = T();
        T m_output = T();
        T m_last_derivative = T();
        bool m_has_last_derivative = false;
        q::Clock::duration m_duration;
    };


    template<class T, class LPF>
    PID<T, LPF>::PID(LPF const& lpf)
        : m_lpf(lpf)
    {
    }

    template<class T, class LPF>
    PID<T, LPF>::PID(Params const& params, LPF const& lpf)
        : m_lpf(lpf)
    {
        set_params(params);
    }

    template<class T, class LPF>
    void PID<T, LPF>::set_target(T const& target)
    {
        m_target = target;
    }
    template<class T, class LPF>
    auto PID<T, LPF>::get_target() const -> T const&
    {
        return m_target;
    }
    template<class T, class LPF>
    void PID<T, LPF>::set_input(T const& input)
    {
        m_input = input;
    }
    template<class T, class LPF>
    auto PID<T, LPF>::get_input() const -> T const&
    {
        return m_input;
    }
    template<class T, class LPF>
    auto PID<T, LPF>::get_output() const -> T const&
    {
        return m_output;
    }

    template<class T, class LPF>
    T PID<T, LPF>::process(q::Clock::duration dt)
    {
// 		if (dt >= chrono::secondsf(1))
// 		{
// 			dt = 0;
//
// 			// if this PID hasn't been used for a full second then zero
// 			// the intergator term. This prevents I buildup from a
// 			// previous fight mode from causing a massive return before
// 			// the integrator gets a chance to correct itself
// 			reset();
// 		}

        // Compute proportional component
        m_error = m_target - m_input;
        m_output = T();
        m_output += m_error * m_params.kp;

        auto dts = q::Seconds(dt).count();
        if (dts > 0)
        {
            // Compute integral component if time has elapsed
            if (m_has_ki)
            {
                m_integrator += (m_error * m_params.ki) * dts;
                if (math::is_positive(m_integrator))
                {
                    m_integrator = math::min(m_integrator, m_params.max);
                }
                else
                {
                    m_integrator = math::max(m_integrator, -m_params.max);
                }
                m_output += m_integrator;
            }

            // Compute derivative component if time has elapsed
            if (m_has_kd)
            {
                float derivative;

                if (!m_has_last_derivative)
                {
                    // we've just done a reset, suppress the first derivative
                    // term as we don't want a sudden change in input to cause
                    // a large D output change
                    derivative = 0;
                    m_has_last_derivative = true;
                }
                else
                {
                    //derivative = (m_error - m_last_error) / dt.count;
                    derivative = (m_input - m_last_input) / dts;
                }

                derivative = m_lpf.process(derivative);

                // 			// discrete low pass filter, cuts out the
                // 			// high frequency noise that can drive the controller crazy
                // 			float RC = 1 / (2 * PI*_fCut);
                // 			derivative = m_last_derivative +
                // 				((delta_time / (RC + delta_time)) *
                // 				(derivative - _last_derivative));

                // update state
                m_last_derivative = derivative;

                // add in derivative component
                //m_output += m_params.kd * derivative;
                m_output -= m_params.kd * derivative;
            }
        }
        //m_last_error = error;
        m_last_input = m_input;

        if (math::is_positive(m_output))
        {
            m_output = math::min(m_output, m_params.max);
        }
        else
        {
            m_output = math::max(m_output, -m_params.max);
        }
        return m_output;
    }

    template<class T, class LPF>
    void PID<T, LPF>::reset()
    {
        m_integrator = T();
        m_has_last_derivative = false;
        //m_last_error = T();
        m_last_input = T();
        m_error = T();
        m_target = T();
        m_input = T();
        m_output = T();
    }

    template<class T, class LPF>
    auto PID<T, LPF>::get_params() const -> Params const&
    {
        return m_params;
    }
    template<class T, class LPF>
    void PID<T, LPF>::set_params(Params const& params)
    {
        m_params = params;
        m_has_ki = !math::is_zero(params.ki);
        m_has_kd = !math::is_zero(params.kd);
    }

}


