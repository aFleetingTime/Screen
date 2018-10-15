#pragma once

struct Coordinate
{
	using Pos = unsigned short;
	Coordinate(Pos y = 0, Pos x = 0);
	Coordinate operator++(int);
	Coordinate& operator++();
	Coordinate operator--(int);
	Coordinate& operator--();
	Coordinate& operator+=(Pos n);
	Coordinate& operator-=(Pos n);
	Pos x, y;
};
