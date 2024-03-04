#include <algorithm>
#include "job_shop.h"




int main() {

/*
Before moving into main body, let us create sample population and evaluate its fitness:

Sample population table:

Job	Duration (days)	        Due Date
    A	3	                Day 5
    B	5	                Day 6
    C	9	                Day 16
    D	7	                Day 14

Explanation:

Table 2: Delay for Sequence (A, B, C, D)
Job	Completion Day	Delay
A	3	d1 = 3-5 = -2 -> 0
A, B	3+5 = 8	d2 = 8-6 = 2
A, B, C	3+5+9 = 17	d3 = 17 - 16 = 1
A, B, C, D	3+5+9+7 = 24	d4 = 24 - 14 = 10
 T = d1 + d2 + d3 + d4= 0 + 2 + 1 + 10 = 13 days.

*/
    job_genetic a;
    job_genetic b;
    job_genetic c;
    job_genetic d;
    a.due_date = 5;
    b.due_date = 6;
    c.due_date = 16;
    d.due_date = 14;
    a.required_time =3;
    b.required_time = 5;
    c.required_time = 9;
    d.required_time = 7;
    population test_pop;
    test_pop.chromosome.push_back(a);
    test_pop.chromosome.push_back(b);
    test_pop.chromosome.push_back(c);
    test_pop.chromosome.push_back(d);
    std::cout<<"Fitness of sample sequence: 'ABCD': "<<test_pop.calculate_fitness()<<std::endl;

    //Next, let us calculate fitness of optimized sequence, which minimizes total due date
    //As we know from the article mentioned in readme file, optimized sequence is abdc, so:

    test_pop.chromosome[2] = d;
    test_pop.chromosome[3] = c;
    std::cout<<"Fitness of optimized  sequence: 'ABDC': "<<test_pop.calculate_fitness()<<std::endl;



    /*
     * Now let us create population of size 100
     * each member of is called chromosome and will have 26 * 26 = 676 members
     * job names will be 'aa', 'ab' ... 'zy', 'zz', hence the number 676
     * then we create chromosome using create_chromosome() method and
     * we shuffle population using shuffle_jobs() method, and finally, add this element to our vector
     * working principles of both functions are described in source file
     *
     * Let us implement two evolution methods, tournament selection with elitism and
     * sorted method, where we sort population vector by fitness into descending order and
     * replace the least fit individuals with the offspring of the fittest ones.
     */
    const int POPULATION_SIZE = 100;
    std::vector<population> populations;

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population p;
        p.create_chromosomes();
        p.shuffle_jobs();
        populations.push_back(p);
    }

    // Since we implement two different methods on the same population
    // we copy the created population to the new vector

    std::vector<population> populations_tournament = populations;

    // Let us sort population by descending fitness, because we will perform cross_over on
    // fittest individuals and substitute the least fit individuals with offspring of fittest individuals

    std::sort(populations.begin(), populations.end(), [](population a, population b) {
        return a.calculate_fitness() > b.calculate_fitness();
    });



    //Creating necessary variables
    const int MAX_GENERATIONS = 300;  //This can be tuned
    const int MAX_GENERATIONS_WITHOUT_IMPROVEMENT = 70; //This can be tuned
    double previous_fitness;
    double average_fitness=0;
    int generations_without_improvement = 0;
    const double tolerance_parameter = 0.05; //This can be tuned


    // Number of offspring to generate in each generation
    const int NUM_OFFSPRING = 10; //This can be tuned

// Beginning of first implementation, using sort method
for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
    previous_fitness = average_fitness;

    // Check for early stopping
    if (generations_without_improvement >= MAX_GENERATIONS_WITHOUT_IMPROVEMENT) {
        std::cout << "Stopping early due to lack of improvement" << std::endl;
        break;
    }


    // Generate offspring
    for (int i = 0; i < NUM_OFFSPRING; ++i) {
        // Selection
        population parent1 = populations[0];
        population parent2 = populations[1];  // Ensure different parents are selected

        // Crossover
        population new_individual = parent1.cross_over(parent1, parent2);

        // Replacement
        populations[populations.size() - 1 - i] = new_individual;  // Replace from the end
    }


    // Resort the population
    std::sort(populations.begin(), populations.end(), [](population a, population b) {
        return a.calculate_fitness() > b.calculate_fitness();
    });

    // Calculate average fitness
    double total_fitness = 0.0;
    for (population individual : populations) {
        double fitness = individual.calculate_fitness();
        total_fitness += fitness;
    }

    average_fitness = total_fitness / POPULATION_SIZE;

    // Check for improvement
    double current_percent_improvement = 100 * (average_fitness - previous_fitness) / previous_fitness;

    if (current_percent_improvement > tolerance_parameter) {
        generations_without_improvement = 0;
    }
    else {
        ++generations_without_improvement;
    }

    if (generation == 0 or (generation+1) % 10 == 0)
        std::cout << "Sort method: Generation " << generation +1 << ": average fitness = " << average_fitness << std::endl;
}


    // Let us update this variable, so we can use it for the second method
    generations_without_improvement = 0;

    // From here, we implement evolution using tournament selection and elitism

    const int TOURNAMENT_SIZE = 50;


    std::random_device rd;
    std::mt19937 gen(rd());

    const int ELITISM_COUNT = 5;  // Number of elites


    int min = 0;
    int max = populations.size()-1;
    std::uniform_int_distribution<int> dis(min, max);
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        // Calculate average fitness of the current generation
        double total_fitness = 0.0;
        for (population individual : populations_tournament) {
            double fitness = individual.calculate_fitness();
            total_fitness += fitness;
        }
        previous_fitness = average_fitness;
        average_fitness = total_fitness / POPULATION_SIZE;



        // Preserve the elites
        std::vector<population> elites = populations_tournament;  // Copy the current population



        std::sort(elites.begin(), elites.end(), [](population a, population b) {
            return a.calculate_fitness() > b.calculate_fitness();
        });
        elites.resize(ELITISM_COUNT);  // Keep only the top individuals

        // Generate offspring
        for (int i = 0; i < NUM_OFFSPRING; ++i) {
            // Selection
            population parent1 = tournament_selection(populations_tournament, TOURNAMENT_SIZE);
            population parent2 = tournament_selection(populations_tournament, TOURNAMENT_SIZE);

            // Crossover
            population new_individual = parent1.cross_over(parent1, parent2);

            // Replacement
            int index = dis(gen);  // Select a random individual to replace
            populations_tournament[index] = new_individual;
        }

        // Replace the worst individuals with the elites
        for (int i = 0; i < ELITISM_COUNT; ++i) {
            int index = dis(gen);  // Select a random individual to replace
            populations_tournament[index] = elites[i];
        }



        // Check for improvement
        double current_percent_improvement = 100 * (average_fitness - previous_fitness) / previous_fitness;
        if (current_percent_improvement > tolerance_parameter) {
            generations_without_improvement = 0;
        } else {
            ++generations_without_improvement;
        }
        // Check for early stopping
        if (generations_without_improvement >= MAX_GENERATIONS_WITHOUT_IMPROVEMENT) {
            std::cout << "Stopping early due to lack of improvement" << std::endl;
            break;
        }
        if (generation == 0 or (generation+1) % 10 == 0)
            std::cout << "Tournament selection: Generation " << generation +1 << ": average fitness = " << average_fitness << std::endl;
    }





    return 0;
}