#pragma once

namespace canvas_painter
{
	class change_request
	{
		int new_color_;
		shapes::shape_type new_shape_type_;
		point new_position_;
		int new_size_;
	public:
		change_request(const int new_color, const shapes::shape_type new_shape_type, const point new_position, const int new_size)
		: new_color_(new_color),
		new_shape_type_(new_shape_type),
		new_position_(new_position),
		new_size_(new_size)
		{
			
		}
		int new_color() const
		{
			return new_color_;
		}



		shapes::shape_type new_shape_type() const
		{
			return new_shape_type_;
		}


		int new_size() const
		{
			return new_size_;
		}



		point new_position() const
		{
			return new_position_;
		}

		change_request with_type(shapes::shape_type type) const
		{
			return {new_color_, type, new_position_, new_size_};
		}
	};
}