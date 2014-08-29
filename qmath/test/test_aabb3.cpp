#include "stdafx.h"
#include "qmath.h"
#include "config.h"

// each test module could contain no more then one 'main' file with init function defined
// alternatively you could define init function yourself
#include <boost/test/unit_test.hpp>

using namespace math;

namespace
{

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB)
{
	//test default constructor
	aabb3f my_box;
	vec3f minus_one(-1.0f, -1.0f, -1.0f);
	vec3f one(1.0f, 1.0f, 1.0f);

	BOOST_CHECK(my_box.minPoint == minus_one && my_box.maxPoint == one);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_MIN_MAX)
{
	//test 2 vec constructor
	vec3f min(1.0f, 2.0f, 3.0f);
	vec3f max(4.0f, 5.0f, 6.0f);

	aabb3f box(min, max);

	BOOST_CHECK(
			box.minPoint == min &&
			box.maxPoint == max
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_MIN)
{
	//test 1 vec constructor
	vec3f min(1.0f, 2.0f, 3.0f);

	aabb3f box(min);

	BOOST_CHECK(
			box.minPoint == min &&
			box.maxPoint == min
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RESET_MIN)
{
	//test reset
	aabb3f box;

	box.reset(1.0f, 2.0f, 3.0f);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.minPoint.z == 3.0f &&
			box.maxPoint.x == 1.0f &&
			box.maxPoint.y == 2.0f &&
			box.maxPoint.z == 3.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RESET_BOX)
{
	//test reset
	aabb3f initial_box(1.0, 2.0, 3.0, 1.0, 2.0, 3.0);
	aabb3f box;

	box.reset(initial_box);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.minPoint.z == 3.0f &&
			box.maxPoint.x == 1.0f &&
			box.maxPoint.y == 2.0f &&
			box.maxPoint.z == 3.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RESET_MIN_MAX)
{
	//test reset
	vec3f min(1.0f, 2.0f, 3.0f);
	vec3f max(4.0f, 5.0f, 6.0f);

	aabb3f box;

	box.reset(min, max);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.minPoint.z == 3.0f &&
			box.maxPoint.x == 4.0f &&
			box.maxPoint.y == 5.0f &&
			box.maxPoint.z == 6.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RESET_VEC)
{
	//test reset
	vec3f min(1.0, 2.0, 3.0);
	aabb3f box;

	box.reset(min);

	BOOST_CHECK(
			box.minPoint.x == 1.0f &&
			box.minPoint.y == 2.0f &&
			box.minPoint.z == 3.0f &&
			box.maxPoint.x == 1.0f &&
			box.maxPoint.y == 2.0f &&
			box.maxPoint.z == 3.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_EXTENT)
{
	//test extent
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	vec3f ext = box.get_extent();
	BOOST_CHECK(
			ext.x == 3.0f &&
			ext.y == 3.0f &&
			ext.z == 3.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RADIUS)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float rad = box.get_radius();
	BOOST_CHECK(
			math::equals(rad,2.5980761f)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RADIUS_FAST)
{

	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float rad = box.get_radius_fast();
	BOOST_CHECK(
			math::equals(rad, 2.5980761f, 0.001f)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_RADIUS_SQ)
{

	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float rad = box.get_radius_sq();
	BOOST_CHECK(
			rad == 6.75f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_INNER_RADIUS)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float rad = box.get_inner_radius();
	BOOST_CHECK(
			 rad == 1.5f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_INNER_RADIUS_SQ)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float rad = box.get_inner_radius_sq();
	BOOST_CHECK(
			rad == 2.25f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_GET_EDGES)
{

	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	vec3f edges[8];
	box.get_edges(edges);

	vec3f e0(1.0, 2.0, 3.0);
	vec3f e1(1.0, 5.0, 3.0);
	vec3f e2(1.0, 2.0, 6.0);
	vec3f e3(1.0, 5.0, 6.0);
	vec3f e4(4.0, 2.0, 3.0);
	vec3f e5(4.0, 5.0, 3.0);
	vec3f e6(4.0, 2.0, 6.0);
	vec3f e7(4.0, 5.0, 6.0);

	BOOST_CHECK(
			edges[0] == e0 &&
			edges[1] == e1 &&
			edges[2] == e2 &&
			edges[3] == e3 &&
			edges[4] == e4 &&
			edges[5] == e5 &&
			edges[6] == e6 &&
			edges[7] == e7
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_IS_EMPTY)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(1.0, 2.0, 3.0));
	bool empty = box.is_empty();
	BOOST_CHECK(
			empty
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_REPAIR)
{
	vec3f min(4.0, 5.0, 6.0);
	vec3f max(1.0, 2.0, 3.0);
	aabb3f box(min, max);
	box.repair();
	BOOST_CHECK(
			box.minPoint == max &&
			box.maxPoint == min
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_GET_INTERPOLATED)
{
	aabb3f box1(vec3f(-4.0, -5.0, -6.0), vec3f(-1.0, -2.0, -3.0));
	aabb3f box2(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	aabb3f box = box1.get_interpolaed(box2, 0.5f);
	vec3f middle(1.5, 1.5, 1.5);

	BOOST_CHECK(
			box.minPoint == -middle &&
			box.maxPoint == middle
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_GET_CENTER)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	vec3f center = box.get_center();
	BOOST_CHECK(
			center.x == 2.5f &&
			center.y == 3.5f &&
			center.z == 4.5f
		);
}
///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_GET_VOLUME)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float volume = box.get_volume();
	BOOST_CHECK(
			volume == 27.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_GET_AREA)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	float area = box.get_area();
	BOOST_CHECK(
			area == 54.0f
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_IS_POINT_INSIDE)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	vec3f center(2.0, 3.0, 4.0);

	BOOST_CHECK(
			box.is_point_inside(center)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_IS_POINT_TOTAL_INSIDE)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	vec3f center(2.0, 3.0, 4.0);
	BOOST_CHECK(
			box.is_point_totally_inside(center)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_IS_FULL_INSIDE)
{
	aabb3f box(vec3f(1.0, 1.0, 1.0), vec3f(4.0, 4.0, 4.0));
	aabb3f box2(vec3f(2.0, 2.0, 2.0), vec3f(3.0, 3.0, 3.0));
	BOOST_CHECK(
			box2.is_fully_inside(box)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_ADD_INTERNAL_POINT_VEC)
{
	vec3f zero(0.0 ,0.0 ,0.0);
	vec3f min(1.0, 2.0, 3.0);
	vec3f max(4.0, 5.0, 6.0);

	aabb3f box(min, max);

	box.add_internal_point(zero);

	BOOST_CHECK(
			box.minPoint == zero &&
			box.maxPoint == max
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_ADD_INTERNAL_POINT)
{
	vec3f zero(0.0 ,0.0 ,0.0);
	vec3f min(1.0, 2.0, 3.0);
	vec3f max(4.0, 5.0, 6.0);

	aabb3f box(min, max);

	box.add_internal_point(0.0 ,0.0 ,0.0);

	BOOST_CHECK(
			box.minPoint == zero &&
			box.maxPoint == max
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_ADD_INTERNAL_BOX)
{
	vec3f min(1.0, 2.0, 3.0);
	vec3f max(4.0, 5.0, 6.0);

	vec3f min2(0.0, 0.0, 0.0);
	vec3f max2(2.0, 3.0, 4.0);

	aabb3f box(min, max);
	aabb3f box2(min2, max2);

	box.add_internal_box(box2);

	BOOST_CHECK(
			box.minPoint == min2 &&
			box.maxPoint == max
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_INTERSECTS_WITH_BOX)
{
	vec3f min(1.0, 2.0, 3.0);
	vec3f max(4.0, 5.0, 6.0);

	vec3f min2(0.0, 0.0, 0.0);
	vec3f max2(2.0, 3.0, 4.0);

	aabb3f box(min, max);
	aabb3f box2(min2, max2);

	BOOST_CHECK(
			box.intersect_with_box(box2)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_INTERSECTS_WITH_LINE_VEC)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));

	vec3f middle(3, 3, 3);
	vec3f vect(1.0, 0.0, 0.0);

	BOOST_CHECK(
			box.intersect_with_line(middle, vect, 2.5)
		);
}

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_INTERSECTS_WITH_LINE)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	line3f line(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	BOOST_CHECK(
			box.intersect_with_line(line)
		);
}


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_INTERSECTS_WITH_RAY)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	line3f line(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	vec3f outNear, outFar;
	BOOST_CHECK(
			box.intersect_with_ray(line, outNear, outFar)
		);
}


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_OPERATOR_EQUALS)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	aabb3f box2(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	BOOST_CHECK(
			box == box2
		);
}


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(AABB_OPERATOR_NOT_EQUAL)
{
	aabb3f box(vec3f(1.0, 2.0, 3.0), vec3f(4.0, 5.0, 6.0));
	aabb3f box2(vec3f(1.0, 2.0, 4.0), vec3f(4.0, 5.0, 6.0));
	BOOST_CHECK(
			box != box2
		);
}

///////////////////////////////////////////////////////////////////////////////
}
