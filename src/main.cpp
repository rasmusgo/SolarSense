#include "SceneSolarSystem/SceneSolarSystem.hpp"

bool isWindow = false;

int main(int argc, char* argv[]) {

    // Game and window definitions

    WINDOW_TITLE = "SolarSense";
    CONTEXT_SETTINGS_OPENGL = sf::ContextSettings(32,32,4,2,0);
    FOV = 60.0f; //degrees
    ZNEAR = 0.01f;
    ZFAR = 2000.0f;
    SCRWIDTH = sf::VideoMode::getDesktopMode().width/2;
    SCRHEIGHT = sf::VideoMode::getDesktopMode().height/2;

    bool slaveMode = false;

    if(argc > 1){
        if(argv[1][0] == 'w') {
            isWindow = true;
            SCRWIDTH = sf::VideoMode::getDesktopMode().width/2;
            SCRHEIGHT = sf::VideoMode::getDesktopMode().height/2;
        }
        else if (argv[1][0] == 's')
            slaveMode = true;
    }


    // Init
    Game game(slaveMode);
    game.setRoot(new SceneSolarSystem());
    game.run();

	return 42;
}
