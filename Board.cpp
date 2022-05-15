#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char m_Board[10][10];
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            m_Board[i][j] = '.';
        }
    }
}

void BoardImpl::clear()
{
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            m_Board[i][j] == '.';
        }
    }
}


void BoardImpl::block()
{
    int cells = m_game.rows() * m_game.cols();
    int num_blocked = 0;
    while (num_blocked < cells / 2) {
        int r = randInt(m_game.rows());
        int c = randInt(m_game.cols());
        if (m_Board[r][c] == '.') {
            m_Board[r][c] = '#';
            num_blocked++;
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_Board[r][c] == '#') {
                m_Board[r][c] = '.';
            }
        }
}



bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0 || shipId >= m_game.nShips())
        return false;
    if (topOrLeft.r < 0 || topOrLeft.r >= m_game.rows())
        return false;
    if (topOrLeft.c < 0 || topOrLeft.c >= m_game.cols())
        return false;
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            if (m_Board[i][j] == m_game.shipSymbol(shipId))
                return false;
        }
    }
    if (dir == HORIZONTAL) {
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
            return false;
        for (int i = 0; i < m_game.shipLength(shipId); i++) { // first loop to determine if position is valid
            if (m_Board[topOrLeft.r][topOrLeft.c + i] != '.') {
                return false;
            }
        }
        for (int i = 0; i < m_game.shipLength(shipId); i++) { // second loop to place the symbols
            m_Board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
        }
    } else if (dir == VERTICAL) {
        if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
            return false;
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (m_Board[topOrLeft.r + i][topOrLeft.c] != '.') {
                return false;
            }
        }
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            m_Board[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0 || shipId >= m_game.nShips())
        return false;
    if (topOrLeft.r < 0 || topOrLeft.r >= m_game.rows())
        return false;
    if (topOrLeft.c < 0 || topOrLeft.c >= m_game.cols())
        return false;
    if (dir == HORIZONTAL) {
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
            return false;
        for (int i = 0; i < m_game.shipLength(shipId); i++) { // first loop to determine if position is valid
            if (m_Board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId)) {
                return false;
            }
        }
        for (int i = 0; i < m_game.shipLength(shipId); i++) { // second loop to place the points
            m_Board[topOrLeft.r][topOrLeft.c + i] = '.';
        }
    }
    else if (dir == VERTICAL) {
        if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
            return false;
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (m_Board[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId)) {
                return false;
            }
        }
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            m_Board[topOrLeft.r + i][topOrLeft.c] = '.';
        }
    }
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for (int i = 0; i < m_game.cols(); i++) {
        cout << i;
    }
    cout << endl;
    for (int i = 0; i < m_game.rows(); i++) {
        cout << i << " ";
        for (int j = 0; j < m_game.cols(); j++) {
            if (shotsOnly) {
                if (m_Board[i][j] == 'X' || m_Board[i][j] == 'o') {
                    cout << m_Board[i][j];
                }
                else {
                    cout << '.';
                }
            }
            else {
                cout << m_Board[i][j];
            }
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (p.r < 0 || p.r >= m_game.rows())
        return false;
    if (p.c < 0 || p.c >= m_game.cols())
        return false;
    if (m_Board[p.r][p.c] == 'o' || m_Board[p.r][p.c] == 'X')
        return false;
    if (m_Board[p.r][p.c] == '.') {
        shotHit = false;
        m_Board[p.r][p.c] = 'o';
        return true;
    }
    else {
        shotHit = true;
        char temp_symbol = m_Board[p.r][p.c];
        m_Board[p.r][p.c] = 'X';
        bool found = false;
        for (int i = 0; i < m_game.rows(); i++) {
            for (int j = 0; j < m_game.cols(); j++) {
                if (m_Board[i][j] == temp_symbol)
                    found = true;
            }
        }
        if (found) {
            shipDestroyed = false;
        }
        else {
            shipDestroyed = true;
            for (int i = 0; i < m_game.nShips(); i++) {
                if (m_game.shipSymbol(i) == temp_symbol) {
                    shipId = i;
                    break;
                }
            }
        }
        return true;
    }
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            if (m_Board[i][j] != '.' && m_Board[i][j] != 'X' && m_Board[i][j] != 'o') {
                return false;
            }
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
