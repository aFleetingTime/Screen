#include "Coordinate.h"
#include "Screen.h"

Coordinate::Coordinate(Pos r, Pos c) : x(c), y(r) { }
Coordinate Coordinate::operator++(int)
{
	Coordinate old = *this;
	++*this;
	return old;
}
Coordinate& Coordinate::operator++()
{
	if (++x >= Screen::getCol())
		++y, x = 0;
	if (y >= Screen::getRow()) y = 0;
	return *this;
}
Coordinate Coordinate::operator--(int)
{
	Coordinate old = *this;
	--*this;
	return old;
}
Coordinate& Coordinate::operator--()
{
	if (x == 0)
	{
		x = Screen::getCol();
		if (y == 0) y = Screen::getRow();
		--y;
	}
	--x;
	return *this;
}
Coordinate& Coordinate::operator+=(Pos n)
{
	x += n;
	y += x / Screen::getCol();
	x %= Screen::getCol();
	y %= Screen::getRow();
}
Coordinate& Coordinate::operator-=(Pos n)
{
}
