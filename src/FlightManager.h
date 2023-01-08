#ifndef FLIGHTMANAGER_H
#define FLIGHTMANAGER_H

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>

#include "Airline.h"
#include "Airport.h"

struct AirportNode;

struct Flight{
    Flight(std::shared_ptr<AirportNode> destination_node,std::vector<std::shared_ptr<Airline>> flights);

    std::shared_ptr<AirportNode> destination_node;
    
    std::vector<std::shared_ptr<Airline>> flights;

};

struct AirportNode{
    AirportNode(Airport airport);

    Airport airport;

    std::list<Flight> flights;

    std::shared_ptr<AirportNode> parent;

    std::shared_ptr<AirportNode> prev;

    bool visited;

    int dist; //distance to destination node


    bool operator==(const AirportNode& node) const;
};


class FlightManager{

    


    public:

        FlightManager(FlightManager& other) = delete;

        void operator=(const FlightManager &) = delete;

        static FlightManager* getInstance();


        void loadFiles();

        std::shared_ptr<AirportNode> getAirportNode(std::string code);

        std::shared_ptr<Airline> getAirline(std::string code);

        std::vector<std::shared_ptr<Airline>> getAirlines();

        void resetVisitedAirports();

        void resetdistanceAirports();

        bool addFlight(std::string src_code, std::string dst_code, std::shared_ptr<Airline>);

        std::vector<std::shared_ptr<AirportNode>> cityFlights(std::string src,std::string dest,std::vector<std::shared_ptr<Airline>> possibleairlines);

        std::vector<std::string> airportinformation(std::string code);

        std::vector<std::string> airportinformationreachable(std::string code, int n);

        std::set<std::string> airportinformationairlines(std::string code);

        std::set<std::string> airportinformationcountries(std::string code);

        void displayairportinformation();

        void displaycityflight();



    private:



        struct AirportHash{
            std::size_t operator()(const std::shared_ptr<AirportNode>& airportNode) const noexcept;
        };

        struct AirportEquals{
            bool operator()(const std::shared_ptr<AirportNode>& airportNode1, const std::shared_ptr<AirportNode>& airportNode2) const noexcept;
        };

        struct AirlineEquals{
            bool operator()(const std::shared_ptr<Airline>& airportNode1, const std::shared_ptr<Airline>& airportNode2) const noexcept;

        };
        
        std::unordered_set<std::shared_ptr<AirportNode>, AirportHash, AirportEquals> airports;
        std::unordered_set<std::shared_ptr<Airline>, HashAirline, AirlineEquals> airlines;

        FlightManager(){}

        static FlightManager* instance;
};


#endif