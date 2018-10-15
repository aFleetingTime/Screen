#include "Color.h"

std::string Color::form() {
	return std::string("\033[") + std::to_string(color) + std::string(1, 'm');
}

Color Color::setColor(ColorType ct) {
	Color temp = *this;
	color = ct;
	return temp;
}
