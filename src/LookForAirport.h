//
// Created by Joca on 06/01/2023.
//

#ifndef PROJ1_AED_LOOKFORAIRPORT_H
#define PROJ1_AED_LOOKFORAIRPORT_H

#include <string>
#include <vector>
#include "Airline.h"
#include "FlightManager.h"

namespace LookForAirport{

    std::vector<std::shared_ptr<AirportNode>> searchByAirport(std::string airportcodedeparture,std::string airportcodearrival,std::vector<std::shared_ptr<Airline>> possibleairlines);

    void bfs(std::string AirportDep,std::vector<std::shared_ptr<Airline>> airlines);
}

#endif //PROJ1_AED_LOOKFORAIRPORT_H
