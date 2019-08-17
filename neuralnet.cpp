#include "neuralnet.h"

NeuralNet::NeuralNet()
{

}

NeuralNet::NeuralNet(vvd WM_IH, vvd WM_HO, int ni, int nh, int no): _WM_IH(WM_IH), _WM_HO(WM_HO)
{
    _num_inputs     = ni;
    _num_hidden     = nh;
    _num_outputs    = no;

    _inputs.resize(ni);
    _hidden_outputs.resize(nh);
    _outputs.resize(no);
}

NeuralNet::NeuralNet(string filepath)
{
    ifstream in_file(filepath);
    if(!in_file)
        cerr << "Failed to load NeuralNetwork file: " << filepath << endl;

    in_file >> _num_inputs;
    in_file >> _num_hidden;
    in_file >> _num_outputs;

    _inputs.resize(_num_inputs);
    _hidden_outputs.resize(_num_hidden);
    _outputs.resize(_num_outputs);

    /***Setup Input to hidden layer weights***/
    for(int i = 0; i < _num_inputs; i++)
    {
        vd IH; //Contains the weights corresponding from 1 input to all hidden neurons
        for(int j = 0; j < _num_hidden; j++)
        {
            double random_weight;
            in_file >> random_weight;
            IH.push_back(random_weight);
        }
        _WM_IH.push_back(IH);
    }

    /***Setup hidden layer to output weights***/
    for(int i = 0; i < _num_hidden; i++)
    {
        vd HO;
        for(int j = 0; j < _num_outputs; j++)
        {
            double random_weight;
            in_file >> random_weight;
            HO.push_back(random_weight);
        }
        _WM_HO.push_back(HO);
    }
    in_file.close();
}

void NeuralNet::save_to_file(string filepath)
{
    ofstream out_file(filepath);
    if(!out_file)
        cerr << "Could not open: " << filepath << " for writing" << endl;

    out_file << _num_inputs << endl;
    out_file << _num_hidden << endl;
    out_file << _num_outputs << endl;

    for(int i = 0; i < _num_inputs; i++)
        for(int j = 0; j < _num_hidden; j++)
            out_file << _WM_IH[i][j] << endl;

    for(int i = 0; i < _num_hidden; i++)
        for(int j = 0; j < _num_outputs; j++)
            out_file << _WM_HO[i][j] << endl;

    out_file.close();


}

void NeuralNet::calculate_hidden_outputs()
{
    double activation;
    for(int i = 0; i < _num_hidden; i++)
    {
        activation = 0;
       for(int j = 0; j < _num_inputs; j++)
        {
            activation += _inputs[j] * _WM_IH[j][i];
        }
        _hidden_outputs[i] = sigmoid(activation);
    }
}

void NeuralNet::calculate_outputs()
{
    double activation;
    for(int i = 0; i < _num_outputs; i++)
    {
        activation = 0;
        for(int j = 0; j < _num_hidden; j++)
        {
            activation += _hidden_outputs[j] * _WM_HO[j][i];
        }
        _outputs[i] = sigmoid(activation);
    }
}

vd NeuralNet::run(vd inputs)
{
    _inputs = inputs;
    calculate_hidden_outputs();
    calculate_outputs();

    return _outputs;

}

double NeuralNet::sigmoid(double activation)
{
    return 1/(1 + exp(-activation));
}

/***getters***/
vvd     &NeuralNet::get_WM_IH()         {return _WM_IH;}
vvd     &NeuralNet::get_WM_HO()         {return _WM_HO;}
int     NeuralNet::get_num_inputs()     {return _num_inputs;}
int     NeuralNet::get_num_hidden()     {return _num_hidden;}
int     NeuralNet::get_num_outputs()    {return _num_outputs;}
double  NeuralNet::get_fitness()        {return _fitness;}
double  NeuralNet::get_winrate()        {return _winrate;}




/***setters***/
void NeuralNet::set_fitness(double fitness) {_fitness = fitness;}
void NeuralNet::set_winrate(double winrate) {_winrate = winrate;}

