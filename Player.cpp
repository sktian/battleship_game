#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class HumanPlayer : public Player {
public:
    HumanPlayer(string nm, const Game& g);
    virtual ~HumanPlayer() {}
    virtual bool isHuman();
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string nm, const Game& g) 
    :Player(nm, g)
{
    
}

bool HumanPlayer::isHuman() {
    return true;
}

bool HumanPlayer::placeShips(Board& b) {
    for (int i = 0; i < game().nShips(); i++) {
        cout << "Enter h or v for direction of " << game().shipName(i);
        cout << " (length " << game().shipLength(i) << "): ";
        char dir;
        cin >> dir;
        while (dir != 'v' && dir != 'h') {
            cout << "Direction must be h or v." << endl;
            cout << "Enter h or v for direction of " << game().shipName(i);
            cout << " (length " << game().shipLength(i) << "): ";
            cin >> dir;
        }
        Direction direct;
        if (dir == 'h') {
            direct = HORIZONTAL;
        }
        else {
            direct = VERTICAL;
        }
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        int r;
        int c;
        getLineWithTwoIntegers(r, c);
        Point p(r, c);
        while (!b.placeShip(p, i, direct)) {
            cout << "The ship can not be placed there." << endl;
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            getLineWithTwoIntegers(r, c);
            Point temp(r, c);
            p = temp;
        }
        if (i < game().nShips() - 1) {
            b.display(false);
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack() {
    cout << "Enter the row and column to attack (e.g., 3 5):";
    int r;
    int c;
    getLineWithTwoIntegers(r, c);
    Point p(r, c);
    return p;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId) {
    // do nothing cus human player
}

void HumanPlayer::recordAttackByOpponent(Point p) {
    // do nothing cus human player
}



//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player {
public:
    MediocrePlayer(string nm, const Game& g);
    virtual ~MediocrePlayer() {}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool placeshipsrecursive(Board& b, int current_shipId);
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
    :Player(nm, g)
{

}

bool MediocrePlayer::placeshipsrecursive(Board& b, int current_shipId) {
    if (current_shipId == game().nShips())
        return true;
    for (int i = 0; i < game().cols(); i++) {
        for (int j = 0; j < game().rows(); j++) {
            Point p(i, j);
            if (b.placeShip(p, current_shipId, HORIZONTAL) || b.placeShip(p, current_shipId, VERTICAL)) {
                bool canplaceships = placeshipsrecursive(b, current_shipId + 1);
                if (canplaceships) {
                    return true;
                }
                else {
                    b.unplaceShip(p, current_shipId, HORIZONTAL);
                    b.unplaceShip(p, current_shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}

bool MediocrePlayer::placeShips(Board& b) {
    int attempts = 0;
    while (attempts < 50) {
        b.block();
        if (placeshipsrecursive(b, 0)) {
            b.unblock();
            return true;
        }
        b.unblock();
        attempts++;
    }
    return false;
}

Point MediocrePlayer::recommendAttack() {
    Point p(0, 0);
    return p;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {

}

void MediocrePlayer::recordAttackByOpponent(Point p) {
    // do nothing because player is mediocre
}


// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer GoodPlayer;

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
