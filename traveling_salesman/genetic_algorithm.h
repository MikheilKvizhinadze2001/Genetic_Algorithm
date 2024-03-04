#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

// Include necessary libraries
#include <utility>
#include <vector>
#include <set>
#include <utility>
#include <random>
#include <algorithm>

// Define the City class
class City {
    // Define the maximum x and y coordinates for a city
    const int maxX = 100;
    const int maxY = 100;

    // A set to store used coordinates to ensure no two cities have the same coordinates
    // So it would become more challenging for our algorithm to make improvements
    std::set<std::pair<int, int>> usedCoords;

public:
    // The x and y coordinates of the city
    int x, y;


    City(std::mt19937& gen) {
        // Insert the origin (0, 0) into the set of used coordinates
        // Because starting location is always assumed to be (0,0)
        usedCoords.insert({0, 0});

        // Define uniform distributions for the x and y coordinates
        std::uniform_int_distribution<> disX(0, maxX);
        std::uniform_int_distribution<> disY(0, maxY);

        // Generate random x and y coordinates for the city
        // If the generated coordinates are already used, generate new ones
        do {
            x = disX(gen);
            y = disY(gen);
        } while (usedCoords.count({x, y}) > 0);

        // Insert the generated coordinates into the set of used coordinates
        usedCoords.insert({x, y});
    }
};

// Define the Route class
class Route {
public:
    // A vector to store the cities in the route
    std::vector<City> cities;

    // The fitness of the route
    double fitness;

    // A vector to store the indices of the cities in the route
    std::vector<int> Indices;

    // Constructor for the Route class
    // Takes a vector of all cities and a vector of city indices as arguments
    Route(const std::vector<City>& allCities, std::vector<int> cityIndices){
        // Copy all cities to the cities vector
        for (const auto & s : allCities)
            cities.push_back(s);

        // Move the city indices to the Indices vector
        Indices = std::move(cityIndices);
    }

    // Method to calculate the fitness of the route
    double calculateFitness();

    // Method to calculate the distance between two cities
    double distance(City& a, City& b);
};






#endif // GENETIC_ALGORITHM_H
