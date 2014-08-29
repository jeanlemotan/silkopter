#pragma once

namespace q
{
namespace tween
{

	struct Tween
	{
	};

	template<typename T>
	struct Linear : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			return val;
		}
	};
	//Quadratic
	template<typename T>
	struct Quadratic_Ease_Out : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			T invVal = T(1) - val;
			invVal *= invVal; //this is invVal^2
			return T(1) - invVal;
		}
	};
	template<typename T>
	struct Quadratic_Ease_In : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			return val*val;
		}
	};
	template<typename T>
	struct Quadratic_Ease_In_Out : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);

			val *= T(2);
			if (val < T(1))
			{
				return T(0.5) * val * val;
			}
			else
			{
				val -= T(2);
				return T(-0.5) * (val * val - T(2));
			}
		}
	};
	//Quartic
	template<typename T>
	struct Quartic_Ease_Out : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			T invVal = T(1) - val;
			invVal *= invVal; //this is invVal^2
			invVal *= invVal; //this is invVal^4
			return T(1) - invVal;
		}
	};
	template<typename T>
	struct Quartic_Ease_In : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			T aVal = val * val; // val^2
			return aVal * aVal; // val^4
		}
	};
	template<typename T>
	struct Quartic_Ease_In_Out : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			val *= T(2);
			if (val < T(1))
			{
				T aVal = val * val;
				aVal *= aVal; // val^4
				return T(0.5) * aVal;
			}
			else
			{
				val -= T(2);
				T aVal = val * val;
				aVal *= aVal; // val^4
				return -T(0.5) * (aVal - T(2));
			}
		}
	};
	//Octic
	template<typename T>
	struct Octic_Ease_Out : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			T invVal = T(1) - val;
			invVal *= invVal; //this is invVal^2
			invVal *= invVal; //this is invVal^4
			invVal *= invVal; //this is invVal^8
			return T(1) - invVal;
		}
	};
	template<typename T>
	struct Octic_Ease_In : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			T aVal = val;
			aVal *= aVal; //this is aVal^2
			aVal *= aVal; //this is aVal^4
			aVal *= aVal; //this is aVal^8
			return aVal;
		}
	};
	template<typename T>
	struct Octic_Ease_In_Out : public Tween
	{
		typedef T value_t;
		T operator() (T val)
		{
			QASSERT(val >= 0 && val <= 1);
			val *= T(2);
			if (val < T(1))
			{
				T aVal = val * val;
				aVal *= aVal; // val^4
				aVal *= aVal; // val^8
				return T(0.5) * aVal;
			}
			else
			{
				val -= T(2);
				T aVal = val * val;
				aVal *= aVal; // val^4
				aVal *= aVal; // val^8
				return -T(0.5) * (aVal - T(2));
			}
		}
	};
} //tween
} //q

