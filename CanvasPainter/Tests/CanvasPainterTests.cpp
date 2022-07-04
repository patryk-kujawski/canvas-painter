#include "gtest/gtest.h"
#include "../shapes/shape_factory.hpp"
#include "../canvas.hpp"
TEST(ShapeFactoryTests, ThrowsOnNone) {
	// Expect two strings not to be equal.
	canvas_painter::change_request cr(0, canvas_painter::shapes::shape_type::none, canvas_painter::point::point(0,0), 10);
	
		EXPECT_ANY_THROW(canvas_painter::shape_factory::create_shape(cr));

	
	
}

TEST(CanvasTests, ThrowsOnCollisionWithNoBatch)
{
	int* p = new int[100 * 100];
	canvas_painter::canvas cs(100, 100, p);
	cs.add_shape({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,0), 10 });

	EXPECT_ANY_THROW(cs.add_shape({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,0), 10 }));
	delete[] p;
}

TEST(CanvasTests, ThrowsOnCollisionWithBatch)
{
	int* p = new int[100 * 100];
	canvas_painter::canvas cs(100, 100, p);
	cs.batch_add({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,0), 10 });

	EXPECT_ANY_THROW(cs.batch_add({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,0), 10 }));
	delete[] p;
}

TEST(CanvasTests, CanUpdateMultipleTimesInBatch)
{
	int* p = new int[100 * 100];
	canvas_painter::canvas cs(100, 100, p);
	int s = cs.add_shape({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,0), 10 });
	EXPECT_NO_THROW(cs.batch_update(s, { 13, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(5,0), 3 }));
	EXPECT_NO_THROW(cs.batch_update(s , { 12, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,5), 12 }));
	delete[] p;
}

TEST(CanvasTests, AddingShapeChangesArray)
{
	int* p = new int[100 * 100];
	canvas_painter::canvas cs(100, 100, p);
	for (int i = 0; i < 100 * 100; i++)
	{
		EXPECT_EQ(0,p[i]);
	}
	int s = cs.add_shape({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(0,0), 10 });
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			EXPECT_EQ(10, p[100 * i + j]);
		}
	}
}

TEST(CanvasTests, AddingShapeOutOfBoundsDoesntChangeArray)
{
	int* p = new int[100 * 100];
	canvas_painter::canvas cs(100, 100, p);
	for (int i = 0; i < 100 * 100; i++)
	{
		EXPECT_EQ(0, p[i]);
	}
	int s = cs.add_shape({ 10, canvas_painter::shapes::shape_type::rectangle, canvas_painter::point::point(-20,-20), 10 });
	for (int i = 0; i < 100 * 100; i++)
	{
		EXPECT_EQ(0, p[i]);
	}
}
