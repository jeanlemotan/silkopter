#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

using namespace math;

namespace {

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2)
{
	//test default constructor
	aabb2f my_box;
	vec2f minus_one(-1.0f, -1.0f);
	vec2f one(1.0f, 1.0f);

	BOOST_CHECK(my_box.minPoint == minus_one && my_box.maxPoint == one);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_MIN_MAX)
{
	//test 2 vec constructor
	vec2f min(1.0f, 2.0f);
	vec2f max(3.0f, 4.0f);

	aabb2f box(min, max);

	BOOST_CHECK(
			box.minPoint == min &&
			box.maxPoint == max
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_MIN)
{
	//test 1 vec constructor
	vec2f min(1.0f, 2.0f);
	
	aabb2f box(min);

	BOOST_CHECK(
			box.minPoint == min &&
			box.maxPoint == min
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RESET_MIN)
{
	//test reset
	aabb2f box;

	box.reset(1.0f, 2.0f);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.maxPoint.x == 1.0f &&
			box.maxPoint.y == 2.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RESET_BOX)
{
	//test reset
	aabb2f initial_box(1.0, 2.0, 1.0, 2.0);
	aabb2f box;

	box.reset(initial_box);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.maxPoint.x == 1.0f &&
			box.maxPoint.y == 2.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RESET_MIN_MAX)
{
	//test reset
	vec2f min(1.0f, 2.0f);
	vec2f max(3.0f, 4.0f);

	aabb2f box;

	box.reset(min, max);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.maxPoint.x == 3.0f &&
			box.maxPoint.y == 4.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RESET_VEC)
{
	//test reset
	vec2f min(1.0, 2.0);
	aabb2f box;

	box.reset(min);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.maxPoint.x == 1.0f &&
			box.maxPoint.y == 2.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_EXTENT)
{
	//test extent
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	vec2f ext = box.get_extent(); 
	BOOST_CHECK(
			ext.x == 2.0f &&
			ext.y == 2.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RADIUS)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	float rad = box.get_radius(); 
	BOOST_CHECK(
			math::equals(rad,1.4142135f)
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RADIUS_FAST)
{
	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	float rad = box.get_radius_fast(); 
	BOOST_CHECK(
			math::equals(rad,1.4142135f)
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_RADIUS_SQ)
{
	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	float rad = box.get_radius_sq(); 
	BOOST_CHECK(
			rad == 2.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_INNER_RADIUS)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	float rad = box.get_inner_radius(); 
	BOOST_CHECK(
			 rad == 1.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_INNER_RADIUS_SQ)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	float rad = box.get_inner_radius_sq(); 
	BOOST_CHECK(
			rad == 1.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_GET_EDGES)
{
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	vec2f edges[4];
	box.get_edges(edges);

	// not sure about this
	vec2f e0(3.0, 4.0);
	vec2f e1(3.0, 2.0);
	vec2f e2(1.0, 2.0);
	vec2f e3(1.0, 4.0);
	
	BOOST_CHECK(
			edges[0] == e0 &&
			edges[1] == e1 &&
			edges[2] == e2 &&
			edges[3] == e3
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_IS_EMPTY)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(1.0, 2.0));
	bool empty = box.is_empty(); 
	BOOST_CHECK(
			empty
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_REPAIR)
{	
	vec2f min(3.0, 4.0);
	vec2f max(1.0, 2.0);
	aabb2f box(min, max);
	box.repair(); 
	BOOST_CHECK(
			box.minPoint == max &&
			box.maxPoint == min 
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_GET_INTERPOLATED)
{
	aabb2f box1(vec2f(-3.0, -4.0), vec2f(-1.0, -2.0));
	aabb2f box2(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	aabb2f box = box1.get_interpolated(box2, 0.5f); 
	vec2f middle(1.0, 1.0);

	BOOST_CHECK(
			box.minPoint == -middle &&
			box.maxPoint == middle	
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_GET_CENTER)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	vec2f center = box.get_center(); 
	BOOST_CHECK(
			center.x == 2.0f &&
			center.y == 3.0f
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_GET_AREA)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	float area = box.get_area(); 
	BOOST_CHECK(
			area == 4.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_IS_POINT_INSIDE)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	vec2f center(2.0, 3.0);
	 
	BOOST_CHECK(
			box.is_point_inside(center)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_IS_POINT_TOTAL_INSIDE)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	vec2f center(2.0, 3.0);
	BOOST_CHECK(
			box.is_point_totally_inside(center)
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_ADD_INTERNAL_POINT_VEC)
{
	vec2f zero(0.0 ,0.0);
	vec2f min(1.0, 2.0);
	vec2f max(3.0, 4.0);

	aabb2f box(min, max);
	
	box.add_internal_point(zero);

	BOOST_CHECK(
			box.minPoint == zero &&
			box.maxPoint == max
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_ADD_INTERNAL_POINT)
{
	vec2f zero(0.0 ,0.0);
	vec2f min(1.0, 2.0);
	vec2f max(3.0, 4.0);

	aabb2f box(min, max);
	
	box.add_internal_point(0.0 ,0.0);

	BOOST_CHECK(
			box.minPoint == zero &&
			box.maxPoint == max
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_ADD_INTERNAL_BOX)
{	
	vec2f min(1.0, 2.0);
	vec2f max(3.0, 4.0);

	vec2f min2(0.0, 0.0);
	vec2f max2(2.0, 3.0);

	aabb2f box(min, max);
	aabb2f box2(min2, max2);

	box.add_internal_box(box2);

	BOOST_CHECK(
			box.minPoint == min2 &&
			box.maxPoint == max
		);	
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_INTERSECTS_WITH_BOX)
{
	vec2f min(1.0, 2.0);
	vec2f max(3.0, 4.0);

	vec2f min2(0.0, 0.0);
	vec2f max2(2.0, 3.0);

	aabb2f box(min, max);
	aabb2f box2(min2, max2);
	
	BOOST_CHECK(
			box.intersect_with_box(box2)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_INTERSECTS_WITH_LINE_VEC)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));

	vec2f middle(2, 2);
	vec2f vect(1.0, 0.0);

	BOOST_CHECK(
			box.intersect_with_line(middle, vect, 2.5)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_INTERSECTS_WITH_LINE)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	line2f line(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	BOOST_CHECK(
			box.intersect_with_line(line)
		);	
}


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_INTERSECTS_WITH_RAY)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	line2f line(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	vec2f outNear, outFar;
	BOOST_CHECK(
			box.intersect_with_ray(line, outNear, outFar)
		);
}


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_OPERATOR_EQUALS)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	aabb2f box2(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	BOOST_CHECK(
			box == box2
		);
}


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB2_OPERATOR_NOT_EQUAL)
{	
	aabb2f box(vec2f(1.0, 2.0), vec2f(3.0, 4.0));
	aabb2f box2(vec2f(1.0, 3.0), vec2f(3.0, 4.0));
	BOOST_CHECK(
			box != box2
		);
}

///////////////////////////////////////////////////////////////////////////////
}
