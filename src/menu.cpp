#include <iostream>
#include "menu.h"
#include "DistanceAirportFinder.h"
#include "LookForAirport.h"

void menu::inicio() {
    std::cout << "Welcome to the Flight Search System!\n"
                 "Our menu will work based on number inputs! Example: For option 1, please insert as input '1' on console\n"
                 "So let's start:\n"
                 "1-Search the best route from a Airport/Country/Coordinate to another of the same type (with or without flight agency restriction)\n"
                 "2-Statistics about the airline network or a specific airport;\n\n";
    int ans;
    std::cin >> ans;
    std::cout << "\n";
    switch (ans) {
        case 1:
            menu::SearchRoute();
            break;
        case 2:
            Statistics::statisticsMenu();
            break;
        default:
            std::cout<<"Input Not Available, please try again.\n";
            menu::inicio();
            break;
    }
}
void menu::SearchRoute(){
    std::cout << "Please specify the terms of search:\n"
                 "1-Specific Airport\n"
                 "2-Country\n"
                 "3-Coordinates\n\n";
    int ans;
    std::cin >> ans;
    std::cout << "\n";
    switch (ans) {
        case 1:
            LookForAirport::display();
            break;
        case 2:
            //INSERIR PROCURA POR PAIS
            break;
        case 3:
            DistanceAirportFinder::display();
            break;
        default:
            std::cout<<"Input Not Available, please try again.\n";
            menu::SearchRoute();
            break;
    }


}
