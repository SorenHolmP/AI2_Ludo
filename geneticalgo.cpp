#include "geneticalgo.h"

GeneticAlgo::GeneticAlgo()
{

}

GeneticAlgo::GeneticAlgo(int pop_size, double mutation_rate):
    _distribution(uniform_real_distribution<double>(-1,1)), _generator(default_random_engine(time(NULL)))
{
    /***Uniform random generator in range [-1,1]***/
    srand(time(NULL));
    _pop_size       = pop_size;
    _mutation_rate  = mutation_rate;
    init_population();
}

void GeneticAlgo::init_population()
{
    for(int i = 0; i < _pop_size; i++)
    {
        NeuralNet net = generate_random_neural_net(NUM_INPUTS, NUM_HIDDEN, NUM_OUTPUTS);
        _population.push_back(net);
    }
}

NeuralNet GeneticAlgo::generate_random_neural_net(int ni, int nh, int no)
{
    vvd WM_IH, WM_HO;

    /***Setup Input to hidden layer weights***/
    for(int i = 0; i < ni; i++)
    {
        vd IH; //Contains the weights corresponding from 1 input to all hidden neurons
        for(int j = 0; j < nh; j++)
        {
            double random_weight = _distribution(_generator);
            IH.push_back(random_weight);
        }
        WM_IH.push_back(IH);
    }

    /***Setup hidden layer to output weights***/
    for(int i = 0; i < nh; i++)
    {
        vd HO;
        for(int j = 0; j < no; j++)
        {
            double random_weight = _distribution(_generator);
            HO.push_back(random_weight);
        }
        WM_HO.push_back(HO);
    }

    return NeuralNet(WM_IH, WM_HO, ni, nh, no);
}

vector<NeuralNet> GeneticAlgo::select_parents(vector<NeuralNet> population, int num_parents)
{
    /***Select best one based on fitness***/
    vector<NeuralNet> parents;
    for(int k = 0; k < num_parents; k++)
        {
            double best_fitness = population[0].get_fitness();
            double best_idx = 0;
            for(int i = 1; i < population.size(); i++)
                if(population[i].get_fitness() > best_fitness)
                {
                    best_fitness = population[i].get_fitness();
                    best_idx = i;
                }
            parents.push_back(population[best_idx]);
            population.erase(population.begin() + best_idx);
        }

    /***Roulette***/
//    vector<NeuralNet> parents;
//    vector<int> roulette;
//    for(int i = 0; i < population.size(); i++)
//        for(int j = 0; j < population[i].get_fitness(); j++)
//            roulette.push_back(i);

//    for(int i = 0; i < num_parents; i++)
//    {
//        int roulette_idx  = rand() % roulette.size();
//        int parent_idx    = roulette[roulette_idx];
//        parents.push_back(population[parent_idx]);
//    }

    /***Select best ones based on winrate***/
//    vector<NeuralNet> best_ones;
//    for(int k = 0; k < num_parents; k++)
//    {
//        double best_winrate = population[0].get_winrate();
//        double best_idx = 0;
//        for(int i = 1; i < population.size(); i++)
//            if(population[i].get_winrate() > best_winrate)
//            {
//                best_winrate = population[i].get_winrate();
//                best_idx = i;
//            }
//        best_ones.push_back(population[best_idx]);
//        population.erase(population.begin() + best_idx);
//    }


    /***Roulette***/ //https://www.youtube.com/watch?v=ETphJASzYes
//    vector<NeuralNet> parents;

//    double winrate_sum = 0;
//    for(int i = 0; i < best_ones.size(); i++)
//        winrate_sum += best_ones[i].get_winrate();

//    int winrate_sum_scaled = winrate_sum * 1000;

//    for(int i = 0; i < num_parents; i++)
//    {
//        double r = (rand() % winrate_sum_scaled) / 1000.0;
//        int p_idx = 0;
//        while(r > 0)
//        {
//            r = r - best_ones[p_idx].get_winrate();
//            p_idx++;
//        }
//        parents.push_back(best_ones[p_idx - 1]);
//    }




    /***Roulette***/
//    vector<NeuralNet> parents;
//    vector<int> roulette;
//    for(int i = 0; i < best_ones.size(); i++)
//        for(int j = 0; j < best_ones[i].get_winrate() * 100; j++)
//            roulette.push_back(i);

//    for(int i = 0; i < num_parents; i++)
//    {
//        int roulette_idx  = rand() % roulette.size();
//        int parent_idx    = roulette[roulette_idx];
//        parents.push_back(best_ones[parent_idx]);
//    }

    //parents = best_ones;

    return parents;
}

