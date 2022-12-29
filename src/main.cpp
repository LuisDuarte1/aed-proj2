
#include "FlightManager.h"


int main(){
    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->loadFiles();
    std::cout  << "Hello world\n";
}