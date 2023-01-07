
#include "FlightManager.h"
#include "LookForAirport.h"
#include "DistanceAirportFinder.h"

int main(){
    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->loadFiles();
    //DistanceAirportFinder::display();
    std::shared_ptr<Airline> a = flightManager->getAirline("RYR");
    std::vector<std::shared_ptr<Airline>> possibru;
    //possibru.push_back(a);
    LookForAirport::searchByMoreRoutes("TMS","LIN",possibru);


}