#ifndef AIRLINE_H
#define AIRLINE_H

#include <iostream>
#include <memory>

class Airline{

    public:
        Airline(std::string code, std::string callsign, std::string name, std::string country);

        Airline(std::string code);
        
        std::string getCode() const;

        std::string getCallsign() const;

        std::string getName() const;

        std::string getCountry() const;

        bool operator==(const Airline& other);


    private:
        std::string code;
        std::string callsign;
        std::string name;
        std::string country;

};


struct HashAirline{
    std::size_t operator() (const std::shared_ptr<Airline>& airline) const noexcept;
};

#endif