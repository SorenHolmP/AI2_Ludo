#include "ludo_player_shp.h"

#include <random>

ludo_player_shp::ludo_player_shp(){
    _fitness = 0;
}


ludo_player_shp::ludo_player_shp(NeuralNet net)
{
    _net = net;
}

int ludo_player_shp::make_decision()
{
    /***For now the input vector contains information if the piece is stuck and the dice roll value***/
    /*
    vd inputs;
    for(int i = 0; i < 4; i++)
        if(pos_start_of_turn[i] == -1)
            inputs.push_back(1);
        else
            inputs.push_back(0);

    for(int i = 1; i < 7; i++)
    {
        if(dice_roll == i)
            inputs.push_back(1);
         else
            inputs.push_back(0);
    }

    for(int i = 0; i < 4; i++)
    {
        if(pos_start_of_turn[i] == 99)
            inputs.push_back(1);
        else
            inputs.push_back(0);
    }
    */

    vd inputs(12);
    for(int i = 0; i < 4; i++)
    {
        int posi = pos_start_of_turn[i];
        if(posi == 0 || posi == 8 || posi == 21 || posi == 34 || posi == 47 || posi > 50 )
        {
            inputs[i] = 1;
        }
        for(int j = 0; j < 4; j++)
        {
            int posj = pos_start_of_turn[j];
            if(i != j && posi != -1)
            {
                if(posi == posj)
                {
                    inputs[i] = 1;
                }
            }
        }
    }

    for(int i = 0; i < 4; i++)
    {
        int posi = pos_start_of_turn[i];
        int posi_dice = pos_start_of_turn[i] + dice_roll;
        if(posi_dice == 8 || posi_dice == 21 || posi_dice == 34 || posi_dice == 47 || posi_dice > 50 )
        {
            inputs[4+i] = 1;
        }
        for(int j = 0; j < 4; j++)
        {
            int posj = pos_start_of_turn[j];
            if(i != j && posi != -1)
            {
                if(posi_dice == posj)
                {
                    inputs[4+i] = 1;
                }
            }
        }
    }

/***Move piece if it can go into goal***/
for(int i = 0; i < 4; i++)
{
    int posi = pos_start_of_turn[i];
    if(posi + dice_roll == 56)
        inputs[2*4+i] = 1;
    else
        inputs[2*4+i] = 0;
}



/***Move piece if it is in danger***/
//    for(int i = 0; i < 4; i++)
//    {
//        int posi = pos_start_of_turn[i];
//        for(int j = 4; j < pos_start_of_turn.size(); j++)
//        {
//            int posj = pos_start_of_turn[j];
//            if(posi != -1 && posi < 50 && posj != -1)
//            {
//                if(posi - posj )
//            }

//        }
//    }


//    for(int i = 0; i < 4; i++) //Check if in goal
//    {
//        if(pos_start_of_turn[i] == 99)
//            inputs[2*4+i] = 1;
//        else
//            inputs[2*4+i] = 0;
//    }

//    for(int i = 0; i < 4; i++) //Check if in stuck position
//    {
//        if(pos_start_of_turn[i] == -1)
//            inputs[3*4+i] = 1;
//        else
//            inputs[3*4+i] = 0;
//    }

//    if(dice_roll == 6)
//        inputs[16] = 1;
//    else
//        inputs[16] = 0;



    vd      outputs = _net.run(inputs);

//    int     best_idx = 0;
//    double  best_output = outputs[0];
    double best_output = 0;
    int best_idx = -1;

    for(int i = 0; i < outputs.size(); i++)
    {
        if(pos_start_of_turn[i] == -1)
        {
            if(dice_roll == 6)
            {
                if(outputs[i] > best_output)
                {
                    best_output = outputs[i];
                    best_idx = i;
                }
            }
        }
        else if(pos_start_of_turn[i] != 99)
            if(outputs[i] > best_output)
            {
                best_output = outputs[i];
                best_idx = i;
            }
    }




    return best_idx;
}

void ludo_player_shp::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}


void ludo_player_shp::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }

    emit turn_complete(game_complete);
}

/***Genetic algorithm***/
void ludo_player_shp::assign_net(NeuralNet &net)
{
    _net = net;
}


/***Getters***/
int ludo_player_shp::get_fitness()
{
    int fitness = 0;
    for(int i = 0; i < 4; i++)
        fitness += pos_end_of_turn[i];

    return fitness;
}
