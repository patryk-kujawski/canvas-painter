#pragma once
#include <exception>
#include "../shapes/shape.hpp"
#include <string>

namespace canvas_painter
{
	namespace exceptions
	{
		class collision_exception : public std::exception
		{
		public:
			shapes::shape_type existing_type_;
			shapes::shape_type new_type_;
			int x_, y_;
			std::string description_;
			collision_exception(int x, int y, shapes::shape_type existing_type, shapes::shape_type new_type) : x_(x), y_(y), existing_type_(existing_type), new_type_(new_type)
			{
				description_ = std::string("Collision of ")
					+ shapes::describe_shape_type(new_type_)
					+ " with " + shapes::describe_shape_type(existing_type_)
					+ " at point ( " + std::to_string(x_) + "," + std::to_string(y_) + ")";
			}

			char const* what() const override
			{

				return description_.c_str();
	
			}
		};
	}
}
