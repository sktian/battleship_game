#include "Game.h"
#include "Player.h"
#include "Board.h"
#include <iostream>
#include <string>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}


int main() {
    Game g(10, 10);
    g.addShip(5, 'A', "aircraft carrier");
    g.addShip(4, 'B', "battleship");
    g.addShip(3, 'D', "destroyer");
    g.addShip(3, 'S', "submarine");
    g.addShip(2, 'P', "patrol boat");
    Board b(g);
    Point p(0, 0);
    Point p1(1, 1);
    Point p2(2, 5);
    Point p3(9, 7);
    Point p4(8, 1);
    Player* player = createPlayer("mediocre", "kevin", g);
    if (player->placeShips(b)) {
        cout << "sucess";
    }
    b.display(false);
}

//int main()
//{
//    const int NTRIALS = 10;
//
//    cout << "Select one of these choices for an example of the game:" << endl;
//    cout << "  1.  A mini-game between two mediocre players" << endl;
//    cout << "  2.  A mediocre player against a human player" << endl;
//    cout << "  3.  A " << NTRIALS
//         << "-game match between a mediocre and an awful player, with no pauses"
//         << endl;
//    cout << "Enter your choice: ";
//    string line;
//    getline(cin,line);
//    if (line.empty())
//    {
//        cout << "You did not enter a choice" << endl;
//    }
//    else if (line[0] == '1')
//    {
//        Game g(2, 3);
//        g.addShip(2, 'R', "rowboat");
//        Player* p1 = createPlayer("mediocre", "Popeye", g);
//        Player* p2 = createPlayer("mediocre", "Bluto", g);
//        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
//        g.play(p1, p2);
//        delete p1;
//        delete p2;
//    }
//    else if (line[0] == '2')
//    {
//        Game g(10, 10);
//        addStandardShips(g);
//        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
//        Player* p2 = createPlayer("human", "Shuman the Human", g);
//        g.play(p1, p2);
//        delete p1;
//        delete p2;
//    }
//    else if (line[0] == '3')
//    {
//        int nMediocreWins = 0;
//
//        for (int k = 1; k <= NTRIALS; k++)
//        {
//            cout << "============================= Game " << k
//                 << " =============================" << endl;
//            Game g(10, 10);
//            addStandardShips(g);
//            Player* p1 = createPlayer("awful", "Awful Audrey", g);
//            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
//            Player* winner = (k % 2 == 1 ?
//                                g.play(p1, p2, false) : g.play(p2, p1, false));
//            if (winner == p2)
//                nMediocreWins++;
//            delete p1;
//            delete p2;
//        }
//        cout << "The mediocre player won " << nMediocreWins << " out of "
//             << NTRIALS << " games." << endl;
//          // We'd expect a mediocre player to win most of the games against
//          // an awful player.  Similarly, a good player should outperform
//          // a mediocre player.
//    }
//    else
//    {
//       cout << "That's not one of the choices." << endl;
//    }
//}
