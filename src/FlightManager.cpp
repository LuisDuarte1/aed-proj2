#include "FlightManager.h"
#include "LookForAirport.h"
#include "DistanceAirportFinder.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <set>
#include <climits>
#include <algorithm>

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
    }
}
void FlightManager::resetdistanceAirports() {
    static const std::shared_ptr<AirportNode> null = std::shared_ptr<AirportNode>(nullptr);
    for (auto it = airports.begin(); it != airports.end(); it++) {
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

std::vector<std::string> FlightManager::airportinformation(std::string code){
    std::set<std::string> airlines;
    std::set<std::string> countries;
    std::vector<std::string> res;
    std::shared_ptr<AirportNode> airport = *airports.find(std::make_shared<AirportNode>(Airport(code)));

    for(auto it=airport->flights.begin();it!=airport->flights.end();it++){
        for(int i = 0;i<it->flights.size();i++){
            airlines.insert((*it).flights[i]->getCode());
        }
    }

    for(auto it=airport->flights.begin();it!=airport->flights.end();it++){
        countries.insert((*it).destination_node->airport.getCountry());
    }

    res.push_back(airport->airport.getCode());
    res.push_back(airport->airport.getName());
    res.push_back(airport->airport.getCity()+", "+airport->airport.getCountry());
    res.push_back(std::to_string(airport->airport.getCoordinates().first)+" ; "+std::to_string(airport->airport.getCoordinates().second));
    res.push_back(std::to_string(airport->flights.size()));
    res.push_back(std::to_string(airlines.size()));
    res.push_back(std::to_string(countries.size()));

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


std::vector<std::string> FlightManager::airportinformationreachable(std::string code, int n){
    std::vector<std::string> res;
    std::set<std::string> countries;
    std::set<std::string> cities;
    int nairports=0;
    std::vector<std::shared_ptr<Airline>> airlines;

    LookForAirport::bfs(code,airlines);

    for(auto it = airports.begin();it!=airports.end();it++){
        if((*it)->dist<=n && (*it)->dist!=-1){
            nairports++;
            countries.insert((*it)->airport.getCountry());
            cities.insert((*it)->airport.getCity());
        }
    }

    res.push_back(std::to_string(nairports));
    res.push_back(std::to_string(cities.size()));
    res.push_back(std::to_string(countries.size()));

    return res;
}


void FlightManager::displayairportinformation() {

    std::string airportCode;
    std::cout << "Input the airport code: ";
    if (!(std::cin >> airportCode).good()) {
        std::cout << "Invalid airport code... try again.\n";
        return displayairportinformation();
    }

    std::vector<std::string> res = airportinformation(airportCode);

    std::cout <<"Code: "+res[0]+"\n";
    std::cout <<"Name: "+res[1]+"\n";
    std::cout <<"City: "+res[2]+"\n";
    std::cout <<"Location: "+res[3]+"\n";
    std::cout <<"Number of flights from the airport: " + res[4]+"\n";
    std::cout <<"Number of airlines from the airport: "+res[5]+"\n";
    std::cout <<"Number of countries directly reachable: "+res[6]+"\n";
    std::cout <<"To see the airlines type 'airlines', else, type 'no' ";

    std::string airline;
    std::cin>>airline;
    if(airline=="airlines"){
        std::set<std::string> airportairlines = airportinformationairlines(airportCode);
        for(auto it = airportairlines.begin();it!=airportairlines.end();it++){
            std::cout<<*it<<"\n";
        }
    }

    std::cout <<"To see the countries directly reachable type 'countries', else, type 'no' ";
    std::string country;
    std::cin>>country;
    if(country=="countries"){
        std::set<std::string> airportcountries = airportinformationcountries(airportCode);
        for(auto it = airportcountries.begin();it!=airportcountries.end();it++){
            std::cout<<*it<<"\n";
        }
    }

    std::string sn;
    int n;
    std::cout << "To see what can be reached with n flights from the airport type the number of you wish, else, type 'exit' ";

    
    if (!(std::cin >> n).good()){
        std::cout << "Invalid number... \n";
        return;
    }

    res = airportinformationreachable(airportCode,n);

    std::cout<<"Using "<< n <<" flights, you can reach:\n";
    res[0]+" airports\n";
    res[1]+" cities\n";
    res[2]+" countries\n";

}


std::stack<std::shared_ptr<AirportNode>> tracebackPathA(std::shared_ptr<AirportNode> destination){
    std::stack<std::shared_ptr<AirportNode>> path;
    std::shared_ptr<AirportNode> i = destination;
    while(i.get() != nullptr){
        path.push(i);
        i = i->prev;
    }
    return path;
}

std::vector<std::stack<std::shared_ptr<AirportNode>>> FlightManager::cityFlights(std::string src,std::string dest,std::vector<std::shared_ptr<Airline>> possibleairlines){
    std::vector<std::stack<std::shared_ptr<AirportNode>>> best;

    std::vector<std::string> srcairports;
    std::vector<std::string> destairports;
    int dist=INT_MAX;
    std::shared_ptr<AirportNode> airport;

    for(auto ita = airports.begin(); ita != airports.end(); ita++){
        if((*ita)->airport.getCity()==src) srcairports.push_back((*ita)->airport.getCode());
        else if((*ita)->airport.getCity()==dest) destairports.push_back((*ita)->airport.getCode());
    }

    for(int s = 0;s<srcairports.size();s++){
        LookForAirport::bfs(srcairports[s],possibleairlines);
        for(int d = 0; d<destairports.size();d++){
            airport= *airports.find(std::make_shared<AirportNode>(Airport(destairports[d])));
            if(airport->dist<dist && airport->dist != -1){
                best.clear();
                dist = airport->dist;
                best.push_back(tracebackPathA(airport));
            }
            else if(airport->dist==dist){
                best.push_back(tracebackPathA(airport));
            }
        }
    }

    return best;
}


std::vector<std::shared_ptr<Airline>> intersectAirlinesA(std::vector<std::shared_ptr<Airline>>& flights, std::vector<std::shared_ptr<Airline>>& airlines){
    std::vector<std::shared_ptr<Airline>> intersection;
    std::set_intersection(flights.begin(), flights.end(),
        airlines.begin(), airlines.end(), std::back_inserter(intersection));
    return intersection;
}

std::vector<std::shared_ptr<Airline>> getPossibleAirlinesA(std::shared_ptr<AirportNode>& src, 
    std::shared_ptr<AirportNode>& dst, std::vector<std::shared_ptr<Airline>>& possibleAirlines){
    
    for(Flight& flight : src->flights){
        if((*flight.destination_node) == (*dst)){
            if(possibleAirlines.size() == 0) return flight.flights;
            return intersectAirlinesA(flight.flights, possibleAirlines);
        }
    }
    return {};
}

std::string toStringPossibleAirlinesA(std::vector<std::shared_ptr<Airline>> airlines){
    std::string r = "Airlines: ";
    if(airlines.size() == 0) return "Error: no possible airlines?";
    for(auto it = airlines.begin(); it != --airlines.end(); it++){
        r += (*it)->getCode();
        r += " , ";
    }
    r += airlines[airlines.size()-1]->getCode();
    return r;
}


void FlightManager::displaycityflight(){
    std::string srcCity;
    std::string destCity;
    std::cout << "Input the departure city: ";
    std::cin >> srcCity;
    std::cout << "Input the arrival city: ";
    std::cin >> destCity;

    std::string airlines_restriction;
    std::cout << "Input the airlines that you want to travel (write 'none' for no restriction, seperator ,): ";
    std::cin >> airlines_restriction;

    std::vector<std::stack<std::shared_ptr<AirportNode>>> result;

    std::vector<std::shared_ptr<Airline>> airlines;

    if(airlines_restriction == "none"){
        result = cityFlights(srcCity,destCity,{});
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
                return displaycityflight();
            }
            airlines.push_back(i);
        }
        std::shared_ptr<Airline> i = flightManager->getAirline(buf);
        if(i.get() == nullptr){
            std::cout << buf << "is not a valid airline... try again.\n";
            return displaycityflight();
        }
        airlines.push_back(i);
        result = cityFlights(srcCity,destCity,airlines);
    }
    for(auto& path: result){
        std::shared_ptr<AirportNode> last;
        while(path.size() != 1){
            std::shared_ptr<AirportNode> i = path.top();
            path.pop();
            std::cout << "\t"<<i->airport.getCode() << "\n\t" << i->airport.getName() << "\n";
            std::cout << "\t|\n\t| " << toStringPossibleAirlinesA(getPossibleAirlinesA(i, path.top(), airlines)) <<"\n\t|\n\t|\n";
            last = i;
        }
        std::shared_ptr<AirportNode> i = path.top();
        std::cout << "\t" <<i->airport.getCode() << "\n\t" << i->airport.getName() << "\n";
        std::cout << "\n\n\n--------------------\n\n\n";
    }

}

