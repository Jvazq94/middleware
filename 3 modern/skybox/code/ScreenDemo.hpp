#pragma once
#include "View.hpp"
#include <nanogui\nanogui.h>
using namespace std;
namespace example {
	class ScreenDemo : public nanogui::Screen {
	private:
		shared_ptr<View>  ventana;

	public:

		ScreenDemo(string name, int width, int height) : nanogui::Screen(Eigen::Vector2i(width, height), "Ventana") { ventana.reset(new View(width, height)); }

		void drawContents() {
			using namespace nanogui;
			ventana->render();

		}

		shared_ptr<View>   getView() { return ventana; }
	};
}