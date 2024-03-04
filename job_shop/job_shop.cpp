#include <algorithm>
#include "job_shop.h"


// This is a source file where most functions are implemented
// For the sake of randomness and variety, this project uses random device instead of a constant seed
// So this code will output different results for each run, but average performance is relatively constant.


// Initialize a random device
std::random_device rd;

// Function to calculate the fitness of a population
double population::calculate_fitness() {

    // If the chromosome is empty, return 0

    if (chromosome.empty())
        return 0;

    int current_delay;
    double total_delay = 0;
    int completion_day = 0;

    // Loop over each job in the chromosome

    for (const auto& s : chromosome){
        // Add the required time of the current job to the completion day
        completion_day += s.required_time;
        // Calculate the delay by subtracting the due date from the completion day
        current_delay = completion_day - s.due_date;
        // If there is a delay, add it to the total delay
        if (current_delay > 0)
            total_delay += current_delay;
    }

    // Return the inverse of the total delay as the fitness
    return 1 / total_delay;
}

// Function to create chromosomes for a population

void population::create_chromosomes() {
    // Initialize a random number generator with a random seed
    std::mt19937 gen(rd());
    // Initialize a uniform distribution between 1 and 1000
    std::uniform_int_distribution<> dis(1, 1000);

    // Set to store unique due dates
    std::set<int> unique_dates;

    // Loop to generate job names and their due dates and required times
    for (char i = 'a'; i <= 'z'; ++i) {
        for (char j = 'a'; j <= 'z'; ++j) {
            job_genetic job;
            // Create a job name from two characters
            job.job_name = std::string(1, i) + std::string(1, j);

            // Ensure unique due dates
            int random_date;
            do {
                random_date = dis(gen);
            }
            while (unique_dates.find(random_date) != unique_dates.end());
            unique_dates.insert(random_date);

            // Assign the random date as the due date of the job
            job.due_date = random_date;
            // Assign a random required time for the job
            job.required_time = dis(gen);

            // Add the job to the chromosome
            chromosome.push_back(job);
        }
    }
}

// Function to shuffle the jobs in a chromosome

void population::shuffle_jobs() {
    // Initialize a random number generator with a random seed
    std::mt19937 g(rd());
    // Shuffle the jobs in the chromosome
    std::shuffle(chromosome.begin(), chromosome.end(), g);
}

// Function to perform crossover between two parents and produce an offspring

population population::cross_over(const population &parent1, const population &parent2) {
    population offspring;

    // Initialize a random number generator with a random seed
    std::mt19937 gen(rd());
    // Initialize a uniform distribution between 0 and the size of the chromosome
    std::uniform_int_distribution<> dis(0, parent1.chromosome.size());

    // Generate a random crossover point
    int cross_over_point = dis(gen);

    // Copy the first part from parent1 to the offspring
    offspring.chromosome.insert(offspring.chromosome.end(), parent1.chromosome.begin(), parent1.chromosome.begin() + cross_over_point);

    // Copy the second part from parent2 to the offspring
    offspring.chromosome.insert(offspring.chromosome.end(), parent2.chromosome.begin() + cross_over_point, parent2.chromosome.end());

    return offspring;
}

// Function to perform tournament selection from a population

population tournament_selection(std::vector<population>& populations_tournament, int tournament_size) {
    // Initialize a random number generator with a random seed
    std::mt19937 gen(rd());
    // Initialize a uniform distribution between 0 and the size of the population
    std::uniform_int_distribution<> dis(0, populations_tournament.size() - 1);

    population best;
    // Loop to select a tournament of individuals
    for (int i = 0; i < tournament_size; ++i) {
        // Select a random individual from the population
        int index = dis(gen);
        // If this is the first individual or if this individual's fitness is better than the current best, update the best
        if (i == 0 or populations_tournament[index].calculate_fitness() > best.calculate_fitness()) {
            best = populations_tournament[index];
        }
    }

    // Return the best individual from the tournament
    return best;
}
