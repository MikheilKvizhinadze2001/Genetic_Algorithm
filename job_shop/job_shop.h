#ifndef JOB_SHOP_H
#define JOB_SHOP_H
#include <iostream>
#include <vector>
#include <random>
#include <set>


/*
 *  This header file declares functions and creates classes.
 *  In our project, each member of class "population" will always contain vector class 'job_genetic'
 *  called 'chromosome'. each member will have unique job name and randomly generated due date and required time
 *  one limitation is that due_dates must not be same, which is taken care in the source file
 *  when we create vector of populations and shuffle them, we effectively shuffle vector of chromosomes, so we shuffle
 *  the order in which jobs will be performed.
 */
class job_genetic{
    public:
        std::string job_name;
        int due_date;
        int required_time;
};

class population : public job_genetic{

public:
    std::vector<job_genetic> chromosome;


    void create_chromosomes();
    double calculate_fitness();
    static population cross_over(const population &parent1,const population &parent2);
    void shuffle_jobs();

};

population tournament_selection(std::vector<population>& populations_tournament, int tournament_size);



#endif //JOB_SHOP_H
