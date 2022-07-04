#pragma once
namespace canvas_painter
{

	class canvas_descriptor
	{
	public:
		canvas_descriptor(const size_t canvas_id, int* canvas_data, const int width, const int height)
			: canvas_id_(canvas_id),
			array_(canvas_data),
			width_(width),
			height_(height)
		{
		}

		size_t id() const
		{
			return canvas_id_;
		}

		int width() const
		{
			return width_;
		}

		int height() const
		{
			return height_;
		}

		int* data() const
		{
			return array_;
		}


	protected:
		size_t canvas_id_;
		//naked pointer - easier to use if API is to be called from a dll for example
		int* array_;
		int width_;
		int height_;
	};

}