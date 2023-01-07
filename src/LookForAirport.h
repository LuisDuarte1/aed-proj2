
#ifndef PROJ1_AED_LOOKFORAIRPORT_H
#define PROJ1_AED_LOOKFORAIRPORT_H

#include <string>
#include <vector>
#include "Airline.h"
#include "FlightManager.h"

namespace LookForAirport{
    extern std::vector<std::list<Flight>> AirlinesDid;
    std::list<Flight> searchByAirport(std::string airportcodedeparture,std::string airportcodearrival,std::vector<std::shared_ptr<Airline>> possibleairlines);
    void searchByMoreRoutes(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines);
    void addAirlinesDid(std::list<Flight> a);
    std::vector<std::list<Flight>> getAirlinesDid();
    void printFlightstoMoreThanOneRoute(std::shared_ptr<AirportNode> airportdeparture);
    void dfsToAllPaths(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines,int limit,int counter);
}

#endif
