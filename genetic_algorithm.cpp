#include "genetic_algorithm.h"


// This function calculates fitness of each chromosome using Euclidean distance.
// cur_fit variable is inverse of distance, so greater this variable, lower the distance and
// fitter the chromosome
double Route::calculateFitness() {
        double totalDistance = 0;
        double cur_fit;
        for (int i = 0; i < cities.size() - 1; ++i) {
            totalDistance += distance(cities[Indices[i]], cities[Indices[i+1]]);
        }

        totalDistance += sqrt(pow(cities[Indices[0]].x,2)+ pow(cities[Indices[0]].y,2));
        totalDistance += sqrt(pow(cities[Indices[Indices.size()-1]].x,2)+ pow(cities[Indices[Indices.size()-1]].y,2));
    cur_fit = 1 / totalDistance;
    return cur_fit;
}


double Route::distance(City &a, City &b) {

    return sqrt(pow(a.x-b.x,2)+ pow(a.y-b.y,2));
}





