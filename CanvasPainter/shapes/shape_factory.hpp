#pragma once
#include "shape.hpp"
#include "rectangle.hpp"
#include <memory>
#include "../change_request.hpp"
#include "circle.hpp"
#include "triangle.hpp"

namespace canvas_painter
{
	
	class shape_factory
	{
	
		template<class TShape>
		static std::shared_ptr<shapes::shape> create_impl(const change_request change_request)
		{
			auto shape = new TShape(change_request.new_color(),
				change_request.new_position().x(),
				change_request.new_position().y(),
				change_request.new_size());
			return std::shared_ptr<shapes::shape>(shape);
		}
	public:
		static std::shared_ptr<shapes::shape> create_shape(const change_request change_request)
		{
			switch(change_request.new_shape_type())
			{
			case shapes::shape_type::circle:
				return create_impl<shapes::circle>(change_request);
			case shapes::shape_type::triangle:
				return create_impl < shapes::triangle >(change_request);
			case shapes::shape_type::rectangle:
				return create_impl< shapes::rectangle>(change_request);
			default:
				throw std::invalid_argument("Cannot create shape of this type");
			}
		}
	};
}
