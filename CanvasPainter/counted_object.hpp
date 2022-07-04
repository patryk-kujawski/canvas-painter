#pragma once
namespace canvas_painter
{

	template<typename counted_type>
	class counted_object
	{
		static size_t _count;
	public:
		static size_t how_many()
		{
			return _count;
		}


		counted_object() { ++_count; }
		counted_object(const counted_object&) { ++_count; }
	};

	template<typename counted_type>
	size_t counted_object<counted_type>::_count = 0;

}