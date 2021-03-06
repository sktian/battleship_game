#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack> 

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

class HumanPlayer : public Player {
public:
    HumanPlayer(string nm, const Game& g);
    virtual ~HumanPlayer() {}
    virtual bool isHuman() const;
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

bool HumanPlayer::isHuman() const {
    return true;
}

bool HumanPlayer::placeShips(Board& b) {
    cout << this->name() << " must place " << game().nShips() << " ships." << endl;
    b.display(false);
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
        while (!getLineWithTwoIntegers(r, c)) {
            cout << "You must enter two integers." << endl;
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        }
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

struct attack_result {
    Point m_p;
    int m_state;
};

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
    bool allchosen();
    attack_result m_history[100];
    int m_history_size;
    Point state_2_coord;
    Point possible_coords[100];
    int n_coords;
};

bool ischosen(Point p, attack_result m_history[], int m_history_size) {
    for (int i = 1; i < m_history_size; i++) {
        if (m_history[i].m_p.c == p.c && m_history[i].m_p.r == p.r)
            return true;
    }
    return false;
}

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
    :Player(nm, g)
{
    m_history[0].m_state = 1;
    m_history_size = 1;
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


bool MediocrePlayer::allchosen() {
    for (int i = 0; i < n_coords; i++) {
        if (!ischosen(possible_coords[i], m_history, m_history_size))
            return false;
    }
    return true;
}



Point MediocrePlayer::recommendAttack() {
    if (m_history[m_history_size - 1].m_state == 1) { // state 1
        Point p = game().randomPoint();
        while (ischosen(p, m_history, m_history_size)) {
            p = game().randomPoint();
        }
        return p;
    }
    else { // state 2
        Point recommend = possible_coords[randInt(n_coords)];
        while (ischosen(recommend, m_history, m_history_size)) {
            if (allchosen()) {
                m_history[m_history_size - 1].m_state = 1;
                Point p = game().randomPoint();
                while (ischosen(p, m_history, m_history_size)) {
                    p = game().randomPoint();
                }
                return p;
            }
            recommend = possible_coords[randInt(n_coords)];
        }
        return recommend;
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
    m_history[m_history_size].m_p = p;
    if (m_history[m_history_size - 1].m_state == 1) { // previously in state 1 when the attack is made
        if (shotHit) {
            if (shipDestroyed) {
                 m_history[m_history_size].m_state = 1;
            }
            else {
                m_history[m_history_size].m_state = 2;
                state_2_coord = p;
                n_coords = 0;
                int r = state_2_coord.r;
                int c = state_2_coord.c;
                int r_lower;
                int r_upper;
                int c_lower;
                int c_upper;
                if (r - 4 < 0) {
                    r_lower = 0;
                }
                else {
                    r_lower = r - 4;
                }
                if (r + 4 > game().rows() - 1) {
                    r_upper = game().rows() - 1;
                }
                else {
                    r_upper = r + 4;
                }
                if (c - 4 < 0) {
                    c_lower = 0;
                }
                else {
                    c_lower = c - 4;
                }
                if (c + 4 > game().cols() - 1) {
                    c_upper = game().cols() - 1;
                }
                else {
                    c_upper = c + 4;
                }
                for (int i = r_lower; i < r; i++) {
                    Point temp(i, c);
                    possible_coords[n_coords] = temp;
                    n_coords++;
                }
                for (int i = r; i < r_upper + 1; i++) {
                    Point temp(i, c);
                    possible_coords[n_coords] = temp;
                    n_coords++;
                }
                for (int i = c_lower; i < c; i++) {
                    Point temp(r, i);
                    possible_coords[n_coords] = temp;
                    n_coords++;
                }
                for (int i = c; i < c_upper + 1; i++) {
                    Point temp(r, i);
                    possible_coords[n_coords] = temp;
                    n_coords++;
                }
            }
        }
        else {
            m_history[m_history_size].m_state = 1;
        }
    }
    else { // previously in state 2 when the attack is made
        if (shotHit) {
            if (shipDestroyed) {
                m_history[m_history_size].m_state = 1;
            }
            else {
                if (allchosen()) {
                    m_history[m_history_size].m_state = 1;
                }
                else {
                    m_history[m_history_size].m_state = 2;
                }
            }
        }
        else {
            if (allchosen()) {
                m_history[m_history_size].m_state = 1;
            }
            else {
                m_history[m_history_size].m_state = 2;
            }
        }
    }
    m_history_size++;
}

void MediocrePlayer::recordAttackByOpponent(Point p) {
    // do nothing because player is mediocre
}



//*********************************************************************
//  GoodPlayer
//*********************************************************************


class GoodPlayer : public Player {
public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer() {}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool placeshipsrecursive(Board& b, int current_shipId);
    bool is_next_to(Point p, attack_result m_history[], int m_history_size) const;
    attack_result m_history[100];
    int m_history_size;
    stack<Point> possiblecoords;
};


GoodPlayer::GoodPlayer(string nm, const Game& g) 
    :Player(nm, g)
{
    m_history[0].m_state = 1;
    m_history_size = 1;
}

bool GoodPlayer::placeshipsrecursive(Board& b, int current_shipId) {
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

bool GoodPlayer::placeShips(Board& b) {
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

bool GoodPlayer::is_next_to(Point p, attack_result m_history[], int m_history_size) const {
    if (p.r - 1 >= 0) {
        Point p1(p.r - 1, p.c);
        if (ischosen(p1, m_history, m_history_size)) {
            return true;
        }
    }
    if (p.r + 1 < game().rows()) {
        Point p1(p.r + 1, p.c);
        if (ischosen(p1, m_history, m_history_size)) {
            return true;
        }
    }
    if (p.c - 1 >= 0) {
        Point p1(p.r, p.c - 1);
        if (ischosen(p1, m_history, m_history_size)) {
            return true;
        }
    }
    if (p.c + 1 < game().cols()) {
        Point p1(p.r, p.c + 1);
        if (ischosen(p1, m_history, m_history_size)) {
            return true;
        }
    }
    return false;
}

Point GoodPlayer::recommendAttack() {
    if (m_history[m_history_size - 1].m_state == 1) { // state 1
        Point p = game().randomPoint();
        int attempts = 0;
        while (ischosen(p, m_history, m_history_size) || is_next_to(p, m_history, m_history_size)) {
            if (attempts > 100) {
                break;
            }
            p = game().randomPoint();
            attempts++;
        }
        if (attempts > 100) {
            while (ischosen(p, m_history, m_history_size)) {
                p = game().randomPoint();
            }
        }
        return p;
    }
    else { // state 2
        if (possiblecoords.empty()) {
            m_history[m_history_size - 1].m_state = 1;
            Point p = game().randomPoint();
            int attempts = 0;
            while (ischosen(p, m_history, m_history_size) || is_next_to(p, m_history, m_history_size)) {
                if (attempts > 100) {
                    break;
                }
                p = game().randomPoint();
                attempts++;
            }
            if (attempts > 100) {
                while (ischosen(p, m_history, m_history_size)) {
                    p = game().randomPoint();
                }
            }
            return p;
        }
        Point p = possiblecoords.top();
        possiblecoords.pop();
        return p;
    }
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
    m_history[m_history_size].m_p = p;
    if (m_history[m_history_size - 1].m_state == 1) { // previously in state 1 when the attack is made
        if (shotHit) {
            if (shipDestroyed) {
                m_history[m_history_size].m_state = 1;
            }
            else {
                m_history[m_history_size].m_state = 2;
                if (p.r - 1 >= 0) { 
                    Point p1(p.r - 1, p.c);
                    if (!ischosen(p1, m_history, m_history_size))
                        possiblecoords.push(p1);
                }
                if (p.r + 1 < game().rows()) {
                    Point p1(p.r + 1, p.c);
                    if (!ischosen(p1, m_history, m_history_size))
                        possiblecoords.push(p1);
                }
                if (p.c - 1 >= 0) {
                    Point p1(p.r, p.c - 1);
                    if (!ischosen(p1, m_history, m_history_size))
                        possiblecoords.push(p1);
                }
                if (p.c + 1 < game().cols()) {
                    Point p1(p.r, p.c + 1);
                    if (!ischosen(p1, m_history, m_history_size))
                        possiblecoords.push(p1);
                }
            }
        }
        else {
            m_history[m_history_size].m_state = 1;
        }
    }
    else { // previously in state 2 when the attack is made
        m_history[m_history_size].m_state = 2;
        if (shotHit) {
            if (p.r - 1 >= 0) {
                Point p1(p.r - 1, p.c);
                if (!ischosen(p1, m_history, m_history_size))
                    possiblecoords.push(p1);
            }
            if (p.r + 1 < game().rows()) {
                Point p1(p.r + 1, p.c);
                if (!ischosen(p1, m_history, m_history_size))
                    possiblecoords.push(p1);
            }
            if (p.c - 1 >= 0) {
                Point p1(p.r, p.c - 1);
                if (!ischosen(p1, m_history, m_history_size))
                    possiblecoords.push(p1);
            }
            if (p.c + 1 < game().cols()) {
                Point p1(p.r, p.c + 1);
                if (!ischosen(p1, m_history, m_history_size))
                    possiblecoords.push(p1);
            }
        }
    }
    m_history_size++;
}

void GoodPlayer::recordAttackByOpponent(Point p) {
    
}


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
