#ifndef __PRINT_H__
#define __PRINT_H__

#include <iostream>
#include <string>

enum COLOR {
	BLACE = 30, RED, GREEN, YELLO, BLUE, FUCHSIA, ULTRAMARINE, WHITE
};


template <typename T>
void print(const T v) {
	std::cout << v << std::endl;
}

template <typename T, typename... Args>
void print(const T v, Args... args) {
	std::cout << v << " ";
	print(args...);
}

template <typename... Args>
void print(COLOR color, Args... args) {
	std::cout << "\033[1;" << color << "m";
	print(args...);
	std::cout << "\033[0m";
}

#endif
