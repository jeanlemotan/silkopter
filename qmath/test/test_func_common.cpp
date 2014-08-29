#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

// These are sample tests that show the different features of the framework

using namespace math;

namespace {


BOOST_AUTO_TEST_CASE(SQRT)
{
	{
		float a = 4.f;
		float b = sqrt(a);

		BOOST_CHECK(equals(b, 2.f));
	}

	{
		double a = 4.0;
		double b = sqrt(a);

		BOOST_CHECK(equals(b, 2.0));
	}

	{
		vec2f a(4.f, 16.f);
		vec2f b = sqrt(a);

		BOOST_CHECK(equals(b, vec2f(2.f, 4.f)));
	}

	{
		vec3f a(4.f, 16.f, 25.f);
		vec3f b = sqrt(a);

		BOOST_CHECK(equals(b, vec3f(2.f, 4.f, 5.f)));
	}

	{
		vec4f a(4.f, 16.f, 25.f, 36.f);
		vec4f b = sqrt(a);

		BOOST_CHECK(equals(b, vec4f(2.f, 4.f, 5.f ,6.f)));
	}
}

BOOST_AUTO_TEST_CASE(SQRT_FAST)
{
	{
		float a = 4.f;
		float b = sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, 2.f, 0.005f));
	}

	{
		double a = 4.0;
		double b = sqrt<double, fast>(a);

		BOOST_CHECK(equals(b, 2.0, 0.005));
	}

	{
		vec2f a(4.f, 16.f);
		vec2f b = sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, vec2f(2.f, 4.f), 0.005f));
	}

	{
		vec3f a(4.f, 16.f, 25.f);
		vec3f b = sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, vec3f(2.f, 4.f, 5.f), 0.005f));
	}

	{
		vec4f a(4.f, 16.f, 25.f, 36.f);
		vec4f b = sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, vec4f(2.f, 4.f, 5.f ,6.f), 0.005f));
	}
}

BOOST_AUTO_TEST_CASE(INV_SQRT)
{
	{
		float a = 4.f;
		float b = inv_sqrt(a);

		BOOST_CHECK(equals(b, 0.5f));
	}

	{
		double a = 4.0;
		double b = inv_sqrt(a);

		BOOST_CHECK(equals(b, 0.5));
	}

	{
		vec2f a(4.f, 16.f);
		vec2f b = inv_sqrt(a);

		BOOST_CHECK(equals(b, vec2f(0.5f, 0.25f)));
	}

	{
		vec3f a(4.f, 16.f, 25.f);
		vec3f b = inv_sqrt(a);

		BOOST_CHECK(equals(b, vec3f(0.5f, 0.25f, 0.2f)));
	}

	{
		vec4f a(4.f, 16.f, 25.f, 36.f);
		vec4f b = inv_sqrt(a);

		BOOST_CHECK(equals(b, vec4f(0.5f, 0.25f, 0.2f, 1.f / 6.f)));
	}
}

BOOST_AUTO_TEST_CASE(INV_SQRT_FAST)
{
	{
		float a = 4.f;
		float b = inv_sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, 0.5f, 0.001f));
	}

	{
		double a = 4.0;
		double b = inv_sqrt<double, fast>(a);

		BOOST_CHECK(equals(b, 0.5, 0.001));
	}

	{
		vec2f a(4.f, 16.f);
		vec2f b = inv_sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, vec2f(0.5f, 0.25f), 0.001f));
	}

	{
		vec3f a(4.f, 16.f, 25.f);
		vec3f b = inv_sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, vec3f(0.5f, 0.25f, 0.2f), 0.001f));
	}

	{
		vec4f a(4.f, 16.f, 25.f, 36.f);
		vec4f b = inv_sqrt<float, fast>(a);

		BOOST_CHECK(equals(b, vec4f(0.5f, 0.25f, 0.2f, 1.f / 6.f), 0.001f));
	}
}

BOOST_AUTO_TEST_CASE(POSITIVE_ZERO)
{
	{
		{
			std::stringstream str;
			str << -0.f;
			BOOST_CHECK(str.str() == "-0");
		}
		{
			std::stringstream str;
			str << positive_zero(-0.f);
			BOOST_CHECK(str.str() == "0");
		}
		{
			std::stringstream str;
			str << positive_zero(0.f);
			BOOST_CHECK(str.str() == "0");
		}
		{
			std::stringstream str;
			str << positive_zero(-7.f);
			BOOST_CHECK(str.str() == "-7");
		}
		{
			std::stringstream str;
			str << positive_zero(7.f);
			BOOST_CHECK(str.str() == "7");
		}
	}
}

