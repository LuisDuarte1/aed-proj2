#include <queue>
#include <algorithm>
#include <sstream>
#include "LookForAirport.h"
#include "FlightManager.h"
namespace LookForAirport{  std::vector<std::list<Flight>> AirlinesDid;}

void LookForAirport::addAirlinesDid(std::list<Flight> a) {
    AirlinesDid.push_back(a);
}
std::vector<std::list<Flight>> LookForAirport::getAirlinesDid() {
    return LookForAirport::AirlinesDid;
}
/**
 * @brief:Function that returns the airlines allowed by the user in comparison to the airlines that fly between two airports.
 * Complexity:O(1)
 *
 * @param flights std::vector<std::shared_ptr<Airline>>& flights
 * @param airlines std::vector<std::shared_ptr<Airline>>& airlines
 * @return std::vector<std::shared_ptr<Airline>>
 */
std::vector<std::shared_ptr<Airline>> intersecterAirlines(std::vector<std::shared_ptr<Airline>>& flights, std::vector<std::shared_ptr<Airline>>& airlines){
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

void LookForAirport:: bfs(std::string AirportDep,std::vector<std::shared_ptr<Airline>> airlines){
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
            intersect = intersecterAirlines(e.flights,airlines);
            if(!w->visited&&(intersect.size()!=0||airlines.size()==0)){
                queue.push(w);
                w->visited = true;
                w->dist=u->dist + 1;
                w->prev=u;
            }
        }
    }
}
/**
 * @brief:Function that given a airport of departure and a list<Flight>, gives output of the route.
 * Complexity:O(n)
 *
 * @param flights std::list<Flight> flights
 * @param airportdeparture std::shared_ptr<AirportNode> airportdeparture
 */
