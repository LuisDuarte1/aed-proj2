#include "DistanceAirportFinder.h"

#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>


std::vector<std::shared_ptr<Airline>> intersectAirlines(std::vector<std::shared_ptr<Airline>>& flights, std::vector<std::shared_ptr<Airline>>& airlines){
    std::vector<std::shared_ptr<Airline>> intersection;
    std::set_intersection(flights.begin(), flights.end(),
        airlines.begin(), airlines.end(), std::back_inserter(intersection));
    return intersection;
}

bool checkIfFlightHasAirline(std::vector<std::shared_ptr<Airline>>& flights, std::vector<std::shared_ptr<Airline>>& airlines){
    return intersectAirlines(flights, airlines).size() != 0;
}


std::vector<std::shared_ptr<AirportNode>> DistanceAirportFinder::bfsDistance(std::string code, float radius, std::pair<double,double> coords
    ,std::vector<std::shared_ptr<Airline>>& airlines){

    FlightManager * flightManager = FlightManager::getInstance();
    flightManager->resetVisitedAirports();
    std::shared_ptr<AirportNode> airport = flightManager->getAirportNode(code);
    if(airport.get() == nullptr) return {};
    std::queue<std::shared_ptr<AirportNode>> queue;
    queue.push(airport);

    std::vector<std::shared_ptr<AirportNode>> possible_airports;

    while(!queue.empty()){
        std::shared_ptr<AirportNode> i = queue.front();
        queue.pop();
        bool to_stop = false;
        double maxDist = haversine(i->airport.getCoordinates(), coords);

        
        for(Flight& flight : i->flights){
            if(flight.destination_node->visited) continue;

            double dist = haversine(coords,flight.destination_node->airport.getCoordinates());
            //the distance should reduce to better optimize this, otherwise it will probably get stuck checking all coordinates
            if(dist > maxDist || (!checkIfFlightHasAirline(flight.flights, airlines) && airlines.size() != 0)) continue;
            flight.destination_node->parent = i;
            flight.destination_node->visited = true;
            queue.push(flight.destination_node);
            if(dist <= radius){
                to_stop = true;
                possible_airports.push_back(flight.destination_node);
            }
        }
        if(to_stop) break;
    }    
    return possible_airports;
}

std::stack<std::shared_ptr<AirportNode>> tracebackPath(std::shared_ptr<AirportNode> destination){
    std::stack<std::shared_ptr<AirportNode>> path;
    std::shared_ptr<AirportNode> i = destination;
    while(i.get() != nullptr){
        path.push(i);
        i = i->parent;
    }
    return path;
}


std::vector<std::shared_ptr<Airline>> getPossibleAirlines(std::shared_ptr<AirportNode>& src, 
    std::shared_ptr<AirportNode>& dst, std::vector<std::shared_ptr<Airline>>& possibleAirlines){
    
    for(Flight& flight : src->flights){
        if((*flight.destination_node) == (*dst)){
            if(possibleAirlines.size() == 0) return flight.flights;
            return intersectAirlines(flight.flights, possibleAirlines);
        }
    }
    return {};
}

std::string toStringPossibleAirlines(std::vector<std::shared_ptr<Airline>> airlines){
    std::string r = "Airlines: ";
    if(airlines.size() == 0) return "Error: no possible airlines?";
    for(auto it = airlines.begin(); it != --airlines.end(); it++){
        r += (*it)->getCode();
        r += " , ";
    }
    r += airlines[airlines.size()-1]->getCode();
    return r;
}

void DistanceAirportFinder::display(){

    std::string airportCode;
    std::cout << "Input the airport code: ";
    if(!(std::cin >> airportCode).good()){
        std::cout << "Invalid airport code... try again.\n";
        return display();
    }

    double latitude;
    std::cout << "Input the destination latitude: ";
    if(!(std::cin >> latitude).good()){
        std::cout << "Invalid latitude... try again.\n";
        return display();
    }
    double longitude;
    std::cout << "Input the destination longitude:";

    if(!(std::cin >> longitude).good()){
        std::cout << "Invalid longitude... try again.\n";
        return display();       
    }
    int max_distance;
    std::cout << "Input the max distance to destination (in kilometers):";
    if(!(std::cin >> max_distance).good()){
        std::cout << "Invalid max distance... try again.\n";
        return display();
    }

    std::string airlines_restriction;
    std::cout << "Input the airlines that you want to travel (write 'none' for no restriction, seperator ,): ";
    std::cin >> airlines_restriction;

    std::vector<std::shared_ptr<AirportNode>> destAirports;

    std::vector<std::shared_ptr<Airline>> airlines;
    if(airlines_restriction == "none"){
        destAirports = bfsDistance(airportCode, max_distance, std::make_pair(latitude, longitude), airlines);
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
        destAirports = bfsDistance(airportCode, max_distance, std::make_pair(latitude, longitude), airlines);
    }


    if(destAirports.empty()){
        std::cout << "No suitable airports with this criteria have been found... try again.\n";
        return display();
    }
    std::shared_ptr<AirportNode> closestAirport;
    double minDist = INFINITY;
    for(std::shared_ptr<AirportNode> i : destAirports){
        double dist = haversine(i->airport.getCoordinates(), std::make_pair(latitude, longitude));
        if(dist < minDist){
            minDist = dist;
            closestAirport = i;
        }
    }
    std::stack<std::shared_ptr<AirportNode>> path = tracebackPath(closestAirport);
    std::shared_ptr<AirportNode> last;
    while(path.size() != 1){
        std::shared_ptr<AirportNode> i = path.top();
        path.pop();
        std::cout << "\t"<<i->airport.getCode() << "\n\t" << i->airport.getName() << "\n";
        std::cout << "\t|\n\t| " << toStringPossibleAirlines(getPossibleAirlines(i, path.top(), airlines)) <<"\n\t|\n\t|\n";
        last = i;
    }
    std::shared_ptr<AirportNode> i = path.top();
    std::cout << "\t" <<i->airport.getCode() << "\n\t" << i->airport.getName() << "\n";


    std::cout << "Distance to desired coordinates: "<< minDist << " km\n";


}



double DistanceAirportFinder::haversine(std::pair<double,double> coords1, std::pair<double,double> coords2){

        double PI = 4.0*atan(1.0);
        
        double dlat1=coords1.first*(PI/180);

        double dlong1=coords1.second*(PI/180);
        double dlat2=coords2.first*(PI/180);
        double dlong2=coords2.second*(PI/180);

        double dLong=dlong1-dlong2;
        double dLat=dlat1-dlat2;

        double aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
        double cHarv=2*atan2(sqrt(aHarv),sqrt(1.0-aHarv));
        //The IUGG value for the equatorial radius of the Earth is 6378.137 km (3963.19 mile)
        const double earth=6378.137;
        return earth*cHarv;
}