NeuralNet GeneticAlgo::crossover(NeuralNet n1, NeuralNet n2)
{
    int num_inputs  = n1.get_num_inputs();
    int num_hidden  = n1.get_num_hidden();
    int num_outputs = n1.get_num_outputs();
    /***Take traits from n1 and n2 input->hidden weights***/
    vvd WM_IH_n1 = n1.get_WM_IH();
    vvd WM_IH_n2 = n2.get_WM_IH();
    vvd WM_IH_nout = WM_IH_n1;                              //Only to get the proper size...
    for(int i = 0; i < num_inputs; i++)
    {
        for(int j = 0; j < num_hidden; j++)
        {
            int decider = rand() % 100;
            if(decider > 50)
                WM_IH_nout[i][j] = WM_IH_n1[i][j];
            else
                WM_IH_nout[i][j] = WM_IH_n2[i][j];
        }
    }

    /***Take traits from n1 and n2 hidden->output weights***/
    vvd WM_HO_n1 = n1.get_WM_HO();
    vvd WM_HO_n2 = n2.get_WM_HO();
    vvd WM_HO_nout = WM_HO_n1;
    for(int i = 0; i < num_hidden; i++)
        for(int j = 0; j < num_outputs; j++)
        {
            int decider = rand() % 100;
            if(decider > 50)
                WM_HO_nout[i][j] = WM_HO_n1[i][j];
            else
                WM_HO_nout[i][j] = WM_HO_n2[i][j];
        }

    return NeuralNet(WM_IH_nout,WM_HO_nout,num_inputs,num_hidden,num_outputs);

}

void GeneticAlgo::mutate(NeuralNet &net)
{
    vvd &WM_IH = net.get_WM_IH(); //Reference, changing values will effect 'net'.
    vvd &WM_HO = net.get_WM_HO();

    int num_inputs   = net.get_num_inputs();
    int num_hidden   = net.get_num_hidden();
    int num_outputs  = net.get_num_outputs();

    /***Let's change a percentage of the chromosome***/
    double p = 0.05;
    int num_alter_in_hidden = (num_inputs * num_hidden) * p;
    int num_alter_hidden_out = (num_hidden * num_outputs) * p;

    for(int i = 0; i < num_alter_in_hidden; i++)
    {
        int idx_in      = rand() % num_inputs;
        int idx_hid     = rand() % num_hidden;
        WM_IH[idx_in][idx_hid]      = _distribution(_generator);
    }
    for(int i = 0; i < num_alter_hidden_out; i++)
    {
        int idx_hid2    = rand() % num_hidden;
        int idx_out     = rand() % num_outputs;
        WM_HO[idx_hid2][idx_out]    = _distribution(_generator);
    }
}

void GeneticAlgo::make_new_population(vector<NeuralNet> parents, vector<NeuralNet> &pop)
{
    int num_parents = parents.size();
    int popsize     = pop.size();
    int retain      = parents.size() / 2; //Let's keep some parents
    for(int i = 0; i < retain; i++)
        pop[i] = parents[i];
    for(int i = retain; i < popsize; i++)
    {
        int p1_idx = rand() % num_parents;
        int p2_idx = rand() % num_parents;
        NeuralNet n1 = parents[p1_idx];
        NeuralNet n2 = parents[p2_idx];

        NeuralNet child = crossover(n1,n2);
        if(rand() % 100 < _mutation_rate * 100)
            mutate(child);
        pop[i] = child;
    }

}