void printFlightstoOneRoute(std::list<Flight> flights,std::shared_ptr<AirportNode> airportdeparture)
{
    std::cout<<airportdeparture->airport.getName()<<"\n";
    for(auto it = flights.begin();it != flights.end();it++){
        auto airline = it->flights;
        auto f = airline.begin();
        auto l = *f;
        std::cout<<"    |\n    | Airline: "<< l->getCode()<<"\n    |\n"<<it->destination_node->airport.getName()<<"\n";
    }
    std::cout<<"\n";
}
void LookForAirport::printFlightstoMoreThanOneRoute(std::shared_ptr<AirportNode> airportdeparture)
{
    std::vector<std::list<Flight>> AirlinesDid = LookForAirport::getAirlinesDid();
    if(AirlinesDid.size()==1){
        std::cout<<"\nSorry... Any more possible routes were found.\n";
    }
    for(auto it=AirlinesDid.begin();it!=AirlinesDid.end();it++) {
        if (it != AirlinesDid.begin()) {
            std::cout << airportdeparture->airport.getName() << "\n";
            for (auto i = it->begin(); i != it->end(); i++) {
                auto airline = i->flights;
                auto f = airline.begin();
                auto l = *f;
                std::cout << "    |\n    | Airline: " << l->getCode() << "\n    |\n"
                          << i->destination_node->airport.getName() << "\n";
            }
            std::cout<<"\n";
        }
    }
    std::cout<<"\n";

}
std::list<Flight> LookForAirport::searchByAirport(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines) {
    FlightManager *flightManager = FlightManager::getInstance();
    flightManager->resetVisitedAirports();
    flightManager->resetdistanceAirports();
    std::shared_ptr<AirportNode> airportarrival = flightManager->getAirportNode(airportcodearrival);
    std::shared_ptr<AirportNode> airportdeparture = flightManager->getAirportNode(airportcodedeparture);
    if (airportarrival.get() == nullptr){
        std::list<Flight> a;
        return a;
    }
    if (airportdeparture.get() == nullptr){
        std::list<Flight> a;
        return a;
    }
    bfs(airportcodedeparture, possibleairlines);
    std::list<Flight> path;
    if (airportarrival->visited) {
        std::shared_ptr<AirportNode> airportcurrent = airportarrival;
        while (airportcurrent->airport.getCode() != airportdeparture->airport.getCode()) {
            std::shared_ptr<AirportNode> airportafter = airportcurrent;
            airportcurrent = airportcurrent->prev;
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
                        intersect = intersecterAirlines(it->flights, possibleairlines);
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
        std::reverse(path.begin(),path.end());
    }
    else{
        std::cout<<"Not available\n";
    }
    return path;
}
void LookForAirport::dfsToAllPaths(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines,int limit,int counter) {
    FlightManager *flightManager = FlightManager::getInstance();
    std::shared_ptr<AirportNode> airportarrival = flightManager->getAirportNode(airportcodearrival);
    std::shared_ptr<AirportNode> airportdeparture = flightManager->getAirportNode(airportcodedeparture);
    airportdeparture->visited = true;
    //std::cout<<airportdeparture->airport.getCode()<<"\n";
    if (airportarrival == airportdeparture) {
        //std::cout<<"GOT HERE!\n";
        std::list<Flight> path;
        if (airportarrival->visited) {
            std::shared_ptr<AirportNode> airportcurrent = airportarrival;
            int counterr = 0;
            while (counterr < limit) {
                std::shared_ptr<AirportNode> airportafter = airportcurrent;
                counterr++;
                airportcurrent = airportcurrent->prev;
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
                            intersect = intersecterAirlines(it->flights, possibleairlines);
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
            std::reverse(path.begin(), path.end());
            int verifyout = 0;
            for (auto i = AirlinesDid.begin(); i != AirlinesDid.end(); i++) {
                int verify = 0;
                auto it = path.begin();
                for (auto ite = i->begin(); ite != i->end(); ite++) {
                    if (it->destination_node != ite->destination_node) {
                        it++;
                    } else {
                        verify++;
                        it++;
                    }
                }
                if (verify == limit) {
                    verifyout++;
                }
            }
            if(verifyout==0){LookForAirport::addAirlinesDid(path); }
        }
        else{
            std::cout << "Not available\n";
        }

    } else if (counter == limit) {
        //std::cout<<"Not got here!\n";
        return;
    } else {
        counter++;
        for (auto it = airportdeparture->flights.begin(); it != airportdeparture->flights.end(); it++) {
            if (!it->destination_node->visited) {
                it->destination_node->prev = airportdeparture;
                //std::cout<<it->destination_node->airport.getCode()<<"\n";
                dfsToAllPaths(it->destination_node->airport.getCode(), airportcodearrival, possibleairlines, limit,
                              counter);
            }
        }
    }
}
void LookForAirport::searchByMoreRoutes(std::string airportcodedeparture,std::string airportcodearrival, std::vector<std::shared_ptr<Airline>> possibleairlines){

    std::list<Flight> firstroute = searchByAirport(airportcodedeparture,airportcodearrival,possibleairlines);
    if(firstroute.size()==0){
        std::cout<<"Not possible at all to give a single route\n";
        return;
    }
    else {

        FlightManager *flightManager = FlightManager::getInstance();
        flightManager->resetVisitedAirports();
        flightManager->resetdistanceAirports();
        std::shared_ptr<AirportNode> airportarrival = flightManager->getAirportNode(airportcodearrival);
        std::shared_ptr<AirportNode> airportdeparture = flightManager->getAirportNode(airportcodedeparture);
        if (airportarrival.get() == nullptr) {
            return;
        }
        if (airportdeparture.get() == nullptr) {
            return;
        }
        int size = firstroute.size();
        int count = 0;
        AirlinesDid.clear();
        AirlinesDid.push_back(firstroute);
        dfsToAllPaths(airportcodedeparture, airportcodearrival, possibleairlines, size, count);
        LookForAirport::printFlightstoMoreThanOneRoute(airportdeparture);
    }
}

void LookForAirport::display(){

    std::string airportCodeDeparture;
    std::cout << "Input the airport code of departure: ";
    if(!(std::cin >> airportCodeDeparture).good()){
        std::cout << "Invalid airport code... try again.\n";
        return display();
    }
    std::string airportCodeArrival;
    std::cout << "Input the airport code of arrival: ";
    if(!(std::cin >> airportCodeArrival).good()){
        std::cout << "Invalid airport code... try again.\n";
        return display();
    }
    std::string airlines_restriction;
    std::cout << "Input the airlines that you want to travel (write 'none' for no restriction, seperator ,): ";
    std::cin >> airlines_restriction;

    std::list<Flight> destAirports;

    std::vector<std::shared_ptr<Airline>> airlines;
    if(airlines_restriction == "none"){
        std::cout <<"\n";
        destAirports=searchByAirport(airportCodeDeparture, airportCodeArrival, airlines);
        if(destAirports.size()==0){
            std::cout<<"Not possible at all to give a single route\n";
            return;
        }
        else {
            FlightManager * flightManager = FlightManager::getInstance();
            flightManager->resetVisitedAirports();
            flightManager->resetdistanceAirports();
            std::shared_ptr<AirportNode> airportdeparture = flightManager->getAirportNode(airportCodeDeparture);
            printFlightstoOneRoute(destAirports,airportdeparture);
        }

    } else {
        airlines_restriction.erase(std::remove(airlines_restriction.begin(), airlines_restriction.end(), ' ')
                , airlines_restriction.end());
        std::stringstream airlineStream(airlines_restriction);
        std::string buf;
        FlightManager * flightManager = FlightManager::getInstance();
        while(std::getline(airlineStream,buf, ',').good()){
            std::shared_ptr<Airline> i = flightManager->getAirline(buf);
            if(i.get() == nullptr){
                std::cout << buf << "is not a valid airline... try again.\n";
                return display();
            }
            airlines.push_back(i);
        }
        std::shared_ptr<Airline> i = flightManager->getAirline(buf);
        if(i.get() == nullptr){
            std::cout << buf << "is not a valid airline... try again.\n";
            return display();
        }
        airlines.push_back(i);
        destAirports=searchByAirport(airportCodeDeparture, airportCodeArrival, airlines);
        if(destAirports.size()==0){
            std::cout<<"Not possible at all to give a single route\n";
            return;
        }
        else {
            flightManager->resetVisitedAirports();
            flightManager->resetdistanceAirports();
            std::shared_ptr<AirportNode> airportdeparture = flightManager->getAirportNode(airportCodeDeparture);
            printFlightstoOneRoute(destAirports,airportdeparture);
        }

    }
    std::cout<<"\nDo you want more options of routes (if they exist) with the same amount of flights?\n"
               "Type 'YES' for yes and 'NO' for no\n";
    std::string a;
    std::cin>>a;
    if(a=="YES"){
        searchByMoreRoutes(airportCodeDeparture, airportCodeArrival, airlines);
    }


}



