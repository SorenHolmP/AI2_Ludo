#ifndef NEURALNET_H
#define NEURALNET_H

#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

typedef vector<double> vd;
typedef vector<vector<double>> vvd;

class NeuralNet
{
public:
    NeuralNet();
    NeuralNet(vvd WM_IH, vvd WM_HO, int ni, int nh, int no );
    NeuralNet(string filepath);

    void calculate_hidden_outputs();
    void calculate_outputs();
    vd run(vd input);
    double sigmoid(double activation);
    void save_to_file(string filepath);

    /***getters***/
    vvd     &get_WM_IH();
    vvd     &get_WM_HO();
    int     get_num_inputs();
    int     get_num_hidden();
    int     get_num_outputs();
    double  get_fitness();
    double  get_winrate();

    /***setters***/
    void set_fitness(double fitness);
    void set_winrate(double winrate);

private:
    int _num_inputs;
    int _num_hidden;
    int _num_outputs;

    double _fitness;
    double _winrate;

    vd _inputs;
    vd _hidden_outputs;
    vd _outputs;

    vector<vector<double>> _WM_IH;
    vector<vector<double>> _WM_HO;
};

#endif // NEURALNET_H
