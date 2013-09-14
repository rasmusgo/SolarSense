#include "SolarSenseApp.hpp"
#include "SceneSolarSystem/SceneSolarSystem.hpp"

/*
 * Code structure and basic classes taken from Towerthousand's WIP game engine at https://github.com/Towerthousand/VBE
 */

int main() {
    SolarSenseApp myApp;

    if(myApp.init()) {
        myApp.setScene(new SceneSolarSystem(myApp));
        myApp.run();
	}
	return 42;
}
