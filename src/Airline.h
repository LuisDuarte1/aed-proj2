#ifndef AIRLINE_H
#define AIRLINE_H

#include <iostream>
#include <memory>

class Airline{

    public:
        /**
         * @brief Construct a new Airline object
         * Complexity: O(1)
         * 
         * 
         * @param code 
         * @param callsign 
         * @param name 
         * @param country 
         */
        Airline(std::string code, std::string callsign, std::string name, std::string country);

        /**
         * @brief Construct a new Airline object, suitable only for comparisions
         * Complexity: O(1)
         * 
         * @param code 
         */
        Airline(std::string code);
        
        /**
         * @brief Get the Code object
         * Complexity: O(1)
         * 
         * 
         * @return std::string 
         */
        std::string getCode() const;

        /**
         * @brief Get the Callsign object
         * Complexity: O(1)
         * 
         * 
         * @return std::string 
         */
        std::string getCallsign() const;

        /**
         * @brief Get the Name object
         * Complexity: O(1)
         * 
         * @return std::string 
         */
        std::string getName() const;

        /**
         * @brief Get the Country object
         * Complexity: O(1)
         * 
         * 
         * @return std::string 
         */
        std::string getCountry() const;

        bool operator==(const Airline& other);


    private:
        std::string code;
        std::string callsign;
        std::string name;
        std::string country;

};


struct HashAirline{
    /**
     * @brief hash function for airlines
     * 
     * @param airline pointer to airline
     * @return std::size_t unique identifier of airline
     */
    std::size_t operator() (const std::shared_ptr<Airline>& airline) const noexcept;
};

#endif