BOOST_AUTO_TEST_CASE(ABS_SGN)
{
	float a = -1.f;
	float b = math::abs(a);

	BOOST_CHECK(equals(b, -a));
	BOOST_CHECK(sgn(a) == a);
	BOOST_CHECK(sgn(b) == b);

	double c = -12123412.187246;
	double d = abs(c);

	BOOST_CHECK(equals(d, -c));
	BOOST_CHECK(sgn(d) == 1);
	BOOST_CHECK(sgn(c) == -1);

	int e = -10;
	int f = abs(e);

	BOOST_CHECK(equals(f, -e));
	BOOST_CHECK(sgn(e) == -1);
	BOOST_CHECK(sgn(f) == 1);

	int64_t g = -129381094710288L;
	int64_t h = math::abs(g);

	BOOST_CHECK(equals(h, -g));
	BOOST_CHECK(sgn(g) == -1);
	BOOST_CHECK(sgn(h) == 1);

	vec2f a2(0.f, -1.f);
	vec2f b2 = abs(a2);

	BOOST_CHECK(equals(b2, -a2));
	BOOST_CHECK(sgn(a2) == a2);
	BOOST_CHECK(sgn(b2) == b2);

	vec3f a3(0.f, -1.f, -1.f);
	vec3f b3 = abs(a3);

	BOOST_CHECK(equals(b3, -a3));
	BOOST_CHECK(sgn(a3) == a3);
	BOOST_CHECK(sgn(b3) == b3);

	vec4f a4(0.f, -1.f, 0.0f, -1.f);
	vec4f b4 = abs(a4);

	BOOST_CHECK(equals(b4, -a4));
	BOOST_CHECK(sgn(a4) == a4);
	BOOST_CHECK(sgn(b4) == b4);
}





BOOST_AUTO_TEST_CASE(INVERSE_TRANSPOSE)
{
	{
		float a = 10.f;
		float b = inverse(a);
		BOOST_CHECK(equals(b, 0.1f, math::epsilon<float>()));
	}

	{
		double a = 10.f;
		double b = inverse(a);
		double c = 0.1;
		BOOST_CHECK(equals(b, c));
	}
	
	{
		mat3f a;
		a.set_row(0, vec3f(1.f, 1.f, 1.f));
		a.set_row(1, vec3f(2.f, 3.f, 2.f));
		a.set_row(2, vec3f(4.f, 5.f, 6.f));

		mat3f b = inverse(a);
		
		BOOST_CHECK(equals(b.get_row(0), vec3f(4.f, -0.5f, -0.5f), math::epsilon<float>()));
		BOOST_CHECK(equals(b.get_row(1), vec3f(-2.f, 1.f, 0.f), math::epsilon<float>()));
		BOOST_CHECK(equals(b.get_row(2), vec3f(-1.f, -0.5f, 0.5f), math::epsilon<float>()));

		mat3f c = transposed(a);

		BOOST_CHECK(equals(c.get_row(0), vec3f(1.f, 2.f, 4.f), math::epsilon<float>()));
		BOOST_CHECK(equals(c.get_row(1), vec3f(1.f, 3.f, 5.f), math::epsilon<float>()));
		BOOST_CHECK(equals(c.get_row(2), vec3f(1.f, 2.f, 6.f), math::epsilon<float>()));

	}

	{
		mat4f a;
		a.set_row(0, vec4f(1.f, 2.f, 2.f, 1.f));
		a.set_row(1, vec4f(2.f, 1.f, 2.f, 1.f));
		a.set_row(2, vec4f(2.f, 2.f, 1.f, 2.f));
		a.set_row(3, vec4f(1.f, 1.f, 2.f, 1.f));

		mat4f b = inverse(a);

		BOOST_CHECK(equals(b.get_row(0), vec4f(0.f, 1.f, 0.f, -1.f), math::epsilon<float>()));
		BOOST_CHECK(equals(b.get_row(1), vec4f(1.f, 0.f, 0.f, -1.f), math::epsilon<float>()));
		BOOST_CHECK(equals(b.get_row(2), vec4f(0.f, 0.f, -0.33333333f, 0.66666666f), math::epsilon<float>()));
		BOOST_CHECK(equals(b.get_row(3), vec4f(-1.f, -1.f, 0.66666666f, 1.66666666f), math::epsilon<float>()));

		mat4f c = transposed(a);

		BOOST_CHECK(equals(c.get_row(0), vec4f(1.f, 2.f, 2.f, 1.f), math::epsilon<float>()));
		BOOST_CHECK(equals(c.get_row(1), vec4f(2.f, 1.f, 2.f, 1.f), math::epsilon<float>()));
		BOOST_CHECK(equals(c.get_row(2), vec4f(2.f, 2.f, 1.f, 2.f), math::epsilon<float>()));
		BOOST_CHECK(equals(c.get_row(3), vec4f(1.f, 1.f, 2.f, 1.f), math::epsilon<float>()));
	}

	{
		quatf a(0,1,1,1);
		quatf b = inverse(a);
		quatf c(0,-1,-1,1);
		BOOST_CHECK(equals(b, c));
	}
}

