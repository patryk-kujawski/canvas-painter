#pragma once
#include "shape.hpp"
#include <cmath>
#include "../point.hpp"

namespace canvas_painter
{

	inline double pi() noexcept { return std::atan(1) * 4; } 
	namespace shapes
	{
		class circle final : public shape
		{
			vertical_point_collection points_impl() const override
			{
				const int r_squared = size_ * size_;
				vertical_point_collection result(position_.y() - size_, position_.y() + size_);
				for (int i = -size_; i <= size_; i++)
				{
					const int y = i * i;
					int start_x = position_.x() + size_;
					int end_x = position_.x() - size_;
					for (int j = -size_; j <= size_; j++)
					{
						const int x = j*j;
						const int position_x = j + position_.x();
						if (y + x <= r_squared)
						{
							if(position_x < start_x)
							{
								start_x = position_x;
							}
							end_x = position_x;
						}
					}
					result.emplace_back(start_x, end_x);
				}
				return result;
			}
		public:


			shape_type type() const override
			{
				return shape_type::circle;
			}

			circle(const int color, const int x, const int y, const int size) : shape(color, x, y, size)
			{

			}
		};
	}
}