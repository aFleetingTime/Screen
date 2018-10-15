#include "Pixels.h"
#include "Screen.h"

Coordinate Pixels::cur;

Pixels::Pixels() : xy(cur++), color(BLACK) {
	//*Screen::output << '1';
	//if (xy.x == Screen::getCol() - 1)
		//*Screen::output << '\n';
}

Pixels& Pixels::operator=(char ch)
{
	*Screen::output << "\033[s";
	auto old = Screen::onlyPtr->moveCursor(xy);
	*Screen::output << color.form() << ch << "\033[u";
	//Screen::onlyPtr->moveCursor(old);
	return *this;
}
