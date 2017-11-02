#include <iostream>
#include <string>
#include <CAPTAIN/Coordinate.h>

int main()
{
	//Compiler test
	Coordinate c(10, 56);
	std::cout << "lat: " << c.Latitude_ << " long: " << c.Longitude_ << std::endl;
}
