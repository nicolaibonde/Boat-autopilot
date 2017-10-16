#include <iostream>
#include "Coordinate.h"

int main(int argc, char* argv[])
{
	Coordinate uut = Coordinate(100, 100);

	std::cout << uut.Longitude_ << std::endl;
}

