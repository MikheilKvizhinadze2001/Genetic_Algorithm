#include "genetic_algorithm.h"
#include <iostream>



// Vector of cities and Mersenne Twister pseudo-random generator is declared here be use them in the functions below:
std::vector<City> cities;
std::mt19937 gen(42); // 42 is the seed
/*
 *  Let us implement two crossover methods:
 *  One common method for crossover is ordered crossover. In this method, a random subset of the first parent is copied to the child,
 *  and the remaining genes are filled with the genes from the second parent in the order they appear,
 *  without duplicating any genes in the selected subset from the first parent.
 */
Route crossOver(const Route &a, const Route &b) {
    std::vector<int> childIndices(a.Indices.size(), -1); // Initialize with -1
    int start = gen() % a.Indices.size();
    int end = gen() % a.Indices.size();
    if (start > end) std::swap(start, end);

    // Copy a subset from the first parent
    for (int i = start; i <= end; ++i) {
        childIndices[i] = a.Indices[i];
    }

    // Fill the remaining indices with the genes from the second parent
    for (int i = 0; i < b.Indices.size(); ++i) {
        if (std::find(childIndices.begin(), childIndices.end(), b.Indices[i]) == childIndices.end()) {
            int j = 0;
            while (childIndices[j] != -1) ++j;
            childIndices[j] = b.Indices[i];
        }
    }

    return Route(cities, childIndices);
}


/*
 *   Another common method is cycle crossover: This method identifies a set of connected subsets between the two parent chromosomes.
 *   These subsets are alternately copied into the child from one parent and from the other parent.
 *   CX is particularly useful for problems where the order of elements matters, such as TSP.
 */

Route cycleCrossover(const Route& parent1, const Route& parent2) {
    int size = parent1.Indices.size();
    std::vector<int> childIndices(size, -1);

    // Initialize a boolean vector to keep track of visited indices
    std::vector<bool> visited(size, false);

    // Start with the first parent
    bool useParent1 = true;

    // Perform Cycle Crossover
    int index = 0;
    while (std::find(childIndices.begin(), childIndices.end(), -1) != childIndices.end()) {
        // Find the next available index
        while (visited[index]) {
            ++index;
            if (index >= size) index = 0;
        }

        // If this index is not visited yet
        if (!visited[index]) {
            // Start a cycle
            int current = index;
            while (!visited[current]) {
                visited[current] = true;

                // Copy the gene from the current parent to the child
                childIndices[current] = (useParent1 ? parent1.Indices[current] : parent2.Indices[current]);

                // Find the corresponding gene in the other parent
                int corresponding = std::find(
                        (useParent1 ? parent2.Indices.begin() : parent1.Indices.begin()),
                        (useParent1 ? parent2.Indices.end() : parent1.Indices.end()),
                        childIndices[current]
                ) - (useParent1 ? parent2.Indices.begin() : parent1.Indices.begin());

                // Move to the corresponding index in the other parent
                current = corresponding;
            }

            // Switch parents for the next cycle
            useParent1 = !useParent1;
        }
    }

    return Route(cities,childIndices);
}



int main() {

    const int numCities = 100;


    // Create a vector of cities

    // Initialize the other cities with random coordinates

    /*
     * In this project, vector of "City" class will contain cities with different (x,y) coordinates.
     * We use integer vector of cityIndices, to create population. In each case, we will shuffle indices
     * and append to this vector. For example, if first two terms of first chromosome will be 32 and 6, it
     * would mean that we would travel from starting location (0,0) coordinate to city[32] and then, to city[6]
     * and we calculate distances, effectively calculating fitness.
     */
    std::vector<int> cityIndices;
    for (int i = 1; i < numCities; ++i) {
        cities.push_back(City(gen));
        cityIndices.push_back(i-1);
    }

    auto shuffleVector = [](std::vector<int>& vec) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vec.begin(), vec.end(), g);
        return vec;
    };

    std::vector<Route> population_ordered_cross;
    std::vector<Route> population_cycle_cross;
    std::vector<int> shuffledCityIndices = shuffleVector(cityIndices);

    for (int i=1;i<100;i++){
        Route current_chromosome(cities,shuffledCityIndices);
        population_ordered_cross.push_back(current_chromosome);
        population_cycle_cross.push_back(current_chromosome);
        shuffledCityIndices = shuffleVector(cityIndices);
    }

    double avg_fitness_before=0;


    for (auto& route : population_ordered_cross) {
        route.fitness= route.calculateFitness();
        avg_fitness_before+=route.fitness;
    }
    std::cout << "Initial Population Fitness: "<<avg_fitness_before/population_ordered_cross.size()<<std::endl;



    const int numIterations = 1000;
    const int tournamentSize = 20;
    //select parents
    //We need two copies of variables, because we are testing both cross-over methos at the same time
    int max_index_parent1_order =0;
    int max_index_parent1_cross =0;
    int max_index_parent2_order=0;
    int max_index_parent2_cross=0;

    int min_index_parent1_order=0;

    int min_index_parent1_cross = 0;



    int min_index_for_population_ordered=0;
    int min_index_for_population_cross=0;

    std::vector<Route> tournament_cross;
    std::vector<Route> tournament_ordered;




