#include "FlightManager.h"
#include "LookForAirport.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <set>

FlightManager* FlightManager::instance = nullptr;


std::size_t FlightManager::AirportHash::operator()(const std::shared_ptr<AirportNode>& airportNode) const noexcept{
    return std::hash<std::string>{}(airportNode->airport.getCode());
}


bool FlightManager::AirportEquals::operator()(const std::shared_ptr<AirportNode>& airportNode1, const std::shared_ptr<AirportNode>& airportNode2) const noexcept{
    return (*airportNode1) == (*airportNode2);
}


bool FlightManager::AirlineEquals::operator()(const std::shared_ptr<Airline>& airportNode1, const std::shared_ptr<Airline>& airportNode2) const noexcept{
    return (*airportNode1) == (*airportNode2);
}


AirportNode::AirportNode(Airport airport) : airport(airport){
    visited = false;
}
Flight::Flight(std::shared_ptr<AirportNode> destination_node, std::vector<std::shared_ptr<Airline>> flights) {
    this->flights = flights;
    this->destination_node = destination_node;
}

bool AirportNode::operator==(const AirportNode& node) const{
    return airport == node.airport;
}


FlightManager* FlightManager::getInstance(){
    if(instance == nullptr) instance = new FlightManager();

    return instance;
}


std::string& trim_string(std::string& str) {
    #ifndef _WIN32
        /*
            O windows usa \r\n para sinalizar o fim de uma linha, Enquanto que o linux só usa \n.
            Ou seja ao ler ficheiros com o formato do windows no linux vamos ter um caracter a mais
            e vai induzir em erros na comparação de turmas.
        */
        if(str.find_first_of('\r') != std::string::npos)
            str = str.substr(0, str.size()-1);
    #endif
    return str;
}



void FlightManager::loadFiles(){
    //o windows usa \\ para os diretórios enquanto o unix usa /
    #ifndef _WIN32
        std::ifstream file_airlines = std::ifstream("../recursos/airlines.csv");
        std::ifstream file_airports = std::ifstream("../recursos/airports.csv");
        std::ifstream file_flights = std::ifstream("../recursos/flights.csv");
    #else
        std::ifstream file_airlines = std::ifstream("..\\recursos\\airlines.csv");
        std::ifstream file_airports = std::ifstream("..\\recursos\\airports.csv");
        std::ifstream file_flights = std::ifstream("..\\recursos\\flights.csv");
    #endif

    std::string buffer;
    //skip first line
    std::getline(file_airlines, buffer);
    while(std::getline(file_airlines, buffer).good()){
        std::istringstream line(buffer);

        std::string code;
        std::getline(line, code, ',');
        std::string name; 
        std::getline(line, name, ',');
        std::string callsign; 
        std::getline(line, callsign, ',');
        std::string country; 
        std::getline(line, country, ',');
        country = trim_string(country);

        auto airline = std::make_shared<Airline>(code,callsign,name,country);
        
        auto it = airlines.insert(airline);
        if(!it.second){
            std::cout << "Couldn't insert element " << code << " because of: " << (*it.first)->getCode() << "\n";
        }
    }
    
    std::getline(file_airports, buffer);
    while(std::getline(file_airports, buffer).good()){
        std::istringstream line(buffer);
        std::string code;
        std::getline(line, code, ',');
        std::string name;
        std::getline(line, name, ',');
        std::string city;
        std::getline(line, city, ',');
        std::string country;
        std::getline(line, country, ',');
        std::string latitude;
        std::getline(line, latitude, ',');
        std::string longitude;
        std::getline(line, longitude, ',');

        longitude = trim_string(longitude);
        
        airports.insert(std::make_shared<AirportNode>(Airport(code, name, city, country
            , std::atof(latitude.c_str()), std::atof(longitude.c_str()))));

    }

    std::getline(file_flights, buffer);
    while(std::getline(file_flights, buffer).good()){
        std::istringstream line(buffer);
        std::string src;
        std::getline(line, src, ',');
        std::string dst;
        std::getline(line, dst, ',');
        std::string airline;
        std::getline(line, airline, ',');
        airline = trim_string(airline);
        addFlight(src,dst,getAirline(airline));
    }
}


void FlightManager::resetVisitedAirports(){
    static const std::shared_ptr<AirportNode> null =std::shared_ptr<AirportNode>(nullptr); 
    for(auto it = airports.begin(); it != airports.end(); it++){
        (*it)->visited = false;
        (*it)->parent = null;
        (*it)->dist=-1;
    }
}

