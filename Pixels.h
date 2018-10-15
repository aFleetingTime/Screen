#pragma once
#include <iostream>
#include "Color.h"
#include "Coordinate.h"

class Screen;

class Pixels
{
	friend class Screen;
public:
	using Pos = unsigned short;
	Pixels();
	explicit Pixels(Coordinate coord, Color c = BLACK) : xy(coord), color(c) { }
	Pixels(Pos x, Pos y, Color c = BLACK) : xy(x, y), color(c) { }
	Pixels& operator=(char);
	Pixels& setColor(Color color) {
		this->color = color;
		return *this;
	}
	Coordinate getXy() const {
		return xy;
	}
	Color getColor() const {
		return color;
	}

private:
	const Coordinate xy;
	Color color;
	static Coordinate cur;
};
