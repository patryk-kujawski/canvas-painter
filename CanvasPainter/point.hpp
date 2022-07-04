#pragma once
namespace canvas_painter
{
	class point
	{
		int x_;
		int y_;
	public:
		point(const int x, const int y) : x_(x), y_(y)
		{

		}

		point(const point& point, const int translate_x, const int translate_y) : x_(point.x_ + translate_x), y_(point.y_ + translate_y)
		{
			
		}

		int x() const
		{
			return x_;
		}

		int y() const
		{
			return y_;
		}

		bool operator==(const point& rhs) const
		{
			return rhs.x_ == x_ && rhs.y_ == y_;
		}

		point operator+(const point& rhs) const
		{
			return { rhs.x_ + x_, rhs.y_ + y_ };
		}


		
	};
}