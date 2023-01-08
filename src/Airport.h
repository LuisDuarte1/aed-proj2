#ifndef AIRPORT_H
#define AIRPORT_H

#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "Airline.h"



class Airport{
    public:
        /**
         * @brief Construct a new Airport object
         * Complexity: O(1)
         * 
         * 
         * @param code 
         * @param name 
         * @param city 
         * @param country 
         * @param latitude 
         * @param longitude 
         */
        Airport(std::string code, std::string name, std::string city, std::string country, double latitude, double longitude);
        
        /**
         * @brief Construct a new Airport object, should only be used of comparisions
         * Complexity: O(1)
         * 
         * @param code 
         */
        Airport(std::string code);

        /**
         * @brief Get the Code object
         * Complexity: O(1)
         * 
         * 
         * @return std::string 
         */
        std::string getCode() const;
        /**
         * @brief Get the Name object
         * Complexity: O(1)
         * 
         * @return std::string 
         */
        std::string getName() const;
        /**
         * @brief Get the City object
         * Complexity: O(1)
         * 
         * 
         * @return std::string 
         */
        std::string getCity() const;
        /**
         * @brief Get the Country object
         * Complexity: O(1)
         * 
         * 
         * @return std::string 
         */
        std::string getCountry() const;
        /**
         * @brief Get the Coordinates object
         * Complexity: O(1)
         * 
         * 
         * @return std::pair<double,double> 
         */
        std::pair<double,double> getCoordinates() const;

        /**
         * @brief compares only the airport codes
         * Complexity: O(1)
         * 
         * @param airport other airport object
         * @return true if equals
         * @return false if different
         */
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