BOOST_AUTO_TEST_CASE(NORMALIZE)
{
	{
		 vec2f c(3.f, 4.f);
		 vec2f d = normalized(c);
		 BOOST_CHECK(equals(d, vec2f(0.6f, 0.8f)));
	}
	{
		 vec3f c(0.f, 3.f, 4.f);
		 vec3f d = normalized(c);
		 BOOST_CHECK(equals(d, vec3f(0.0f, 0.6f, 0.8f)));
	}
	{
		 vec4f c(0.f, 3.f, 4.f, 0.f);
		 vec4f d = normalized(c);
		 BOOST_CHECK(equals(d, vec4f(0.0f, 0.6f, 0.8f, 0.0f)));
	}
	{
		 quatf a;
		 quatf b = normalized(a);
		 BOOST_CHECK(equals(b, quatf()));

		 //quatf c(kPI, kPI, kPI, 1.f);
		 //quatf d = normalized(c);
		 //BOOST_CHECK(equals(d, quatf()));
	}
}

BOOST_AUTO_TEST_CASE(NORMALIZE_FAST)
{
	{
// 		vec2f a;
// 		vec2f b = normalized<float, fast>(a);
// 		BOOST_CHECK(equals(b, vec2f(), 0.001f));

		vec2f c(3.f, 4.f);
		vec2f d = normalized<float, fast>(c);
		BOOST_CHECK(equals(d, vec2f(0.6f, 0.8f), 0.005f));
	}
	{
		vec3f c(0.f, 3.f, 4.f);
		vec3f d = normalized<float, fast>(c);
		BOOST_CHECK(equals(d, vec3f(0.0f, 0.6f, 0.8f), 0.005f));
	}
	{
		vec4f c(0.f, 3.f, 4.f, 0.f);
		vec4f d = normalized<float, fast>(c);
		BOOST_CHECK(equals(d, vec4f(0.0f, 0.6f, 0.8f, 0.0f), 0.005f));
	}
	{
		quatf a;
		quatf b = normalized<float, fast>(a);
		BOOST_CHECK(equals(b, quatf(), 0.005f));

		//quatf c(kPI, kPI, kPI, 1.f);
		//quatf d = normalized(c);
		//BOOST_CHECK(equals(d, quatf()));
	}
}

BOOST_AUTO_TEST_CASE(DOT)
{
	{
		 vec2f a(1.f, 2.f);
		 vec2f b(3.f, 4.f); 
		 float c = dot(a, b);
		 BOOST_CHECK(equals(c, 11.f));
	}
	{
		 vec3f a(1.f, 2.f, 3.f);
		 vec3f b(4.f, 5.f, 6.f);
		 float c = dot(a, b);
		 BOOST_CHECK(equals(c, 32.f));
	}
	{
		 vec4f a(1.f, 2.f, 3.f, 4.f);
		 vec4f b(5.f, 6.f, 7.f, 8.f);
		 float c = dot(a, b);
		 BOOST_CHECK(equals(c, 70.f));
	}	
}
BOOST_AUTO_TEST_CASE(CROSS)
{
	{
		 vec2f a;
		 vec2f b;
		 float c = cross(a, b);
		 BOOST_CHECK(equals(c, 0.f));
	}
	{
		 vec3f a;
		 vec3f b;
		 vec3f c = cross(a, b);
		 BOOST_CHECK(equals(b, vec3f()));
	}
	{
		 vec4f a;
		 vec4f b;
		 vec4f c; 
		 vec4f d = cross(a, b, c);
		 BOOST_CHECK(equals(d, vec4f()));
	}	
}

