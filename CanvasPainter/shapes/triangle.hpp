#pragma once
#include "shape.hpp"
#include "../point.hpp"

namespace canvas_painter
{
	namespace shapes
	{
		class triangle final : public shape
		{
			vertical_point_collection points_impl() const override
			{
				vertical_point_collection result(position_.y(), position_.y() + size_);
				for (auto i = 0; i < size_; i++)
				{
					result.emplace_back(position_.x(), position_.x() + size_ - i);
				}
				return result;
			}
		public:


			shape_type type() const override
			{
				return shape_type::triangle;
			}

			triangle(const int color, const int x, const int y, const int size) : shape(color, x, y, size)
			{

			}
		};
	}
}