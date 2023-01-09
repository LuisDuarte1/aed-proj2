#include "Statistics.h"
#include <algorithm>
#include <set>
#include <unordered_map>
#include <queue>

void Statistics::topAirports(){
    std::cout << "Get top airports by:\n"
        "1 - Num of flights\n"
        "2 - Num of airlines that departure from that airport\n"
        "3 - Go back\n"
        "Selection: ";
    
    int selection;
    if(!(std::cin >> selection).good()){
        std::cout << "Invalid selection... try again... \n";
        return topAirports();
    }

    std::cout << "Number of airports to be displayed: ";
    int k;
    if(!(std::cin >> k).good()){
        std::cout << "Invalid number... try again... \n";
        return topAirports();
    }

    FlightManager * flightManager = FlightManager::getInstance();
    auto airports = flightManager->getAirports();

    std::vector<std::shared_ptr<AirportNode>> topKAirports = {};
    switch(selection){
        case 1:
            for(auto it = airports.begin(); it != airports.end(); it++){
                topKAirports.push_back(*it);
            } 
            std::sort(topKAirports.begin(), topKAirports.end(), 
            [](std::shared_ptr<AirportNode> a1, std::shared_ptr<AirportNode> a2){
                int a1acc = 0;
                for(Flight f : a1->flights){
                    a1acc += f.flights.size();
                }
                int a2acc = 0;
                for(Flight f : a2->flights){
                    a2acc += f.flights.size();
                }
                return a1acc > a2acc;
            });
            for(int i = 0; i < k && i < topKAirports.size(); i++){
                std::cout << i+1 << " - " << topKAirports[i]->airport.getCode() << " " << topKAirports[i]->airport.getName() << "\n";
            }
            break;
        case 2:
            for(auto it = airports.begin(); it != airports.end(); it++){
                topKAirports.push_back(*it);
            } 
            std::sort(topKAirports.begin(), topKAirports.end(), 
            [](std::shared_ptr<AirportNode> a1, std::shared_ptr<AirportNode> a2){
                std::set<std::shared_ptr<Airline>> airlinesa1;
                for(Flight f : a1->flights){
                    for (std::shared_ptr<Airline> airlinepog: f.flights){

                        airlinesa1.insert(airlinepog);
                    }
                }
                std::set<std::shared_ptr<Airline>> airlinesa2;

                for(Flight f : a2->flights){
                    for (std::shared_ptr<Airline> airlinepog: f.flights){

                        airlinesa2.insert(airlinepog);
                    }
                }
                return airlinesa1.size() > airlinesa2.size();
            });
            for(int i = 0; i < k && i < topKAirports.size(); i++){
                std::cout << i+1 << " - " << topKAirports[i]->airport.getCode() << " " << topKAirports[i]->airport.getName() << "\n";
            }
            break;
        case 3:return;

    }
}


void Statistics::topCountry(){
    std::cout << "Get top countries by:\n"
        "1 - Number of airports\n"
        "2 - Number of airlines\n"
        "3 - Go back\n"
        "Selection: ";
    int selection;
    if(!(std::cin >> selection).good()){
        std::cout << "Invalid selection... try again... \n";
        return topCountry();
    }

    std::cout << "Number of countries to be displayed: ";
    int k;

    if(!(std::cin >> k).good()){
        std::cout << "Invalid number... try again... \n";
        return topCountry();
    }
    std::unordered_map<std::string,int> countries;
    FlightManager * flightManager = FlightManager::getInstance();
    auto airports = flightManager->getAirports();
    auto airlines = flightManager->getAirlines();
    std::vector<std::pair<std::string, int>> tovec;

    switch (selection)
    {
    case 1:
        for(auto it = airports.begin(); it != airports.end(); it++){
            std::string pog = (*it)->airport.getCountry();


            auto f = countries.find(pog);
            if(f != countries.end()){
                f->second++;
            }
            else {
                countries.insert({pog, 1});
            }
        }


        for(const auto& i: countries){
            tovec.push_back(i);
        }

        std::sort(tovec.begin(), tovec.end(), [](auto i1, auto i2){
            return i1.second > i2.second;
        });

        for(int i = 0; i < k && i < tovec.size(); i++){
            std::cout << i+1 << " - " << tovec[i].first << "\n";
        }
        break;
    
    case 2:
        for(auto it = airlines.begin(); it != airlines.end(); it++){
            std::string pog = (*it)->getCountry();


            auto f = countries.find(pog);
            if(f != countries.end()){
                f->second++;
            }
            else {
                countries.insert({pog, 1});
            }
        }


        for(const auto& i: countries){
            tovec.push_back(i);
        }

        std::sort(tovec.begin(), tovec.end(), [](auto i1, auto i2){
            return i1.second > i2.second;
        });

        for(int i = 0; i < k && i < tovec.size(); i++){
            std::cout << i+1 << " - " << tovec[i].first << "\n";
        }
        break;
    case 3:
        return;
    }
}


int Statistics::bfsDist(std::shared_ptr<AirportNode> node, std::shared_ptr<AirportNode> end){
    FlightManager * flightManager = FlightManager::getInstance();

    std::queue<std::shared_ptr<AirportNode>> queue;

    flightManager->resetVisitedAirports();

    node -> dist = 0;

    queue.push(node);



    while(queue.size() != 0){
        auto i = queue.front();
        queue.pop();

        for(Flight f : i->flights){
            if(!f.destination_node->visited){
                f.destination_node->dist = i->dist + 1;
                f.destination_node->visited = true;
                if(*f.destination_node == *end) return f.destination_node->dist;
                queue.push(f.destination_node);
            }
        }
    }
    return -1; //no path found
}

std::shared_ptr<AirportNode> Statistics::bfsMax(std::shared_ptr<AirportNode> node){

    FlightManager * flightManager = FlightManager::getInstance();

    std::queue<std::shared_ptr<AirportNode>> queue;

    flightManager->resetVisitedAirports();

    node -> dist = 0;

    std::shared_ptr<AirportNode> max = node;
    queue.push(node);

    while(queue.size() != 0){
        auto i = queue.front();
        queue.pop();

        for(Flight f : i->flights){
            if(!f.destination_node->visited){
                f.destination_node->dist = i->dist + 1;
                f.destination_node->visited = true;
                max = f.destination_node;
                queue.push(f.destination_node);
            }
        }
    }
    return max;
}

void Statistics::diameter(){
    FlightManager * flightManager = FlightManager::getInstance();
    auto airports = flightManager->getAirports();



    auto f = bfsMax(*airports.begin());

    auto i = bfsMax(f);

    int max = bfsDist(i,f);
    

    std::cout << "Diameter of network: "<<max << "\n";


}



void Statistics::statisticsMenu(){
    std::cout << "\n\nStatistics:\n"
        "1 - Top airports\n"
        "2 - Top countries\n"
        "3 - Diameter of network\n"
        "4 - Go back\n"
        "Selection:";

    int selection;
    if(!(std::cin >> selection).good()){
        std::cout << "Invalid selection...\n";
        return statisticsMenu();
    }
    std::cout << "\n\n";

    switch (selection){
        case 1:
            topAirports();
            break;
        case 2:
            topCountry();
            break;
        case 3:
            diameter();
            break;
        case 4: return;
    }
    return statisticsMenu();
}