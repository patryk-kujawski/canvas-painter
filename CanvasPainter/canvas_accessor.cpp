// CanvasPainter.cpp : Defines the entry point for the application.
//
#include "canvas_accessor.hpp"
#include "shapes/rectangle.hpp"
#include <iostream>
using namespace std;
void print_canvas(canvas_painter::canvas_descriptor descriptor)
{
	int width = descriptor.width();
	int height = descriptor.height();
	int* data = descriptor.data();
	cout << hex << uppercase;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << data[width * i + j];
		}
		cout << endl;
	}
}
canvas_painter::point build_point()
{
	int x, y;
	cout << "Input x" << endl;
	cin >> x;
	cout << "Input y" << endl;
	cin >> y;
	return { x, y };
}
canvas_painter::change_request build_change_request()
{
	int size, color;
	auto point = build_point();
	cout << "Input size" << endl;
	cin >> size;
	cout << "Input color" << endl;
	cin >> color;
	return { color, canvas_painter::shapes::shape_type::none, point, size };
}
void add_shape(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd)
{
	cout << "Select shape type" << endl;
	cout << "1 - circle" << endl;
	cout << "2 - rectangle" << endl;
	cout << "3 - triangle" << endl;
	int type;
	cin >> type;
	auto cr = build_change_request();
	acc.add_shape(cd.id(), cr.with_type(static_cast<canvas_painter::shapes::shape_type>(type)));
}

void batch_add_shape(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd)
{
	cout << "Select shape type" << endl;
	cout << "1 - circle" << endl;
	cout << "2 - rectangle" << endl;
	cout << "3 - triangle" << endl;
	int type;
	cin >> type;
	auto cr = build_change_request();
	auto shape = canvas_painter::shape_factory::create_shape(cr.with_type(static_cast<canvas_painter::shapes::shape_type>(type)));
	acc.batch_request(cd.id(), shape);
}

void batch_update_shape(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd, int id)
{
	auto cr = build_change_request();
	acc.batch_request(cd.id(), id, cr );
}

void update_shape(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd)
{
	auto cr = build_change_request();
	acc.update_shape(cd.id(), cr);
}

int select_shape(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd)
{
	auto point = build_point();
	return acc.select_shape(cd.id(), point.x(), point.y());
}

void begin_batch(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd)
{
	int shape_id = 0;
	while(true)
	{
		cout << "Select operation" << endl;
		cout << "0 - Add shape" << endl;
		cout << "1 - Select shape" << endl;
		cout << "2 - Update selected shape" << endl;
		cout << "3 - Execute batch" << endl;
		cout << "4 - cancel batch" << endl;
		int operation;
		cin >> operation;
		try
		{
			switch (operation)
			{
			case 0:
				batch_add_shape(acc, cd);
				break;
			case 1:
				shape_id = select_shape(acc, cd);
				cout << "Selected shape with id " << shape_id << endl;
				break;
			case 2:
				batch_update_shape(acc, cd, shape_id);
				break;
			
			case 3:
				acc.apply_batch(cd.id());
				return;
			case 4:
				acc.cancel_batch(cd.id());
				return;
			default:
				cout << "Unrecognisable option" << endl;
			}
	
		}
		catch (std::exception& ex)
		{
			cout << "Exception: " << ex.what() << endl;
		}
	}
}



void unselect_shape(canvas_painter::canvas_accessor& acc, canvas_painter::canvas_descriptor& cd)
{
	acc.unselect_shape(cd.id());
}

int main()
{
	int width, height;
	canvas_painter::canvas_accessor ca;
	cout << "Input canvas width: " << endl;
	cin >> width;
	cout << "Input canvas height: " << endl;
	cin >> height;
	auto canvas = ca.create(width, height);
	while (true)
	{
		int operation;
		cout << "Select operation" << endl;
		cout << "0 - Add shape" << endl;
		cout << "1 - Select shape" << endl;
		cout << "2 - Unselect shape" << endl;
		cout << "3 - Update selected shape" << endl;
		cout << "4 - Begin batch" << endl;
		cout << "5 - Finish" << endl;
		cin >> operation;
		try
		{
			switch (operation)
			{
			case 0:
				add_shape(ca, canvas);
				break;
			case 1:
				select_shape(ca, canvas);
				break;
			case 2:
				unselect_shape(ca, canvas);
				break;
			case 3:
				update_shape(ca, canvas);
				break;
			case 4:
				begin_batch(ca, canvas);
				break;
			case 5:
				return 0;
			default:
				cout << "Unrecognisable option" << endl;
			}
			print_canvas(canvas);
		}
		catch (std::exception& ex)
		{
			cout << "Exception: " << ex.what() << endl;
		}
	}

}
