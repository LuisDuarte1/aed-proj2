
#include "FlightManager.h"
#include "DistanceAirportFinder.h"
#include "Statistics.h"

int main(){
    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->loadFiles();

}