bool FlightManager::addFlight(std::string src_code, std::string dst_code, std::shared_ptr<Airline> airline){
    std::shared_ptr<AirportNode> src = getAirportNode(src_code);
    if(src.get() == nullptr) return false;
    std::shared_ptr<AirportNode> dst = getAirportNode(dst_code);
    if(dst.get() == nullptr) return false;


    bool found = false;
    
    for(auto it = src->flights.begin(); it != src->flights.end(); it++){
        if((*(*it).destination_node) == *dst){
            found = true;
            (*it).flights.push_back(airline);
            break;
        }
    }

    if(!found) src->flights.push_back({dst, {airline}});
    return true;
}

std::shared_ptr<AirportNode> FlightManager::getAirportNode(std::string code){
    return *airports.find(std::make_shared<AirportNode>(Airport(code)));

}


std::shared_ptr<Airline> FlightManager::getAirline(std::string code){
    return *airlines.find(std::make_shared<Airline>(Airline(code)));
}

std::list<std::string> FlightManager::airportinformation(std::string code){
    std::set<std::string> airlines;
    std::set<std::string> countries;
    std::list<std::string> res;
    std::shared_ptr<AirportNode> airport = *airports.find(std::make_shared<AirportNode>(Airport(code)));

    for(auto it=airport->flights.begin();it!=airport->flights.end();it++){
        for(int i = 0;i<it->flights.size();i++){
            airlines.insert((*it).flights[i]->getCode());
        }
    }

    for(auto it=airport->flights.begin();it!=airport->flights.end();it++){
        countries.insert((*it).destination_node->airport.getCountry());
    }

    res.push_back("Code: "+airport->airport.getCode());
    res.push_back("Name: "+airport->airport.getName());
    res.push_back("City: "+airport->airport.getCity()+", "+airport->airport.getCountry());
    res.push_back("Location: "+std::to_string(airport->airport.getCoordinates().first)+" ; "+std::to_string(airport->airport.getCoordinates().second));
    res.push_back("Number of flights from the airport: " + std::to_string(airport->flights.size()));
    res.push_back("Number of airlines: "+std::to_string(airlines.size()));
    res.push_back("Number of countries: "+std::to_string(countries.size()));

    return res;
}

std::set<std::string> FlightManager::airportinformationairlines(std::string code){
    std::set<std::string> airlines;
    std::shared_ptr<AirportNode> airport = *airports.find(std::make_shared<AirportNode>(Airport(code)));

    for(auto it=airport->flights.begin();it!=airport->flights.end();it++){
        for(int i = 0;i<it->flights.size();i++){
            airlines.insert((*it).flights[i]->getCode());
        }
    }

    return airlines;
}

std::set<std::string> FlightManager::airportinformationcountries(std::string code){
    std::set<std::string> countries;
    std::shared_ptr<AirportNode> airport = *airports.find(std::make_shared<AirportNode>(Airport(code)));

    for(auto it=airport->flights.begin();it!=airport->flights.end();it++){
        countries.insert((*it).destination_node->airport.getCountry());
    }

    return countries;
}


std::list<std::string> FlightManager::airportinformationreachable(std::string code, int n){
    std::list<std::string> res;
    std::set<std::string> countries;
    std::set<std::string> cities;
    int nairports=0;

    LookForAirport::bfs(code,this->getAirlines());
    for(auto it = airports.begin();it!=airports.end();it++){
        nairports++;
        countries.insert((*it)->airport.getCountry());
        cities.insert((*it)->airport.getCity());
    }

    res.push_back("Using "+std::to_string(n)+" flights, we can reach:");
    res.push_back(std::to_string(nairports)+" airports");
    res.push_back(std::to_string(cities.size())+" cities");
    res.push_back(std::to_string(countries.size())+" countries");

    return res;
}




std::vector<std::shared_ptr<AirportNode>> FlightManager::cityFlights(std::string src,std::string dest,std::vector<std::shared_ptr<Airline>> possibleairlines){
    std::vector<std::shared_ptr<AirportNode>> res;
    std::vector<std::shared_ptr<AirportNode>> best;
    std::vector<std::shared_ptr<AirportNode>> atual;
    std::vector<std::string> srcairports;
    std::vector<std::string> destairports;

    for(auto ita = airports.begin(); ita != airports.end(); ita++){
        if((*ita)->airport.getCity()==src) srcairports.push_back((*ita)->airport.getCode());
        else if((*ita)->airport.getCity()==dest) destairports.push_back((*ita)->airport.getCode());
        }

    for(int s = 0;s<srcairports.size();s++){
        int mindest = INT_MAX;
        for(int d = 0; d<destairports.size();d++){
            atual=LookForAirport::searchByAirport(srcairports[s],destairports[d],possibleairlines);
            if(atual.size()<=mindest){
                best = atual;
                mindest= atual.size();
            }
        }
        res.insert(res.end(),best.begin(),best.end());
    }

    return res;
}




