#include "Airport.h"



Airport::Airport(std::string code, std::string name, std::string city, std::string country, double latitude, double longitude)
    : code(code), name(name), city(city), country(country), latitude(latitude), longitude(longitude){

    }


Airport::Airport(std::string code) : code(code){

}

std::string Airport::getCode() const{return code;}

std::string Airport::getName() const{return name;}

std::string Airport::getCity() const{return city;}

std::string Airport::getCountry() const{return country;}

std::pair<double,double> Airport::getCoordinates() const{return std::make_pair(latitude, longitude);}

bool Airport::operator==(const Airport& airport) const{
    return code == airport.code;
}



