#ifndef FLIGHTMANAGER_H
#define FLIGHTMANAGER_H

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>
#include <stack>

#include "Airline.h"
#include "Airport.h"

struct AirportNode;

/**
 * @brief Data structures that stores flight information, acts like an edge of a graph
 * 
 */
struct Flight{
    Flight(std::shared_ptr<AirportNode> destination_node,std::vector<std::shared_ptr<Airline>> flights);

    std::shared_ptr<AirportNode> destination_node;
    
    std::vector<std::shared_ptr<Airline>> flights;

};

/**
 * @brief Stores the airport and also stores information necessary to do graph operations
 * 
 */
struct AirportNode{
    AirportNode(Airport airport);

    Airport airport;

    std::list<Flight> flights;

    std::shared_ptr<AirportNode> parent;

    std::shared_ptr<AirportNode> prev;

    bool visited;

    int dist; //distance to destination node


    /**
     * @brief equals function for airportNode
     * Complexity: O(1)
     * 
     * @param node other AirportNode
     * @return true if airport is equal
     * @return false if airport is false
     */
    bool operator==(const AirportNode& node) const;
};


class FlightManager{

    

        struct AirlineEquals{
            bool operator()(const std::shared_ptr<Airline>& airportNode1, const std::shared_ptr<Airline>& airportNode2) const noexcept;

        };
        

        struct AirportHash{
            std::size_t operator()(const std::shared_ptr<AirportNode>& airportNode) const noexcept;
        };

        struct AirportEquals{
            bool operator()(const std::shared_ptr<AirportNode>& airportNode1, const std::shared_ptr<AirportNode>& airportNode2) const noexcept;
        };


    public:

        FlightManager(FlightManager& other) = delete;

        std::vector<std::stack<std::shared_ptr<AirportNode>>> cityFlights(std::string src,std::string dest,std::vector<std::shared_ptr<Airline>> possibleairlines);

        std::vector<std::string> airportinformation(std::string code);

        std::vector<std::string> airportinformationreachable(std::string code, int n);

        std::set<std::string> airportinformationairlines(std::string code);

        std::set<std::string> airportinformationcountries(std::string code);

        void displayairportinformation();

        void displaycityflight();

        void operator=(const FlightManager &) = delete;

        /**
         * @brief Get the Instance object
         * 
         * Complexity: O(1)
         * 
         * @return FlightManager* 
         */
        static FlightManager* getInstance();

        /**
         * @brief function that loads .csv files into memory
         * 
         * Complexity: O(n)
         * 
         */
        void loadFiles();

        /**
         * @brief Get the Airport Node object
         * 
         * Complexity: O(1)
         * 
         * @param code airport code
         * @return std::shared_ptr<AirportNode> AirportNodeObject
         */
        std::shared_ptr<AirportNode> getAirportNode(std::string code);

        /**
         * @brief Get the Airline object
         * 
         * Complexity: O(1)
         * @param code airline code
         * @return std::shared_ptr<Airline> airline object
         */
        std::shared_ptr<Airline> getAirline(std::string code);




        void resetdistanceAirports();

        /**
         * @brief resets visited airports
         * 
         * Complexity: O(|V|) where v is the list of airports
         * 
         */
        void resetVisitedAirports();

        /**
         * @brief adds flight that departs in src and lands in dst.
         * 
         * Complexity: O(|E|) where E is the list of flights that departs from src
         * 
         * @param src_code source airport code
         * @param dst_code destination airport code
         * @param airline airline object
         */
        bool addFlight(std::string src_code, std::string dst_code, std::shared_ptr<Airline> airline);

        /**
         * @brief Get the Airports object
         * Complexity: O(1)
         * 
         * @return std::unordered_set<std::shared_ptr<AirportNode>, AirportHash, AirportEquals> 
         */
        inline std::unordered_set<std::shared_ptr<AirportNode>, AirportHash, AirportEquals> getAirports(){return airports;};

        /**
         * @brief Get the Airlines object
         * Complexity: O(1)
         * 
         * @return std::unordered_set<std::shared_ptr<Airline>, HashAirline, AirlineEquals> 
         */
        inline std::unordered_set<std::shared_ptr<Airline>, HashAirline, AirlineEquals> getAirlines(){return airlines;};




    private:



        std::unordered_set<std::shared_ptr<AirportNode>, AirportHash, AirportEquals> airports;
        std::unordered_set<std::shared_ptr<Airline>, HashAirline, AirlineEquals> airlines;

        FlightManager(){}

        static FlightManager* instance;
};


#endif