#pragma once
#include <exception>

struct MoveOutOfRange : public std::logic_error
{
	MoveOutOfRange() : logic_error("move out of range") { }
	explicit MoveOutOfRange(const std::string &info) : logic_error(info) { }
};

struct ScreenError : public std::logic_error
{
	ScreenError() : logic_error("screen size error") { }
	explicit ScreenError(const std::string &info) : logic_error(info) { }
};
