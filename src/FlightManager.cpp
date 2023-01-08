#include "FlightManager.h"
#include <fstream>
#include <sstream>

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
        (*it)->prev = null;
        (*it)->dist = -1;
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
void FlightManager::resetdistanceAirports() {
    static const std::shared_ptr<AirportNode> null = std::shared_ptr<AirportNode>(nullptr);
    for (auto it = airports.begin(); it != airports.end(); it++) {
        (*it)->dist = -1;
    }
}