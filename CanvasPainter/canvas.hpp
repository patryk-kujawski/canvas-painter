#pragma once
#include "shapes/shape.hpp"
#include "canvas_descriptor.hpp"
#include "counted_object.hpp"
#include "change_request.hpp"
#include <map>
#include <memory>
#include "shapes/shape_factory.hpp"
#include "exceptions/collision_exception.hpp"
#include <algorithm>
#include <set>

namespace canvas_painter
{


	class canvas : private counted_object<canvas>
	{

		canvas_descriptor descriptor_;
		std::map<int, std::shared_ptr<shapes::shape>> shapes_{};
		std::vector<int> shape_ids_;
		int next_shape_id_;
		std::vector<std::pair<int, std::shared_ptr<shapes::shape>>> batched_requests_{};
		int selected_shape_;
		std::shared_ptr<shapes::shape> remove_shape(int shape_id)
		{
			auto iter = shapes_.find(shape_id);
			if (iter == shapes_.end())
			{
				throw std::exception(("No element " + std::to_string(shape_id) + " present").c_str());
			}

			undraw_impl(iter->first, iter->second);

			return iter->second;
		}

		template<class iterator_function>
		void iterate_over_points(iterator_function functor, std::shared_ptr<shapes::shape> shape)
		{
			const int width = descriptor_.width();
			const int height = descriptor_.height();
			auto points = shape->points();
			for (auto&& row : points)
			{
				if (row.y_index() < 0 || row.y_index() >= height) continue;;
				for (int i = std::max(0, row.begin()); i <= std::min(width - 1, row.end()); i++)
				{
					size_t index = static_cast<size_t>(width) * row.y_index() + i;
					if (functor(index)) return;
				}

			}
		}

		void undraw_impl(const int shape_id, std::shared_ptr<shapes::shape> shape)
		{
			auto data = descriptor_.data();
			auto points = shape->points();
			const auto lambda = [&](const size_t& index)
			{
				data[index] = 0;
				shape_ids_[index] = 0;
				return false;
			};
			iterate_over_points(lambda, shape);
		}

		void draw_impl(const int shape_id, std::shared_ptr<shapes::shape> shape)
		{
			auto data = descriptor_.data();
			auto points = shape->points();
			const auto lambda = [&](const size_t& index)
			{
				data[index] = shape->color();
				shape_ids_[index] = shape_id;
				return false;
			};
			iterate_over_points(lambda, shape);
		}

		void redraw_shape(const int shape_id, std::shared_ptr<shapes::shape> shape)
		{
			shapes_.emplace(shape_id, shape);
			draw_impl(shape_id, shape);
		}

		int add_shape_impl(const std::shared_ptr<shapes::shape> add_this)
		{
			const int result = next_shape_id_;
			next_shape_id_++;
			redraw_shape(result, add_this);

			return result;
		}

		bool check_current_collisions(std::shared_ptr<shapes::shape> add_this, size_t& out_index)
		{
			auto points = add_this->points();
			bool result = false;
			const auto lambda = [&](const size_t& index)
			{
				if (shape_ids_[index] > 0)
				{
					out_index = index;
					result = true;
					return true;
				}
				out_index = -1;
				return false;
			};

			iterate_over_points(lambda, add_this);
			return result;
		}

		void check_batch_collisions(int id, std::shared_ptr<shapes::shape> add_this)
		{
			std::set<int> checked;

			for (auto i = batched_requests_.rbegin();
				i != batched_requests_.rend(); ++i) {
				if (checked.find(i->first) == checked.end() && i->first != id)
				{
					point point(0, 0);
					if (add_this->points().has_collision(i->second->points(), point))
					{
						throw exceptions::collision_exception(point.x(), point.y(), i->second->type(), add_this->type());
					}
					checked.insert(i->first);
				}
			}
			for (auto& i : shapes_)
			{
				if (checked.find(i.first) == checked.end() && i.first != id)
				{
					point point(0, 0);
					if (add_this->points().has_collision(i.second->points(), point))
					{
						throw exceptions::collision_exception(point.x(), point.y(), i.second->type(), add_this->type());
					}
					checked.insert(i.first);
				}
			}

		}

