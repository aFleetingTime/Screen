#pragma once
#include <string>

enum ColorType : short {
	BLACK = 30, RED, GREEN, YELLOW, BLUE, PURPLE, DARKER, WHITE, A, B, C, D
};

class Color
{
public:
	constexpr Color(ColorType c = ColorType::BLACK) : color(c) { }
	std::string form();
	Color setColor(ColorType);
private:
	ColorType color;
};
