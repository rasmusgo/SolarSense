#include "SceneSolarSystem/SceneSolarSystem.hpp"

int main() {

    // Game and window definitions

    WINDOW_TITLE = "SolarSense";
    CONTEXT_SETTINGS_OPENGL = sf::ContextSettings(32,32,4,2,0);
    FOV = 60.0f; //degrees
    ZNEAR = 0.01f;
    ZFAR = 1000.0f;
    SCRWIDTH = sf::VideoMode::getDesktopMode().width;
    SCRHEIGHT = sf::VideoMode::getDesktopMode().height;

    // Init
    Game game;
    game.setRoot(new SceneSolarSystem());
    game.run();

	return 42;
}
