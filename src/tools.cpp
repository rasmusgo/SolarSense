#include "tools.hpp"

//extern'd in tools.hpp
int SCRWIDTH = sf::VideoMode::getDesktopMode().width;
int SCRHEIGHT = sf::VideoMode::getDesktopMode().height;
bool WINDOWFOCUS = false;
sf::Clock GLOBALCLOCK;

//implement random functions here
std::string toString(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}
