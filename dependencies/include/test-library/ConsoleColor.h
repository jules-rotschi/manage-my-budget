#pragma once

#include <iostream>

enum class Color
{
	DEFAULT,
	GREEN,
	RED
};

void SetConsoleColor(Color color)
{
	switch (color)
	{
		using enum Color;
	case DEFAULT:
		std::cout << "\033[0m";
		break;
	case GREEN:
		std::cout << "\033[32m";
		break;
	case RED:
		std::cout << "\033[31m";
		break;
	}
}

void ResetConsoleColor()
{
	SetConsoleColor(Color::DEFAULT);
}