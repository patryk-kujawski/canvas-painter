#pragma once
#include <vector>
#include "../point.hpp"
#include <stdexcept>
#include "../point_collection.hpp"

namespace canvas_painter
{
	namespace shapes
	{
		enum class shape_type
		{
			none = 0,
			circle = 1,
			rectangle = 2,
			triangle = 3
		};

		constexpr const char* describe_shape_type(const shape_type e)
		{
			switch (e)
			{

			case shape_type::circle:
				return "circle";
			case shape_type::rectangle:
				return "rectangle";
			case shape_type::triangle:
				return "triangle";
			default:
				return "none";
				
			}
		}

		class shape
		{
		protected:
			int color_;
			point position_;
			int size_;
			virtual vertical_point_collection points_impl() const = 0;
			vertical_point_collection saved_points;
		public:
			virtual ~shape() = default;

			shape(const int color, const int x, const int y, const int size) : color_(color), position_(x, y), size_(size), saved_points(1,0)
			{

			}

			vertical_point_collection points()
			{
				if (saved_points.size() == 0)
				{
					saved_points = points_impl();

				}

				return saved_points;
			}

			virtual shape_type type() const = 0;
			int size() const
			{
				return size_;
			}

			int color() const
			{
				return color_;
			}

			point position() const
			{
				return position_;
			}

		};
	}
}
