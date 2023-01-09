#ifndef DISTANCEAIRPORTFINDER_H
#define DISTANCEAIRPORTFINDER_H

#include <iostream>
#include <vector>

#include "FlightManager.h"

namespace DistanceAirportFinder{

    /**
     * @brief Gets the closest airports to the provided coordinate
     * 
     * Complexity: O(|V| + |E|)
     *
     * 
     * @param code starting code
     * @param radius max distance to coordinates
     * @param coords ending coordinates
     * @param airlines airlines restriction
     * @return std::vector<std::shared_ptr<AirportNode>> 
     */
    std::vector<std::shared_ptr<AirportNode>> bfsDistance(std::string code, float radius, std::pair<double,double> coords, std::vector<std::shared_ptr<Airline>>& airlines);


    /**
     * @brief Menu for coordinate airport Finder
     * 
     * Complexity: O(|R|) where R is the returning possible aiports
     */
    void display();

    /**
     * @brief Haversine function to calculate distances in a sphere (aka the Earth)
     * 
     * Complexity: O(1)
     * 
     * @param coords1 coordinates of first pair
     * @param coords2 coordinates of second pair
     * @return double distance in kilometeres
     */
    double haversine(std::pair<double,double> coords1, std::pair<double,double> coords2);

}

#endif
