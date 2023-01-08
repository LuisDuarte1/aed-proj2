#ifndef STATISTICS_H
#define STATISTICS_H

#include "FlightManager.h"

namespace Statistics{

    /**
     * @brief Gets a list of top countries by: number of airports or number of airlines
     * 
     * Complexity: O(nlogn)
     * 
     */
    void topCountry();



    /**
     * @brief Gets a list of top countries by: total number of flights or number of airlines that opererate in that region
     * 
     * Complexity: O(nlogn)
     * 
     */
    void topAirports();


    /**
     * @brief Gets the diameter of the network
     * 
     * Complexity: O(|V| + |E|)
     * 
     */
    void diameter();

    /**
     * @brief Main statistics menu
     * 
     * Complexity: O(1)
     * 
     */
    void statisticsMenu();

    /**
     * @brief Gets the most distante airportnode starting from node
     * 
     * Complexity: O(|V| + |E|)
     * 
     * @param node starting airportNode
     * @return std::shared_ptr<AirportNode> most distant airportNOde
     */
    std::shared_ptr<AirportNode> bfsMax(std::shared_ptr<AirportNode> node);

    /**
     * @brief Gets the distance between two nodes
     * 
     * Complexity: O(|V| + |E|)
     * 
     * @param node starting node
     * @param end finish node
     * @return int distance
     */
    int bfsDist(std::shared_ptr<AirportNode> node, std::shared_ptr<AirportNode> end);

}


#endif