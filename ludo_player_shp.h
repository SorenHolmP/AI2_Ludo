#ifndef LUDO_PLAYER_SHP_H
#define LUDO_PLAYER_SHP_H


#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include "neuralnet.h"

using namespace std;

class ludo_player_shp : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int _fitness;
    int dice_roll;

    int make_decision();

    /***shp***/
    NeuralNet _net;



public:
    ludo_player_shp();

    /***shp***/
    ludo_player_shp(NeuralNet net);
    void assign_net(NeuralNet &net);

    /***Getters***/
    int get_fitness();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};


#endif // LUDO_PLAYER_SHP_H
