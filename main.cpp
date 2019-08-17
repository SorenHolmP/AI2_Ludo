#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "ludo_player_random.h"
#include "positions_and_dice.h"

#include "ludo_player_shp.h"
#include "geneticalgo.h"

Q_DECLARE_METATYPE( positions_and_dice )

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    qRegisterMetaType<positions_and_dice>();

    //instantiate the players here
    NeuralNet n;
    ludo_player_shp p1;         //green (p1)
    //ludo_player p2,p3,p4;             //yellow (p2)
    ludo_player_random p2, p3, p4;  //blue (p3), red (p4)

    game g;
    g.setGameDelay(00); //if you want to see the game, set a delay

    // Add a GUI <-- remove the '/' to uncomment block
//    Dialog w;
//    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
//    QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
//    QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
//    QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
//    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
//    w.show();
     //Or don't add the GUI
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    //*/

    //set up for each player
    QObject::connect(&g, SIGNAL(player1_start(positions_and_dice)),&p1,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p1,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player1_end(std::vector<int>)),    &p1,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p1,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player2_start(positions_and_dice)),&p2,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p2,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player2_end(std::vector<int>)),    &p2,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p2,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player3_start(positions_and_dice)),&p3,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p3,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player3_end(std::vector<int>)),    &p3,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p3,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player4_start(positions_and_dice)),&p4,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p4,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player4_end(std::vector<int>)),    &p4,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p4,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    vector<NeuralNet> pop;
    vector<NeuralNet> parents;
    int popsize = 100;
    double mutation_rate = 0.05;
    GeneticAlgo ga(popsize, mutation_rate);


    //ofstream log_fitness("/home/soren/8-semester/AI2/ludo_fork/ludo-gui/data/vs_semismart/100gen_25pop_0.05mut_12inputs_24_hidden_fitness.txt");
    //ofstream log_winrate("/home/soren/8-semester/AI2/ludo_fork/ludo-gui/data/vs_semismart/100gen_25pop_0.05mut_12inputs_24_hidden_winrate.txt");

//    for(int i = 0; i < popsize; i++)
//    {
//        pop.push_back( ga.generate_random_neural_net(12,24,4) );
//    }

//    int num_generations = 100;
//    int max_fitness_overall = 0;
//    for(int k = 0; k < num_generations; k++)
//    {
//        double avg_fitness = 0;
//        double avg_winrate = 0;
//        double highest_winrate_gen_k = 0;
//        double lowest_winrate_gen_k = 1.1;

//        int highest_fitness_generation = 0;

//        for(int i = 0; i < popsize; ++i)
//        {
//            p1.assign_net(pop[i]);
//            int numAttempts = 20; //How many times should each network get to play
//            double winrate = 0;
//            double fitness_sum = 0;
//            for(int j = 0; j < numAttempts; j++)
//            {
//                g.start();
//                a.exec();
//                pop[i].set_fitness(p1.get_fitness());
//                fitness_sum += pop[i].get_fitness();
//                if(pop[i].get_fitness() == 396)
//                    winrate += 1.0/numAttempts;


//                while (a.closingDown());
//                g.reset();
//                if(g.wait());
//            }
//            if(winrate > highest_winrate_gen_k)
//                highest_winrate_gen_k = winrate;
//            if(winrate < lowest_winrate_gen_k)
//                lowest_winrate_gen_k = winrate;

//            //cout << "Net " << i << " win percentage: " << winrate  << endl;
//            //cout << "Net " << i << " avg. fitness: " << (fitness_sum / pop.size()) << endl;

//            avg_winrate += winrate / pop.size();
//            avg_fitness += fitness_sum / pop.size() / numAttempts;
//            pop[i].set_winrate(winrate);
//            pop[i].set_fitness(fitness_sum);
//        }

//        cout << "****************************" << endl;
//        cout << "Generation " << k << endl;
//        cout << "Average winrate: " << avg_winrate << endl;
//        cout << "Highest winrate: " << highest_winrate_gen_k << endl;
//        cout << "Lowest winrate: " << lowest_winrate_gen_k << endl;
//        cout << "Average fitness: " << avg_fitness << endl;

//        //log_fitness << avg_fitness  << endl;
//        //log_winrate << avg_winrate << endl;


//        parents = ga.select_parents(pop,10); //Choose best parents
//        ga.make_new_population(parents,pop);
//    }


//    NeuralNet shp_bot = parents[0];
//    shp_bot.save_to_file("/home/soren/8-semester/AI2/ludo_fork/ludo-gui/saved_nets/vs_random/pop100_mut0.05_100gen_validmoves.txt");


    NeuralNet shp_bot_from_file("/home/soren/8-semester/AI2/ludo_fork/ludo-gui/saved_nets/vs_semismart/pop100_mut0.05_100gen_validmoves.txt");
    p1.assign_net(shp_bot_from_file);


    ofstream log_fitness_trained("/home/soren/8-semester/AI2/ludo_fork/ludo-gui/data/tabular_data/trained_on_semismart_vs_random_fitness.txt");

    int num_games = 10000;
    double winrate = 0;
    for(int i = 0; i < num_games; i++)
    {
        g.start();
        a.exec();
        //cout << p1.get_fitness() << endl;
        log_fitness_trained << p1.get_fitness() << endl;
        if(p1.get_fitness() == 396)
        {
            winrate += 1.0/num_games;
        }
        while (a.closingDown());
        g.reset();
        if(g.wait());
    }

    cout << "winrate" << winrate << endl;

    return 0;
}
