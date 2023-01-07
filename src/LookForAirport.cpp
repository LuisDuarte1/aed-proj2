#include <queue>
#include <algorithm>
#include "LookForAirport.h"
#include "FlightManager.h"



std::vector<std::shared_ptr<Airline>> intersectAirlines(std::vector<std::shared_ptr<Airline>>& flights, std::vector<std::shared_ptr<Airline>>& airlines){
    std::vector<std::shared_ptr<Airline>> intersection;
    if(airlines.size()==0){
        return flights;
    }
    else {
        std::vector<std::shared_ptr<Airline>> intersection;
        std::set_intersection(flights.begin(), flights.end(),
                              airlines.begin(), airlines.end(), std::back_inserter(intersection));
        return intersection;
    }
}

void LookForAirport::bfs(std::string AirportDep,std::vector<std::shared_ptr<Airline>> airlines){
    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->resetVisitedAirports();
    flightManager->resetdistanceAirports();
    std::shared_ptr<AirportNode> airport = flightManager->getAirportNode(AirportDep);
    if(airport.get() == nullptr) return;
    std::queue<std::shared_ptr<AirportNode>> queue;
    queue.push(airport);
    airport->dist = 0;
    airport->visited = true;
    while(!queue.empty()){
        std::shared_ptr<AirportNode> u = queue.front();
        queue.pop();
        for(auto e:u->flights){
            std::shared_ptr<AirportNode> w = e.destination_node;
            std::vector<std::shared_ptr<Airline>> intersect;
            intersect = intersectAirlines(e.flights,airlines);
            if(!w->visited&&(intersect.size()!=0||airlines.size()==0)){
                queue.push(w);
                w->visited = true;
                w->dist=u->dist + 1;
                w->parent=u;
            }
        }
    }
}
void printFlights(std::list<Flight> flights,std::shared_ptr<AirportNode> airportdeparture)
{
    std::cout<<airportdeparture->airport.getName()<<"\n";
    for(auto it = flights.begin();it != flights.end();it++){
        auto airline = it->flights;
        auto f = airline.begin();
        auto l = *f;
        std::cout<<"    |\n    | Airline: "<< l->getCode()<<"\n    |\n"<<it->destination_node->airport.getName()<<"\n";
    }

}
std::vector<std::shared_ptr<AirportNode>> LookForAirport::searchByAirport(std::string airportcodedeparture,std::string airportcodearrival,
                                                                          std::vector<std::shared_ptr<Airline>> possibleairlines) {
    FlightManager *flightManager = FlightManager::getInstance();
    flightManager->resetVisitedAirports();
    std::shared_ptr<AirportNode> airportarrival = flightManager->getAirportNode(airportcodearrival);
    std::shared_ptr<AirportNode> airportdeparture = flightManager->getAirportNode(airportcodedeparture);
    if (airportarrival.get() == nullptr){
        std::vector<std::shared_ptr<AirportNode>> a;
        return a;
    }
    if (airportdeparture.get() == nullptr){
        std::vector<std::shared_ptr<AirportNode>> a;
        return a;
    }
    bfs(airportcodedeparture, possibleairlines);
    std::list<Flight> path;
    if (airportarrival->visited) {
        std::shared_ptr<AirportNode> airportcurrent = airportarrival;
        while (airportcurrent->airport.getCode() != airportdeparture->airport.getCode()) {
            std::shared_ptr<AirportNode> airportafter = airportcurrent;
            airportcurrent = airportcurrent->parent;
            std::list<Flight> voos = airportcurrent->flights;
            for (auto it = voos.begin(); it != voos.end(); it++) {
                if (it->destination_node == airportafter) {
                    if (possibleairlines.size() == 0) {
                        auto randomcompany = it->flights.begin();
                        std::vector<std::shared_ptr<Airline>> onlyoneairline;
                        onlyoneairline.push_back(*randomcompany);
                        Flight minipath = Flight(airportafter, onlyoneairline);
                        path.push_back(minipath);
                    } else {
                        std::vector<std::shared_ptr<Airline>> intersect;
                        intersect = intersectAirlines(it->flights, possibleairlines);
                        auto randomcompany = intersect.begin();
                        std::vector<std::shared_ptr<Airline>> onlyoneairline;
                        onlyoneairline.push_back(*randomcompany);
                        Flight minipath = Flight(airportafter, onlyoneairline);
                        path.push_back(minipath);
                    }
                    break;
                }
            }
        }
    }
    printFlights(path, airportdeparture);
}




