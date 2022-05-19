#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

class GameImpl
{
 public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    struct Ship {
        int m_length;
        char m_symbol;
        string m_name;
        int m_id;
    };
    int m_nRows;
    int m_nCols;
    Ship m_Ships[50];
    int m_nShips;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
    :m_nRows(nRows), m_nCols(nCols), m_nShips(0)
{
    
}

int GameImpl::rows() const
{
    return m_nRows;
}

int GameImpl::cols() const
{
    return m_nCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (length <= 0 || length > 10) {
        return false;
    }
    if (symbol == 'X' || symbol == 'o' || symbol == '.') {
        return false;
    }
    if (isprint(symbol) == 0) {
        return false;
    }
    for (int i = 0; i < m_nShips; i++) {
        if (symbol == m_Ships[i].m_symbol) {
            return false;
        }
    }
    m_Ships[m_nShips].m_id = m_nShips;
    m_Ships[m_nShips].m_length = length;
    m_Ships[m_nShips].m_name = name;
    m_Ships[m_nShips].m_symbol = symbol;
    m_nShips++;
    return true;
}

int GameImpl::nShips() const
{
    return m_nShips;
}

int GameImpl::shipLength(int shipId) const
{
    return m_Ships[shipId].m_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_Ships[shipId].m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return m_Ships[shipId].m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    bool player1won = false;
    bool player2won = false;
    if (!p1->placeShips(b1))
        return nullptr;
    if (!p2->placeShips(b2))
        return nullptr;
    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {
        // player 1's turn 
        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl;
        if (p1->isHuman()) {
            b2.display(true);
        }
        else {
            b2.display(false);
        }
        Point point1 = p1->recommendAttack();
        bool sh1;
        bool sd1;
        int sID1;
        bool vs1 = b2.attack(point1, sh1, sd1, sID1);
        p1->recordAttackResult(point1, vs1, sh1, sd1, sID1);
        if (vs1) {
            cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and ";
            if (sh1) {
                if (sd1) {
                    cout << "destroyed the " << p2->game().shipName(sID1) << ", resulting in:" << endl;
                }
                else {
                    cout << "hit something, resulting in:" << endl;
                }
            }
            else {
                cout << "missed, resulting in:" << endl;
            }
            if (p1->isHuman()) {
                b2.display(true);
            }
            else {
                b2.display(false);
            }
        }
        else {
            cout << p1->name() << " wasted a shot at (" << point1.r << "," << point1.c << ")." << endl;
        }
        if (b2.allShipsDestroyed()) {
            player1won = true;
            break;
        }
        if (shouldPause) {
            cout << "Press enter to continue:";
            cin.ignore(10000, '\n');
        }
        // player 2's turn 
        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;
        if (p2->isHuman()) {
            b1.display(true);
        }
        else {
            b1.display(false);
        }
        Point point2 = p2->recommendAttack();
        bool sh2;
        bool sd2;
        int sID2;
        bool vs2 = b1.attack(point2, sh2, sd2, sID2);
        p2->recordAttackResult(point2, vs2, sh2, sd2, sID2);
        if (vs2) {
            cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and ";
            if (sh2) {
                if (sd2) {
                    cout << "destroyed the " << p1->game().shipName(sID2) << ", resulting in:" << endl;
                }
                else {
                    cout << "hit something, resulting in:" << endl;
                }
            }
            else {
                cout << "missed, resulting in:" << endl;
            }
            if (p2->isHuman()) {
                b1.display(true);
            }
            else {
                b1.display(false);
            }
        }
        else {
            cout << p2->name() << " wasted a shot at (" << point2.r << "," << point2.c << ")." << endl;
        }
        if (b1.allShipsDestroyed()) {
            player2won = true;
            break;
        }
        if (shouldPause) {
            cout << "Press enter to continue:";
            cin.ignore(10000, '\n');
        }
    }
    if (player1won) {
        cout << p1->name() << " wins!" << endl;
        if (p2->isHuman()) {
            cout << "Here is where " << p1->name() << "'s ships were:" << endl;
            b1.display(false);
        }
        return p1;
    }
    if (player2won) {
        cout << p2->name() << " wins!" << endl;
        if (p1->isHuman()) {
            cout << "Here is where " << p2->name() << "'s ships were:" << endl;
            b2.display(false);
        }
        return p2;
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

