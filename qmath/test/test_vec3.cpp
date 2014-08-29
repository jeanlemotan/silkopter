#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>
				  
using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(VEC3)
{   
	vec3f a;

    BOOST_CHECK(
			a.x == 0.0 &&
			a.y == 0.0 &&
			a.z == 0.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_UNINIT)
{   
	vec3f a(vec3f::uninitialized);	
}

BOOST_AUTO_TEST_CASE(VEC3_FROMVALUES)
{   
	vec3f a(1.0, 2.0, 3.0);

    BOOST_CHECK(
			a.x == 1.0 &&
			a.y == 2.0 &&
			a.z == 3.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_FROMVALUE)
{   
	vec3f a(1.0);

    BOOST_CHECK(
			a.x == 1.0 &&
			a.y == 1.0 &&
			a.z == 1.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_COPY_CONSTRUCTOR)
{   
	vec3f a(1.0, 2.0, 3.0);
	vec3f b(a);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0 &&
			b.z == 3.0    
		);	
}


BOOST_AUTO_TEST_CASE(VEC3_CAST_VEC3U)
{   
	vec3u32 a(1, 2, 3);
	vec3f b(a);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0 &&
			b.z == 3.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_CAST_VEC2)
{   
	vec2f a(1.0, 2.0);
	vec3f b(a);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0 && 
			b.z == 0.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_CAST_VEC2_AND_SCALAR)
{   
	vec2f a(1.0, 2.0);
	vec3f b(a, 3.0f);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0 && 
			b.z == 3.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_CAST_VEC4)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	vec3f b(a);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0 &&
			b.z == 3.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_SET)
{   
	vec3f a(1.0, 2.0, 3.0);
	a.set(3.0 ,4.0, 5.0);

    BOOST_CHECK(
			a.x == 3.0 && 
			a.y == 4.0 && 
			a.z == 5.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_GET_LENGTH)
{   
	vec3f a(1.0, 2.0, 3.0);
	float len = length(a);

    BOOST_CHECK(
			equals(len, 3.7416575f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_GET_LENGTH_SQ)
{   
	vec3f a(1.0, 2.0, 3.0);
	float len = length_sq(a);

    BOOST_CHECK(
			equals(len, 14.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_SET_LENGTH)
{   
	vec3f a(1.0, 0.0, 0.0);
	a.set_length(25);

    BOOST_CHECK(
			equals(a.x, 25.0f) &&
			equals(a.y, 0.0f) &&
			equals(a.z, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_NORMALIZE)
{   
	vec3f a(3.0, 0.0, 0.0);
	vec3f n = math::normalized(a);

    BOOST_CHECK(
			equals(n.x, 1.0f) &&
			equals(n.y, 0.0f) &&
			equals(n.z, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_ACCES_OPERATOR)
{   
	vec3f a(1.0, 2.0, 3.0);
	
    BOOST_CHECK(
			a[0] == 1.0 &&
			a[1] == 2.0	&& 
			a[2] == 3.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_CONST_ACCES_OPERATOR)
{   
	vec3f a(1.0, 2.0, 3.0);
	const float x = a[0];
	const float y = a[1];
	const float z = a[2];

    BOOST_CHECK(
			x == 1.0 &&
			y == 2.0 &&
			z == 3.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_EQUAL_OPERATOR)
{   
	vec3f a(1.0, 2.0, 3.0);
	vec3f b(1.0, 2.0, 3.0);

    BOOST_CHECK(
			a == b
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_NOT_EQUAL_OPERATOR)
{   
	vec3f a(1.0, 2.0, 0.0);
	vec3f b(2.0, 3.0, 1.0);

    BOOST_CHECK(
			a != b
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MINUS)
{   
	vec3f a(1.0, 2.0, 3.0);
	vec3f b = -a;

    BOOST_CHECK(
			equals(b.x, -1.0f) &&
			equals(b.y, -2.0f) &&
			equals(b.z, -3.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_PLUS_SCALAR)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f c = a + 2.0f;

    BOOST_CHECK(
			equals(c.x, 3.0f) &&
			equals(c.y, 4.0f) && 
			equals(c.z, 5.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_PLUS_VEC)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f b(2.0, 3.0, 4.0);
	vec3f c = a + b;

    BOOST_CHECK(
			equals(c.x, 3.0f) &&
			equals(c.y, 5.0f) &&
			equals(c.z, 7.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MINUS_SCALAR)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f b = a - 2.0f;

    BOOST_CHECK(
			equals(b.x, -1.0f) &&
			equals(b.y, 0.0f) &&
			equals(b.z, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MINUS_VEC)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f b(2.0, 3.0, 4.0);
	vec3f c = b - a;

    BOOST_CHECK(
			equals(c.x, 1.0f) &&
			equals(c.y, 1.0f) &&
			equals(c.z, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MULTIPLY_SCALAR)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f c = a * 2.0f;

    BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 4.0f) &&
			equals(c.z, 6.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MULTIPLY_VEC)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f b(2.0, 3.0, 4.0);
	vec3f c = a * b;

    BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 6.0f) &&
			equals(c.z, 12.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_DIVIDE_SCALAR)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f c = a / 2.0f;

    BOOST_CHECK(
			equals(c.x, 0.5f) &&
			equals(c.y, 1.0f) &&
			equals(c.z, 1.5f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_DIVIDE_VEC3)
{   
	vec3f a(0.5, 2.0, 3.0);	
	vec3f b(1.0, 2.0, 2.0);	
	vec3f c = a / b;

    BOOST_CHECK(
			equals(c.x, 0.5f) &&
			equals(c.y, 1.0f) &&
			equals(c.z, 1.5f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_EQUALS)
{   
	vec3f a(1.0, 2.0, 3.0);	
	vec3f b;
	
	b = a;

    BOOST_CHECK(
			equals(b.x, 1.0f) &&
			equals(b.y, 2.0f) &&
			equals(b.z, 3.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_EQUALS_VEC3U)
{   
	vec3u32 a(1, 2, 3);	
	vec3f c(a);

    BOOST_CHECK(
			equals(c.x, 1.f) &&
			equals(c.y, 2.f) && 
			equals(c.z, 3.f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_EQUALS_VEC2)
{   
	vec2f b(2.0, 3.0);
	vec3f c;
	
	c = b;
    
	BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 3.0f) && 
			equals(c.z, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_EQUALS_VEC4)
{   
	vec4f b(2.0, 3.0, 4.0, 5.0f);
	vec3f c;
	
	c = b;
    
	BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 3.0f) &&
			equals(c.z, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_PLUS_EQUALS_SCALAR)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	
	a += 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 3.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 5.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_PLUS_EQUALS_VEC)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	vec3f b(1.0f, 2.0f, 3.0f);	
	
	a += b;
    
	BOOST_CHECK(
			equals(a.x, 2.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 6.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MINUS_EQUALS_SCALAR)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	
	a -= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, -1.0f) &&
			equals(a.y, 0.0f) &&
			equals(a.z, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MINUS_EQUALS_VEC)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	vec3f b(1.0f, 2.0f, 3.0f);	
	
	a -= b;
    
	BOOST_CHECK(
			equals(a.x, 0.0f) &&
			equals(a.y, 0.0f) &&
			equals(a.z, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MULTIPLY_EQUALS_SCALAR)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	
	a *= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 2.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 6.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_MULTIPLY_EQUALS_VEC)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	vec3f b(1.0f, 2.0f, 3.0f);	
	
	a *= b;
    
	BOOST_CHECK(
			equals(a.x, 1.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 9.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_DIVIDE_EQUALS_SCALAR)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	
	a /= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 0.5f) &&
			equals(a.y, 1.0f) &&
			equals(a.z, 1.5f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC3_OPERATOR_DIVIDE_EQUALS_VEC)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	vec3f b(1.0f, 2.0f, 3.0f);	
	
	a /= b;
    
	BOOST_CHECK(
			equals(a.x, 1.0f) &&
			equals(a.y, 1.0f) &&
			equals(a.z, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(SCALAR_OPERATOR_MULTIPLY_VEC3)
{   
	vec3f a(1.0f, 2.0f, 3.0f);
	vec3f b;

	b = 2.0f * a;
    
	BOOST_CHECK(
			equals(b.x, 2.0f) &&
			equals(b.y, 4.0f) && 
			equals(b.z, 6.0f)
		);	
}

}
