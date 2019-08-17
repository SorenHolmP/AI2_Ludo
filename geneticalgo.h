#ifndef GENETICALGO_H
#define GENETICALGO_H

#define NUM_INPUTS  5
#define NUM_HIDDEN  10
#define NUM_OUTPUTS 4

#include <random>
#include <iostream>
#include <vector>
#include <time.h>
#include "neuralnet.h"

using namespace std;


class GeneticAlgo
{
public:
    GeneticAlgo();
    GeneticAlgo(int pop_size, double mutation_rate);

    void init_population();
    NeuralNet generate_random_neural_net(int ni, int nh, int no);
    vector<NeuralNet> select_parents(vector<NeuralNet> population, int num_parents);
    void make_new_population(vector<NeuralNet> parents, vector<NeuralNet> &pop);
    NeuralNet crossover(NeuralNet n1, NeuralNet n2);
    void mutate(NeuralNet &net);


private:
    vector<NeuralNet>                   _population;
    uniform_real_distribution<double>   _distribution;
    default_random_engine               _generator;

    double _mutation_rate;
    int    _pop_size;

};

#endif // GENETICALGO_H
