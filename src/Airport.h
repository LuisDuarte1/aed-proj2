#ifndef AIRPORT_H
#define AIRPORT_H

#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "Airline.h"



class Airport{
    public:
        Airport(std::string code, std::string name, std::string city, std::string country, double latitude, double longitude);
        Airport(std::string code);


        std::string getCode() const;
        std::string getName() const;
        std::string getCity() const;
        std::string getCountry() const;
        std::pair<double,double> getCoordinates() const;

        bool operator==(const Airport& airport) const;


        
        



    private:
        std::string code;
        std::string name;
        std::string city;
        std::string country;
        double latitude;
        double longitude;

        
};



#endif