// The outer loop runs for a specified number of iterations (generations in the genetic algorithm)
    for (int i=0; i<numIterations; i++) {

        // This loop runs twice for selecting two parents
        for (int k=0; k<2; k++) {

            // If k is 1, it means we are in the second iteration of the parent selection loop
            if (k == 1) {

                // This loop runs for each individual in the tournament
                for (int j = 0; j < tournamentSize; j++) {

                    // Select a random individual from the population
                    int randomIndex = gen() % population_ordered_cross.size();
                    tournament_ordered.push_back(population_ordered_cross[randomIndex]);
                    tournament_cross.push_back(population_cycle_cross[randomIndex]);
                    // If the fitness of the current individual is greater than the fitness of the best individual found so far, update the best individual
                    if (tournament_ordered[j].calculateFitness() > tournament_ordered[max_index_parent1_order].calculateFitness())
                        max_index_parent1_order = j;
                    if (tournament_cross[j].calculateFitness() > tournament_cross[max_index_parent1_cross].calculateFitness())
                        max_index_parent1_cross = j;
                    // If the fitness of the current individual is less than the fitness of the worst individual found so far, update the worst individual and its index in the population
                    if (tournament_ordered[j].calculateFitness() < tournament_ordered[min_index_parent1_order].calculateFitness()){
                        min_index_parent1_order = j;
                        min_index_for_population_ordered = randomIndex;
                    }
                    if (tournament_cross[j].calculateFitness() < tournament_cross[min_index_parent1_cross].calculateFitness()){
                        min_index_parent1_cross = j;
                        min_index_for_population_cross = randomIndex;
                    }
                }
            }
            else { // If k is not 1, it means we are in the first iteration of the parent selection loop

                // This loop runs for each individual in the tournament
                for (int j = 0; j < tournamentSize; j++) {

                    // Select a random individual from the population
                    int randomIndex = gen() % population_ordered_cross.size();
                    tournament_ordered.push_back(population_ordered_cross[randomIndex]);
                    tournament_cross.push_back(population_cycle_cross[randomIndex]);
                    // If the fitness of the current individual is greater than the fitness of the best individual found so far, update the best individual
                    if (tournament_ordered[j].calculateFitness() > tournament_ordered[max_index_parent2_order].calculateFitness())
                        max_index_parent2_order = j;
                    if (tournament_cross[j].calculateFitness() > tournament_cross[max_index_parent2_order].calculateFitness())
                        max_index_parent2_order = j;
                }
            }
        }

        // Perform two crossovers on the best two individuals found in the tournament
        // and replace the worst individual in the population with the offspring
        population_cycle_cross[min_index_for_population_cross] = cycleCrossover(tournament_ordered[max_index_parent1_order],tournament_ordered[max_index_parent2_order]);
        population_ordered_cross[min_index_for_population_ordered] = crossOver(tournament_cross[max_index_parent1_cross],tournament_cross[max_index_parent2_cross]);

        // Reset the indices for the next generation
        max_index_parent2_order=0;
        max_index_parent1_order=0;

        min_index_for_population_ordered=0;
        min_index_for_population_cross=0;

        // Clear the tournament for the next generation
        tournament_ordered.clear();
        tournament_cross.clear();
    }


    // Here we calculate fitness values of both populations after evolution
    double avg_fitness_after_ordered=0;
    double avg_fitness_after_cycle=0;

    for (auto& route : population_ordered_cross) {
        route.fitness= route.calculateFitness();
        avg_fitness_after_ordered+=route.fitness;
    }

    for (auto& route : population_cycle_cross) {
        route.fitness = route.calculateFitness();
        avg_fitness_after_cycle+=route.fitness;
    }


    std::cout << "Final Population Fitness using ordered cross-over: "<<avg_fitness_after_ordered/population_ordered_cross.size()<<std::endl;
    std::cout << "Final Population Fitness using cycle cross-over: "<<avg_fitness_after_cycle/population_ordered_cross.size()<<std::endl;
    return 0;
}
