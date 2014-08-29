#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>
				  
using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(VEC4)
{   
	vec4f a;

    BOOST_CHECK(
			a.x == 0.0 &&
			a.y == 0.0 &&
			a.z == 0.0 &&
			a.w == 0.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_UNINIT)
{   
	vec4f a(vec4f::uninitialized);	
}

BOOST_AUTO_TEST_CASE(VEC4_FROMVALUES)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);

    BOOST_CHECK(
			a.x == 1.0 &&
			a.y == 2.0 &&
			a.z == 3.0 &&
			a.w == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_FROMVALUE)
{   
	vec4f a(1.0);

    BOOST_CHECK(
			a.x == 1.0 &&
			a.y == 1.0 &&
			a.z == 1.0 &&
			a.w == 1.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_COPY_CONSTRUCTOR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	vec4f b(a);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0 &&
			b.z == 3.0 &&   
			b.w == 4.0    
		);	
}


BOOST_AUTO_TEST_CASE(VEC4_CAST_VEC4U)
{   
	vec4u32 a(1, 2, 3, 4);
	vec4f b(a);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0 &&
			b.z == 3.0 &&
			b.w == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_CAST_VEC2)
{   
	vec2f a(1.0, 2.0);
	vec4f b(a);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0 && 
			b.z == 0.0 &&
			b.w == 0.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_CAST_VEC2_AND_SCALARS)
{   
	vec2f a(1.0, 2.0);
	vec4f b(a, 3.0f, 4.0f);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0 && 
			b.z == 3.0 &&
			b.w == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_CAST_VEC3)
{   
	vec3f a(1.0, 2.0, 3.0);
	vec4f b(a);
	vec4f c(a, 1.f);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0 &&
			b.z == 3.0 &&
		    b.w == 0.0
		);	
	BOOST_CHECK(
			c.x == 1.0 &&
			c.y == 2.0 &&
			c.z == 3.0 &&
			c.w == 1.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_CAST_VEC3_SCALAR)
{   
	vec3f a(1.0, 2.0, 3.0);
	vec4f b(a, 4.0f);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0 &&
			b.z == 3.0 &&
		    b.w == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_SET)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	a.set(3.0 ,4.0, 5.0, 6.0);

    BOOST_CHECK(
			a.x == 3.0 && 
			a.y == 4.0 && 
			a.z == 5.0 &&
			a.w == 6.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_GET_LENGTH)
{   
	vec4f a(1.0, 0.0, 0.0, 0.0);
	float len = length(a);

    BOOST_CHECK(
			equals(len, 1.f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_GET_LENGTH_SQ)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	float len = length_sq(a);

    BOOST_CHECK(
			equals(len, 30.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_SET_LENGTH)
{   
	vec4f a(1.0, 0.0, 0.0, 0.0);
	a.set_length(25);

    BOOST_CHECK(
			equals(a.x, 25.0f) &&
			equals(a.y, 0.0f) &&
			equals(a.z, 0.0f) &&
			equals(a.w, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_NORMALIZE)
{   
	vec4f a(3.0, 0.0, 0.0, 0.0);
	vec4f n = math::normalized(a);

    BOOST_CHECK(
			equals(n.x, 1.0f) &&
			equals(n.y, 0.0f) &&
			equals(n.z, 0.0f) &&
			equals(n.w, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_ACCES_OPERATOR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	
    BOOST_CHECK(
			a[0] == 1.0 &&
			a[1] == 2.0	&& 
			a[2] == 3.0 &&
			a[3] == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_CONST_ACCES_OPERATOR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	const float x = a[0];
	const float y = a[1];
	const float z = a[2];
	const float w = a[3];

    BOOST_CHECK(
			x == 1.0 &&
			y == 2.0 &&
			z == 3.0 &&
			w == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_EQUAL_OPERATOR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	vec4f b(1.0, 2.0, 3.0, 4.0);

    BOOST_CHECK(
			a == b
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_NOT_EQUAL_OPERATOR)
{   
	vec4f a(1.0, 2.0, 0.0, 0.0);
	vec4f b(2.0, 3.0, 1.0, 0.0);

    BOOST_CHECK(
			a != b
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MINUS)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	vec4f b = -a;

    BOOST_CHECK(
			equals(b.x, -1.0f) &&
			equals(b.y, -2.0f) &&
			equals(b.z, -3.0f) &&
			equals(b.w, -4.0f) 
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_PLUS_SCALAR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f c = a + 2.0f;

    BOOST_CHECK(
			equals(c.x, 3.0f) &&
			equals(c.y, 4.0f) && 
			equals(c.z, 5.0f) &&
			equals(c.w, 6.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_PLUS_VEC)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f b(2.0, 3.0, 4.0, 5.0);
	vec4f c = a + b;

    BOOST_CHECK(
			equals(c.x, 3.0f) &&
			equals(c.y, 5.0f) &&
			equals(c.z, 7.0f) &&
			equals(c.w, 9.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MINUS_SCALAR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f b = a - 2.0f;

    BOOST_CHECK(
			equals(b.x, -1.0f) &&
			equals(b.y, 0.0f) &&
			equals(b.z, 1.0f) &&
			equals(b.w, 2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MINUS_VEC)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f b(2.0, 3.0, 4.0, 5.0);
	vec4f c = b - a;

    BOOST_CHECK(
			equals(c.x, 1.0f) &&
			equals(c.y, 1.0f) &&
			equals(c.z, 1.0f) &&
			equals(c.w, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MULTIPLY_SCALAR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f c = a * 2.0f;

    BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 4.0f) &&
			equals(c.z, 6.0f) &&
			equals(c.w, 8.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MULTIPLY_VEC)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f b(2.0, 3.0, 4.0, 5.0);
	vec4f c = a * b;

    BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 6.0f) &&
			equals(c.z, 12.0f) &&
			equals(c.w, 20.0f)
		);				
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_DIVIDE_SCALAR)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f c = a / 2.0f;

    BOOST_CHECK(
			equals(c.x, 0.5f) &&
			equals(c.y, 1.0f) &&
			equals(c.z, 1.5f) &&
			equals(c.w, 2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_DIVIDE)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f b(2.0, 2.0, 2.0, 2.0);	

	vec4f c = a / b;

    BOOST_CHECK(
			equals(c.x, 0.5f) &&
			equals(c.y, 1.0f) &&
			equals(c.z, 1.5f) &&
			equals(c.w, 2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_EQUALS)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);	
	vec4f b;
	
	b = a;

    BOOST_CHECK(
			equals(b.x, 1.0f) &&
			equals(b.y, 2.0f) &&
			equals(b.z, 3.0f) &&
			equals(b.w, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_EQUALS_VEC4U)
{   
	vec4u32 a(1, 2, 3, 4);
	vec4f b(a);
	
	BOOST_CHECK(
			equals(b.x, 1.0f) &&
			equals(b.y, 2.0f) && 
			equals(b.z, 3.0f) &&
			equals(b.w, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_EQUALS_VEC2)
{   
	vec2f b(2.0, 3.0);
	vec4f c;
	
	c = b;
    
	BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 3.0f) &&
			equals(c.z, 0.0f) &&
			equals(c.w, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_EQUALS_VEC3)
{   
	vec3f b(2.0, 3.0, 4.0);
	vec4f c;
	
	c = b;
    
	BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 3.0f) &&
			equals(c.z, 4.0f) &&
			equals(c.w, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_PLUS_EQUALS_SCALAR)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	
	a += 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 3.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 5.0f) &&
			equals(a.w, 6.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_PLUS_EQUALS_VEC)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	vec4f b(1.0f, 2.0f, 3.0f, 4.0f);	
	
	a += b;
    
	BOOST_CHECK(
			equals(a.x, 2.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 6.0f) &&
			equals(a.w, 8.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MINUS_EQUALS_SCALAR)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	
	a -= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, -1.0f) &&
			equals(a.y, 0.0f) &&
			equals(a.z, 1.0f) &&
			equals(a.w, 2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MINUS_EQUALS_VEC)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	vec4f b(1.0f, 2.0f, 3.0f, 4.0f);	
	
	a -= b;
    
	BOOST_CHECK(
			equals(a.x, 0.0f) &&
			equals(a.y, 0.0f) &&
			equals(a.z, 0.0f) &&
			equals(a.w, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MULTIPLY_EQUALS_SCALAR)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	
	a *= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 2.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 6.0f) &&
			equals(a.w, 8.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_MULTIPLY_EQUALS_VEC)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	vec4f b(1.0f, 2.0f, 3.0f, 4.0f);	
	
	a *= b;
    
	BOOST_CHECK(
			equals(a.x, 1.0f) &&
			equals(a.y, 4.0f) &&
			equals(a.z, 9.0f) &&
			equals(a.w, 16.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_DIVIDE_EQUALS_SCALAR)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	
	a /= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 0.5f) &&
			equals(a.y, 1.0f) &&
			equals(a.z, 1.5f) &&
			equals(a.w, 2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC4_OPERATOR_DIVIDE_EQUALS_VEC)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	vec4f b(1.0f, 2.0f, 3.0f, 4.0f);	
	
	a /= b;
    
	BOOST_CHECK(
			equals(a.x, 1.0f) &&
			equals(a.y, 1.0f) &&
			equals(a.z, 1.0f) &&
			equals(a.w, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(SCALAR_OPERATOR_MULTIPLY_VEC4)
{   
	vec4f a(1.0f, 2.0f, 3.0f, 4.0f);
	vec4f b;

	b = 2.0f * a;
    
	BOOST_CHECK(
			equals(b.x, 2.0f) &&
			equals(b.y, 4.0f) && 
			equals(b.z, 6.0f) &&
			equals(b.w, 8.0f) 
		);	
}

}
