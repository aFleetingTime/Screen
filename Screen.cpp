#include "Screen.h"

std::unique_ptr<Screen> Screen::onlyPtr;
Coordinate Screen::screenSize;
std::ostream* Screen::output;

Screen& operator<<(Screen &screen, const std::string &s)
{
	screen << s;
	screen.cursor += s.size();
}

Screen::Screen(char ch, ColorType color) : screen(getRow() * getCol())
{
	onlyPtr.reset(this);
	clearScreen();
	allColor(color);
	fillScreen(ch);
	moveCursor(0, 0);
}

Screen& Screen::getScreen(Pos y, Pos x, std::ostream &os, char ch, ColorType color)
{
	if(!y || !x)
		throw ScreenError("y | x == 0");
	screenSize.y = y, screenSize.x = x;
	output = &os;
	if(!onlyPtr)
		new Screen(ch, color);
	return *onlyPtr;
}

Screen& Screen::resetScreen(Pos y, Pos x, std::ostream &os, char ch, ColorType color)
{
	if(!y || !x)
		throw ScreenError("y | x == 0");
	screenSize.y = y, screenSize.x = x;
	output = &os;
	new Screen(ch, color);
	return *onlyPtr;
}

void Screen::fillScreen(char ch)
{
	*output << "\033[?25l";
	for(auto &i : screen)
		i = ch;
	flushBuf();
}

Coordinate Screen::moveCursor(Direction d, Pos dis) {
	Coordinate old = cursor;
	switch(d)
	{
	case UP:
		if(dis > cursor.y)
			throw MoveOutOfRange("光标上移超出屏幕范围!");
		cursor.y -= dis;
		break;
	case DOWN:
		if(dis + cursor.y >= screen.size())
			throw MoveOutOfRange("光标下移超出屏幕范围!");
		cursor.y += dis;
		break;
	case LEFT:
		if(dis > cursor.x)
			throw MoveOutOfRange("光标左移超出屏幕范围!");
		cursor.x -= dis;
		break;
	case RIGHT:
		if(dis + cursor.x >= getCol())
			throw MoveOutOfRange("光标右移超出屏幕范围!");
		cursor.x += dis;
		break;
	}
	*output << "\033[s";
	*output << (std::string("\033[") + std::to_string(dis) + static_cast<char>(d));
	return old;
}

Coordinate Screen::moveCursor(Pos y, Pos x) {
	checkRange(y, x);
	Coordinate old = cursor;
	cursor.x = x, cursor.y = y;
	*output << (std::string("\033[") + std::to_string(y + 1) + ';' + std::to_string(x + 1) + 'H');
	return old;
}

Coordinate Screen::moveCursor(Coordinate coord) {
	checkRange(coord.y, coord.x);
	Coordinate old = cursor;
	cursor = coord;
	*output << (std::string("\033[") + std::to_string(coord.y + 1) + ';' + std::to_string(coord.x + 1) + 'H');
	return old;
}

Screen::Pos Screen::getRow() {
	return screenSize.y;
}
Screen::Pos Screen::getCol() {
	return screenSize.x;
}
