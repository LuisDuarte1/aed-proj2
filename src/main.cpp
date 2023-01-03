
#include "FlightManager.h"
#include "DistanceAirportFinder.h"

int main(){
    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->loadFiles();
    DistanceAirportFinder::display();


}