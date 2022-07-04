// CanvasPainter.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <map>
#include "point.hpp"
#include "canvas_descriptor.hpp"
#include "canvas.hpp"

namespace canvas_painter
{
	class canvas_accessor
	{
		std::map<size_t, canvas> _canvases;
		auto& find_canvas(const size_t canvas_id)
		{
			auto canvas_iterator = _canvases.find(canvas_id);
			if (canvas_iterator == _canvases.end())
			{
				throw std::invalid_argument("Unknown canvas id " + std::to_string(canvas_id));
			}
			return canvas_iterator->second;
		}
	public:
		canvas_descriptor create(int width, int height)
		{
			canvas canvas(width, height, new int[static_cast<size_t>(width)*height]);
			_canvases.emplace(canvas.descriptor().id(), canvas);
			return canvas.descriptor();
		}

		void destroy(const size_t canvas_id)
		{		
			auto& canvas = find_canvas(canvas_id);
			_canvases.erase(canvas.descriptor().id());
			delete canvas.descriptor().data();
		}

		void batch_request(const size_t canvas_id, const int shape_id, change_request req)
		{
			find_canvas(canvas_id).batch_update(shape_id, req);
		}

		void batch_request(const size_t canvas_id, std::shared_ptr<shapes::shape>& shape)
		{
			find_canvas(canvas_id).batch_add(shape);
		}

		
		
		void update_shape(const size_t canvas_id,  point new_coordinates,const int new_size, const int new_color)
		{
			find_canvas(canvas_id).update_shape(change_request(new_color, shapes::shape_type::none, new_coordinates, new_size));
		}
		
		void update_shape(const size_t canvas_id,  change_request change_request)
		{
			find_canvas(canvas_id).update_shape(change_request);
		}
		void apply_batch(const size_t canvas_id)
		{
			find_canvas(canvas_id).apply_batch();
		}
		void cancel_batch(const size_t canvas_id)
		{
			find_canvas(canvas_id).cancel_batch();
		}
		int select_shape(const size_t canvas_id, const int x,const int y)
		{
			return find_canvas(canvas_id).select_shape(x, y);
		}
		void unselect_shape(const size_t canvas_id)
		{
			find_canvas(canvas_id).unselect_shape();
		}

		template<class TShape>
		void add_shape(const size_t canvas_id, const TShape& shape)
		{
			find_canvas(canvas_id).add_shape(shape);
		}

	};
}
