#ifndef PROJ1_AED_LOOKFORAIRPORT_H
#define PROJ1_AED_LOOKFORAIRPORT_H

#include <string>
#include <vector>
#include "Airline.h"
#include "FlightManager.h"
/**
 * @brief: Namespace responsible of searching and giving output of routes search by type "AIRPORT"
 */
namespace LookForAirport{
    extern std::vector<std::list<Flight>> AirlinesDid;
    /**
     * @brief:Function that given a string of the Airport Code of the airport of departure and arrival, a vector of shared ptr to Airline of possible airlines
     * and returns a list of class FLIGHT of the best route with the minimum of flights from a airport to another.
     * Complexity:O(n logn)
     *
     * @param airportcodedeparture std::string airportcodedeparture
     * @param airportcodearrival std::string airportcodearrival
     * @param possibleairlines std::vector<std::shared_ptr<Airline>> possibleairlines
     * @return std::list<Flight>
     */
    std::list<Flight> searchByAirport(std::string airportcodedeparture,std::string airportcodearrival,std::vector<std::shared_ptr<Airline>> possibleairlines);
    /**
     * @brief:Function that given a string of the Airport Code of the airport of departure and arrival, a vector of shared ptr to Airline of possible airlines
     * searchs for another possible routes with the same amount of flights.
     * Complexity:O(1)
     *
     * @param airportcodedeparture std::string airportcodedeparture
     * @param airportcodearrival std::string airportcodearrival
     * @param possibleairlines std::vector<std::shared_ptr<Airline>> possibleairlines
     */
    void searchByMoreRoutes(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines);
    /**
     * @brief:Function that adds a list<Flight> to the vector AirlinesDid;
     * Complexity:O(1)
     *
     * @param a std::list<Flight> a;
     */
    void addAirlinesDid(std::list<Flight> a);
    /**
     * @brief: Function that returns the vector of list<Flight> AirlinesDid
     * Complexity:O(1)
     *
     * @return std::vector<std::list<Flight>>
     */
    std::vector<std::list<Flight>> getAirlinesDid();
    /**
     * @brief:Function that gives output of the other possible routes.
     * Complexity:O(n)
     *
     * @param airportdeparture std::shared_ptr<AirportNode> airportdeparture
     */
    void printFlightstoMoreThanOneRoute(std::shared_ptr<AirportNode> airportdeparture);
    /**
     * @brief:Function that given a string of the Airport Code of the airport of departure and arrival, a vector of shared ptr to Airline of possible airlines
     * and a limit and a counter(parameter that helps the recursion) that, using recursion,uses dfs to find other possible routes between the two airports.
     * Complexity:O(V+E)
     * @param airportcodedeparture std::string airportcodedeparture
     * @param airportcodearrival std::string airportcodearrival
     * @param possibleairlines std::vector<std::shared_ptr<Airline>> possibleairlines
     * @param limit int limit
     * @param counter int counter
     */
    void dfsToAllPaths(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines,int limit,int counter);
    /**
    * @brief:Function that searches by bfs through the Airport Graph and sets the distance and the previous node to every node
    * Complexity:O(V+E)
    * @param AirportDep
    * @param airlines
    */
    void bfs(std::string AirportDep,std::vector<std::shared_ptr<Airline>> airlines);
    /**
     * @brief:Function that asks the user for the airport of departure,airport of arrival and the airlines that he wants to fly by.
     * Complexity:O(1)
     */
    void display();
}

#endif
