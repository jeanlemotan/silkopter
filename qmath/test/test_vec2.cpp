#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>
				  
using namespace math;

namespace {

BOOST_AUTO_TEST_CASE(VEC2)
{   
	vec2f a;

    BOOST_CHECK(
			a.x == 0.0 &&
			a.y == 0.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_UNINIT)
{   
	vec2f a(vec2f::uninitialized);
	// nothing to check here
}

BOOST_AUTO_TEST_CASE(VEC2_FROMVALUES)
{   
	vec2f a(1.0, 2.0);

    BOOST_CHECK(
			a.x == 1.0 &&
			a.y == 2.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_FROMVALUE)
{   
	vec2f a(1.0);

    BOOST_CHECK(
			a.x == 1.0 &&
			a.y == 1.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_COPY_CONSTRUCTOR)
{   
	vec2f a(1.0, 2.0);
	vec2f b(a);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0    
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_CAST_VEC2U)
{   
	vec2u32 a(1, 2);
	vec2f b(a);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_CAST_VEC3)
{   
	vec3f a(1.0, 2.0, 3.0);
	vec2f b(a);

    BOOST_CHECK(
			b.x == 1.0 && 
			b.y == 2.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_CAST_VEC4)
{   
	vec4f a(1.0, 2.0, 3.0, 4.0);
	vec2f b(a);

    BOOST_CHECK(
			b.x == 1.0 &&
			b.y == 2.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_SET)
{   
	vec2f a(1.0, 2.0);
	a.set(3.0 ,4.0);

    BOOST_CHECK(
			a.x == 3.0 && 
			a.y == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_GET_LENGTH)
{   
	vec2f a(1.0, 2.0);
	float len = length(a);

    BOOST_CHECK(
			equals(len, 2.2360680f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_GET_LENGTH_SQ)
{   
	vec2f a(1.0, 2.0);
	float len = length_sq(a);

    BOOST_CHECK(
			equals(len, 5.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_SET_LENGTH)
{   
	vec2f a(3.0, 4.0);
	a.set_length(5);

    BOOST_CHECK(
			a.x == 3.0 &&
			a.y == 4.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_NORMALIZE)
{   
	vec2f a(3.0, 4.0);
	vec2f n = a.normalize();

    BOOST_CHECK(
			equals(n.x, 0.6f) &&
			equals(n.y, 0.8f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_ACCES_OPERATOR)
{   
	vec2f a(1.0, 2.0);
	
    BOOST_CHECK(
			a[0] == 1.0 &&
			a[1] == 2.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_CONST_ACCES_OPERATOR)
{   
	vec2f a(1.0, 2.0);
	const float x = a[0];
	const float y = a[1]; 
 
	BOOST_CHECK(
			x == 1.0 &&
			y == 2.0
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_EQUAL_OPERATOR)
{   
	vec2f a(1.0, 2.0);
	vec2f b(1.0, 2.0);

    BOOST_CHECK(
			a == b
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_NOT_EQUAL_OPERATOR)
{   
	vec2f a(1.0, 2.0);
	vec2f b(2.0, 3.0);

    BOOST_CHECK(
			a != b
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MINUS)
{   
	vec2f a(1.0, 2.0);
	vec2f b = -a;

    BOOST_CHECK(
			equals(b.x, -1.0f) &&
			equals(b.y, -2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_PLUS_SCALAR)
{   
	vec2f a(1.0, 2.0);	
	vec2f c = a + 2.0f;

    BOOST_CHECK(
			equals(c.x, 3.0f) &&
			equals(c.y, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_PLUS_VEC)
{   
	vec2f a(1.0, 2.0);	
	vec2f b(2.0, 3.0);
	vec2f c = a + b;

    BOOST_CHECK(
			equals(c.x, 3.0f) &&
			equals(c.y, 5.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MINUS_SCALAR)
{   
	vec2f a(1.0, 2.0);	
	vec2f b = a - 2.0f;

    BOOST_CHECK(
			equals(b.x, -1.0f) &&
			equals(b.y, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MINUS_VEC)
{   
	vec2f a(1.0, 2.0);	
	vec2f b(2.0, 3.0);
	vec2f c = b - a;

    BOOST_CHECK(
			equals(c.x, 1.0f) &&
			equals(c.y, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MULTIPLY_SCALAR)
{   
	vec2f a(1.0, 2.0);	
	vec2f c = a * 2.0f;

    BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MULTIPLY_VEC)
{   
	vec2f a(1.0, 2.0);	
	vec2f b(2.0, 3.0);
	vec2f c = a * b;

    BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 6.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_DIVIDE_SCALAR)
{   
	vec2f a(1.0, 2.0);	
	vec2f c = a / 2.0f;

    BOOST_CHECK(
			equals(c.x, 0.5f) &&
			equals(c.y, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_EQUALS)
{   
	vec2f a(1.0, 2.0);	
	vec2f b;
	
	b = a;

    BOOST_CHECK(
			equals(b.x, 1.0f) &&
			equals(b.y, 2.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_EQUALS_VEC2U)
{   
	vec2u32 a(1, 2);	
	vec2f c(a);

    BOOST_CHECK(
			equals(c.x, 1.f) &&
			equals(c.y, 2.f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_EQUALS_VEC3)
{   
	vec3f b(2.0, 3.0, 4.0);
	vec2f c;
	
	c = b;
    
	BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 3.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_EQUALS_VEC4)
{   
	vec4f b(2.0, 3.0, 4.0, 5.0f);
	vec2f c;
	
	c = b;
    
	BOOST_CHECK(
			equals(c.x, 2.0f) &&
			equals(c.y, 3.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_PLUS_EQUALS_SCALAR)
{   
	vec2f a(1.0f, 2.0f);
	
	a += 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 3.0f) &&
			equals(a.y, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_PLUS_EQUALS_VEC)
{   
	vec2f a(1.0f, 2.0f);
	vec2f b(1.0f, 2.0f);	
	
	a += b;
    
	BOOST_CHECK(
			equals(a.x, 2.0f) &&
			equals(a.y, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MINUS_EQUALS_SCALAR)
{   
	vec2f a(1.0f, 2.0f);
	
	a -= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, -1.0f) &&
			equals(a.y, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MINUS_EQUALS_VEC)
{   
	vec2f a(1.0f, 2.0f);
	vec2f b(1.0f, 2.0f);	
	
	a -= b;
    
	BOOST_CHECK(
			equals(a.x, 0.0f) &&
			equals(a.y, 0.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MULTIPLY_EQUALS_SCALAR)
{   
	vec2f a(1.0f, 2.0f);
	
	a *= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 2.0f) &&
			equals(a.y, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_MULTIPLY_EQUALS_VEC)
{   
	vec2f a(1.0f, 2.0f);
	vec2f b(1.0f, 2.0f);	
	
	a *= b;
    
	BOOST_CHECK(
			equals(a.x, 1.0f) &&
			equals(a.y, 4.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_DIVIDE_EQUALS_SCALAR)
{   
	vec2f a(1.0f, 2.0f);
	
	a /= 2.0f;
    
	BOOST_CHECK(
			equals(a.x, 0.5f) &&
			equals(a.y, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(VEC2_OPERATOR_DIVIDE_EQUALS_VEC)
{   
	vec2f a(1.0f, 2.0f);
	vec2f b(1.0f, 2.0f);	
	
	a /= b;
    
	BOOST_CHECK(
			equals(a.x, 1.0f) &&
			equals(a.y, 1.0f)
		);	
}

BOOST_AUTO_TEST_CASE(SCALAR_OPERATOR_MULTIPLY_VEC2)
{   
	vec2f a(1.0f, 2.0f);
	vec2f b;

	b = 2.0f * a;
    
	BOOST_CHECK(
			equals(b.x, 2.0f) &&
			equals(b.y, 4.0f)
		);	
}

}
