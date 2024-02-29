# Genetic Algorithm for the Traveling Salesman Problem

This project implements a genetic algorithm to solve the Traveling Salesman Problem (TSP). The TSP is a classic algorithmic problem in the field of computer science and operations research which focuses on optimization. In this problem, a salesman is given a list of cities, and must determine the shortest route that allows him to visit each city once and return to his original location.

![image](https://github.com/MikheilKvizhinadze2001/rock_paper_scissors/assets/85734592/9e639ceb-0925-4a4e-8063-f7ee635bf958)


## Genetic Algorithm

A genetic algorithm is a search heuristic that is inspired by Charles Darwin’s theory of natural evolution. This algorithm reflects the process of natural selection where the fittest individuals are selected for reproduction in order to produce the offspring of the next generation.

The process of a genetic algorithm can be broken down into the following steps:

1. **Initialization:** Generate an initial population of individuals. In the context of the TSP, an individual (also known as a chromosome) is a complete route that visits each city once.

2. **Selection:** Evaluate the fitness of each individual in the population. The fitness function determines how fit an individual is (the ability of an individual to compete with other individuals). It gives a fitness score to each individual. In the case of the TSP, the fitness score can be calculated as the inverse of the total distance traveled in the route. The lower the distance, the higher the fitness score.

3. **Crossover:** The idea behind crossover (also called recombination) is to take more than one parent solutions and produce a child solution from them. We have used two types of crossover methods in this project: ordered crossover and cycle crossover.

    - **Ordered Crossover (OX):** A subset of consecutive genes from parent 1 is combined with the remaining genes from parent 2 in their original order.
    - **Cycle Crossover (CX):** It constructs the offspring in such a way that each city (gene) comes from one parent and in the same position as in that parent.

4. **Replacement:** The new population is used in the next iteration of the algorithm. In case of generational replacement, all individuals are replaced by their offspring, whereas in steady-state replacement, only a portion of the population is replaced.

## Results:


    <img width="380" alt="image" src="https://github.com/MikheilKvizhinadze2001/traveling_salesman_problem/assets/85734592/41623594-7162-47ad-aeb8-aa8052d7c0a5">

    
As we see, after 1000 iterations, both crossover methods improved the performance by 40%, which is significant improvement. Also the cycle crossover (CX) method performed better, which can be attributed to its ability to preserve more "relative position" information from the parent routes compared to the Ordered Crossover (OX) method.
In the context of the Traveling Salesman Problem (TSP), the "relative position" of cities in a route can be crucial. This is because the order in which cities are visited can significantly impact the total distance of the route. By preserving more of this relative position information, the CX method is able to generate offspring that inherit more of the advantageous characteristics of their parents, leading to a higher likelihood of producing better solutions.

## Conclusion

This project demonstrates the power of genetic algorithms to solve optimization problems. Despite the simplicity of the algorithm, it is able to find good solutions to the TSP, a problem that is notoriously difficult to solve using traditional optimization methods. With further tuning of the parameters and additional enhancements (such as mutation and more sophisticated selection methods), the performance of the genetic algorithm could be further improved.
