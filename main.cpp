#include "Screen.h"
#include <iomanip>
#include <random>
#include <functional>
#include <mutex>
#include <signal.h>

std::mutex mtx;

struct HourMeter
{
	HourMeter(Screen &s, const Color &c) : screen(s), color(c) { }
	void operator()()
	{
		constexpr std::string_view cost("用时:");
		while (swit)
		{
			{
				std::lock_guard lock(mtx);
				auto old = screen.moveCursor(Screen::getRow() - 1, Screen::getCol() - 10);	
				screen.stream() << color.form() << cost << std::setw(4) << time << 's';
				screen.moveCursor(old);	
			}
			screen.flushBuf();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			++time;
		}
	}
	Screen &screen;
	std::size_t time = 0;
	Color color;
	static bool swit;
};
bool HourMeter::swit = true;

static struct CloseEcho
{
	CloseEcho() { 
		system("stty -icanon");
		system("stty -echo");
	}
	~CloseEcho() {
		system("stty icanon");
		system("stty echo");
	}
}ce;

void printCount(Screen &screen, std::size_t count, Color color)
{
	constexpr std::string_view bingo("正确:");
	auto old = screen.moveCursor(screen.getRow() - 1, 0);
	screen.stream() << color.form() << bingo << std::setw(3) << count;
	screen.moveCursor(old);
}

[[noreturn]] void sigintExit(int) {
	std::exit(EXIT_SUCCESS);
}

int main()
{
	constexpr Screen::Pos row = 45, column = 150;
	constexpr Color textColor;
	signal(SIGINT, sigintExit);

	Screen &screen = Screen::getScreen(row, column, std::cout, ' ');
	std::string str;
	{
		auto randChar = std::bind(std::uniform_int_distribution<char>('A', 'Z'), std::default_random_engine(std::random_device()()));
		for (std::size_t i = 0; i < (row - 1) * column; ++i)
		{
			str.push_back(randChar());
			screen.put(str.back());
		}
	}
	screen.flushBuf();
	screen.moveCursor(0, 0);
	std::size_t count = 0;
	printCount(screen, count, textColor);
	HourMeter hm(screen, textColor);
	std::thread hmt(hm);
	char c;
	for (std::size_t i = 0; i < (row - 1) * column; ++i)
	{
		std::cin >> c;
		std::lock_guard lock(mtx);
		if (str[i] == std::toupper(c))
		{
			screen.put(str[i], ColorType::GREEN);
			printCount(screen, ++count, textColor);
		}
		else
			screen.put(str[i], ColorType::RED);
		screen.flushBuf();
	}
	HourMeter::swit = false;
	hmt.join();
	std::this_thread::sleep_for(std::chrono::seconds(3));
}
