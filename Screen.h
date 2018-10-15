#pragma once
#include <valarray>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include "Coordinate.h"
#include "Color.h"
#include "Pixels.h"
#include "ScreenException.h"

enum Direction : char {
	UP = 'A', DOWN = 'B', RIGHT = 'C', LEFT = 'D'
};

class Screen final
{
	friend struct Coordinate;
	friend class Pixels;
	friend class Color;
	friend Screen& operator<<(Screen &, const std::string &);

public:
	using Pos = unsigned short;

	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

	~Screen() {
		*output << "\033[?25h";
		Pixels::cur.x = 0, Pixels::cur.y = 0;
		moveCursor(0, 0);
		clearScreen();
	}
	std::vector<Pixels> operator[](std::slice s) {
		std::vector<Pixels> res;
		for (std::size_t i = 0; i < s.size(); ++i)
			res.push_back(screen[s.start() + i * s.stride()]);
		return res;
	}
	Pixels& operator()(Pos row, Pos col) {
		return screen[row * getCol() + col];
	}

	Coordinate moveCursor(Direction, Pos);
	Coordinate moveCursor(Pos, Pos);
	Coordinate moveCursor(Coordinate);

	void fill(char ch, std::time_t time = 0) {
		std::chrono::microseconds mtime(time);
		for(auto &i : screen)
		{
			i = ch;
			std::this_thread::sleep_for(mtime);
		}
	}
	void put(char ch, ColorType color = ColorType::BLACK) {
		screen[cursor.y * getCol() + cursor.x].setColor(color) = ch;
		++cursor;
	}

	static Pos getRow();
	static Pos getCol();

	void clearScreen() const {
		*output << "\033[2J";
	}
	void allColor(ColorType color) {
		for(auto &i : screen)
			i.setColor(color);
	}
	void flushBuf() { (*output).flush(); }
	std::ostream& stream() { return *output; }
	static Screen& getScreen(Pos y, Pos x, std::ostream &os = std::cout, char ch = ' ', ColorType color = BLACK);
	static Screen& resetScreen(Pos y, Pos x, std::ostream &os = std::cout, char ch = ' ', ColorType color = BLACK);

private:
	std::valarray<Pixels> screen;
	Coordinate cursor;
	static std::ostream *output;
	static Coordinate screenSize;
	static std::unique_ptr<Screen> onlyPtr;

	Screen(char ch, ColorType);

	void fillScreen(char);
	void checkRange(Pos y, Pos x) const {
		if(y >= getRow() || x >= getCol())
			throw MoveOutOfRange("光标移动越界!");
	}
};

Screen& operator<<(Screen &screen, const std::string &s);
