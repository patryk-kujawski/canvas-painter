#pragma once
#include <vector>

namespace canvas_painter
{

	class point_range
	{
	protected:
		int start_index_ = 0;
		int end_index_ = 0;
	public:
		point_range(const int start_index, const int end_index) : start_index_(start_index), end_index_(end_index)
		{
			
		}

		int size() const
		{
			return end_index_ - start_index_ + 1;
		}
	};

	class horizontal_point_collection : public point_range
	{
		int y_index_;
	public:
		horizontal_point_collection(const int start_index, const int end_index, const int y_index) : point_range(start_index, end_index), y_index_(y_index)
		{
		}

		bool has_collision(const horizontal_point_collection& other, int& out_x) const
		{
			const int start = std::max(start_index_, other.start_index_);
			const int end = std::min(end_index_, other.end_index_);
			if(start <= end)
			{
				out_x = start;
				return true;
			}
			return false;
		}

		int begin() const
		{
			return start_index_;
		}

		int end() const
		{
			return end_index_;
		}

		int y_index() const
		{
			return y_index_;
		}
	};

	class vertical_point_collection : public point_range
	{
		std::vector<horizontal_point_collection> range_ = {};
	public:
		vertical_point_collection(const int start_index, const int end_index ) : point_range(start_index, end_index)
		{
			range_.reserve(size());
		}

		void emplace_back(const int start_index, const int end_index)
		{
			range_.emplace_back(start_index, end_index, range_.size() + start_index_);
		}

		bool has_collision(const vertical_point_collection& other, point& out_point)
		{
			int start = std::max(start_index_, other.start_index_);
			int end = std::min(end_index_, other.end_index_);
			for(int i = start; i <= end; i++)
			{
				int y = 0;
				bool collision = at(i).has_collision(other.at(i), y);
				if(collision)
				{
					out_point = point(i, y);
					return true;
				}
			}
			return false;
		}

		const horizontal_point_collection& at(const int index)  const
		{
			return range_.at(index - start_index_);
		}

		auto end()
		{
			return range_.end();
		}

		auto begin()
		{
			return range_.begin();
		}
	};
}
