
#include "FlightManager.h"
#include "LookForAirport.h"
#include "DistanceAirportFinder.h"
#include "menu.h"
#include "Statistics.h"


int main(){
    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->loadFiles();

    menu::inicio();
}