		point index_to_point(const size_t index) const
		{
			return point(index % descriptor_.width(), index / descriptor_.height());
		}

		void throw_collision(std::shared_ptr<shapes::shape> add_this, size_t index)
		{
			auto point = index_to_point(index);
			throw exceptions::collision_exception(point.x(), point.y(), add_this->type(), shapes_.at(shape_ids_[index])->type());
		}

		void update_shape_impl(int shape_id, std::shared_ptr<shapes::shape> backup, std::shared_ptr<shapes::shape> created)
		{

			size_t index = 0;
			const bool collisions = check_current_collisions(created, index);
			if (collisions)
			{
				redraw_shape(shape_id, backup);
				throw_collision(created, index);
			}
			else
			{
				redraw_shape(shape_id, created);
			}
		}

	public:
		using counted_object<canvas>::how_many;

		template<class TShape>
		int add_shape(const TShape& add_this)
		{
			TShape* copy = new TShape(add_this);
			const std::shared_ptr<shapes::shape> ptr(copy);
			size_t index = 0;
			const auto collisions = check_current_collisions(ptr, index);
			if (collisions)
			{
				throw_collision(ptr, index);

			}


			return add_shape_impl(ptr);
		}

		void batch_add(std::shared_ptr<shapes::shape> add_this)
		{
			int id = -1 * (1 + static_cast<int>(batched_requests_.size()));
			check_batch_collisions(id, add_this);
			batched_requests_.push_back(std::make_pair(id, add_this));
		}

		void batch_add(change_request add_this)
		{
			batch_add(shape_factory::create_shape(add_this));
		}



		int add_shape(const change_request& add_this)
		{

			const auto ptr = shape_factory::create_shape(add_this);
			size_t index = 0;
			const auto collisions = check_current_collisions(ptr, index);
			if (collisions)
			{
				throw_collision(ptr, index);

			}


			return add_shape_impl(ptr);
		}

	

		void update_shape(change_request request)
		{
			const auto shape = remove_shape(selected_shape_);
			const auto updated = shape_factory::create_shape(request.with_type(shape->type()));
			update_shape_impl(selected_shape_, shape, updated);
		}

		void batch_update(int shape_id, const change_request& change_request)
		{
			const auto ptr = shape_factory::create_shape(change_request.with_type(shapes_[shape_id]->type()));
			check_batch_collisions(shape_id, ptr);
			batched_requests_.push_back(std::make_pair(shape_id, ptr));
		}

		void apply_batch()
		{
			for (auto&& request : batched_requests_)
			{
				if (request.first < 0)
				{
					add_shape_impl(request.second);
				}
				else
				{
					update_shape_impl(request.first, remove_shape(request.first), request.second);
				}
			}

			batched_requests_.clear();
		}

		void cancel_batch()
		{
			batched_requests_.clear();
		}

		int select_shape(int x, int y)
		{
			int id = shape_ids_[static_cast<std::vector<int, std::allocator<int>>::size_type>(y) * descriptor_.width() + x];
			if (id > 0)
			{
				selected_shape_ = id;
			}
			return id;

		}

		void unselect_shape()
		{
			selected_shape_ = 0;
		}

		canvas(const int width, const int height, int* array) : descriptor_(how_many(), array, width, height), next_shape_id_(1), selected_shape_(0)
		{
			shape_ids_.resize(static_cast<std::vector<int, std::allocator<int>>::size_type>(width) * height);
			std::memset(descriptor_.data(), 0, static_cast<size_t>(width) * height * sizeof(int));
		}

		canvas_descriptor descriptor() const
		{
			return descriptor_;
		}
	};

}
