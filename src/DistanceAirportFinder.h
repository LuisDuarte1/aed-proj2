#ifndef DISTANCEAIRPORTFINDER_H
#define DISTANCEAIRPORTFINDER_H

#include <iostream>
#include <vector>

#include "FlightManager.h"

namespace DistanceAirportFinder{

    std::vector<std::shared_ptr<AirportNode>> bfsDistance(std::string code, float radius, std::pair<double,double> coords, std::vector<std::shared_ptr<Airline>>& airlines);

    void display();

    double haversine(std::pair<double,double> coords1, std::pair<double,double> coords2);

}

#endif
