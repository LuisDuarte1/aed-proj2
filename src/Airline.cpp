#include "Airline.h"


Airline::Airline(std::string code, std::string callsign, std::string name, std::string country)
    : code(code), callsign(callsign), name(name), country(country){

}

Airline::Airline(std::string code): code(code){

}


std::string Airline::getCode() const { return code; }

std::string Airline::getCallsign() const { return callsign; }

std::string Airline::getName() const { return name; }

std::string Airline::getCountry() const { return country; }

bool Airline::operator==(const Airline& other){
    return code==other.code;
}


std::size_t HashAirline::operator() (const std::shared_ptr<Airline>& airline) const noexcept{
    return std::hash<std::string>{}(airline->getCode());
}