BOOST_AUTO_TEST_CASE(DISTANCE)
{
	{
		 vec2f a;
		 vec2f b;
		 float c = distance(a, b);
		 float d = distance_sq(a, b);
		 BOOST_CHECK(equals(c, 0.f, math::epsilon<float>()));
		 BOOST_CHECK(equals(d, 0.f, math::epsilon<float>()));
	}
	{
		vec2f a(0.32f, 0);
		vec2f b(0, 0.12f);
		float c = distance(a, b);
		float d = distance_sq(a, b);
		BOOST_CHECK(equals(c, 0.341760129f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.116799995f, math::epsilon<float>()));
	}
	{
		 vec3f a;
		 vec3f b;
		 float c = distance(a, b);
		 float d = distance_sq(a, b);
		 BOOST_CHECK(equals(c, 0.f, math::epsilon<float>()));
		 BOOST_CHECK(equals(d, 0.f, math::epsilon<float>()));
	}
	{
		vec3f a(0.32f, 0, 0);
		vec3f b(0, 0.12f, 0);
		float c = distance(a, b);
		float d = distance_sq(a, b);
		BOOST_CHECK(equals(c, 0.341760129f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.116799995f, math::epsilon<float>()));
	}
	{
		 vec4f a;
		 vec4f b;
		 float c = distance(a, b);
		 float d = distance_sq(a, b);
		 BOOST_CHECK(equals(c, 0.f, math::epsilon<float>()));
		 BOOST_CHECK(equals(d, 0.f, math::epsilon<float>()));
	}
	{
		vec4f a(0.32f, 0, 0, 0);
		vec4f b(0, 0.12f, 0, 0);
		float c = distance(a, b);
		float d = distance_sq(a, b);
		BOOST_CHECK(equals(c, 0.341760129f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.116799995f, math::epsilon<float>()));
	}
}

BOOST_AUTO_TEST_CASE(LENGTH)
{
	{
		vec2f a;
		float c = length(a);
		float d = length_sq(a);
		BOOST_CHECK(equals(c, 0.f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.f, math::epsilon<float>()));
	}
	{
		vec2f a(0.32f, 0);
		float c = length(a);
		float d = length_sq(a);
		BOOST_CHECK(equals(c, 0.32f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.1024f, math::epsilon<float>()));
	}
	{
		vec3f a;
		float c = length(a);
		float d = length_sq(a);
		BOOST_CHECK(equals(c, 0.f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.f, math::epsilon<float>()));
	}
	{
		vec3f a(0.32f, 0, 0);
		float c = length(a);
		float d = length_sq(a);
		BOOST_CHECK(equals(c, 0.32f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.1024f, math::epsilon<float>()));
	}
	{
		vec4f a;
		float c = length(a);
		float d = length_sq(a);
		BOOST_CHECK(equals(c, 0.f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.f, math::epsilon<float>()));
	}
	{
		vec4f a(0.32f, 0, 0, 0);
		float c = length(a);
		float d = length_sq(a);
		BOOST_CHECK(equals(c, 0.32f, math::epsilon<float>()));
		BOOST_CHECK(equals(d, 0.1024f, math::epsilon<float>()));
	}
}


BOOST_AUTO_TEST_CASE(DISTANCE_FAST)
{
	{
		vec2f a;
		vec2f b;
		float c = distance<float, fast>(a, b);
		BOOST_CHECK(equals(c, 0.f, 0.005f));
	}
	{
		vec2f a(0.32f, 0);
		vec2f b(0, 0.12f);
		float c = distance<float, fast>(a, b);
		BOOST_CHECK(equals(c, 0.341760129f, 0.005f));
	}
	{
		vec3f a;
		vec3f b;
		float c = distance<float, fast>(a, b);
		BOOST_CHECK(equals(c, 0.f, 0.005f));
	}
	{
		vec3f a(0.32f, 0, 0);
		vec3f b(0, 0.12f, 0);
		float c = distance<float, fast>(a, b);
		BOOST_CHECK(equals(c, 0.341760129f, 0.005f));
	}
	{
		vec4f a;
		vec4f b;
		float c = distance<float, fast>(a, b);
		BOOST_CHECK(equals(c, 0.f, 0.005f));
	}
	{
		vec4f a(0.32f, 0, 0, 0);
		vec4f b(0, 0.12f, 0, 0);
		float c = distance<float, fast>(a, b);
		BOOST_CHECK(equals(c, 0.341760129f, 0.005f));
	}
}

BOOST_AUTO_TEST_CASE(LENGTH_FAST)
{
	{
		vec2f a;
		float c = length<float, fast>(a);
		BOOST_CHECK(equals(c, 0.f, 0.005f));
	}
	{
		vec2f a(0.32f, 0);
		float c = length<float, fast>(a);
		BOOST_CHECK(equals(c, 0.32f, 0.005f));
	}
	{
		vec3f a;
		float c = length<float, fast>(a);
		BOOST_CHECK(equals(c, 0.f, 0.005f));
	}
	{
		vec3f a(0.32f, 0, 0);
		float c = length<float, fast>(a);
		BOOST_CHECK(equals(c, 0.32f, 0.005f));
	}
	{
		vec4f a;
		float c = length<float, fast>(a);
		BOOST_CHECK(equals(c, 0.f, 0.005f));
	}
	{
		vec4f a(0.32f, 0, 0, 0);
		float c = length<float, fast>(a);
		BOOST_CHECK(equals(c, 0.32f, 0.005f));
	}
}


}
