// spiders.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Pit::display going.  That gives you
// more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;      // max number of rows in the pit
const int MAXCOLS = 40;      // max number of columns in the pit
const int MAXSPIDERS = 170;  // max number of spiders allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Pit;  // This is needed to let the compiler know that Pit is a
            // type name, since it's mentioned in the Spider declaration.

class Spider
{
public:
    // Constructor
    Spider(Pit* pp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;

    // Mutators
    void move();

private:
    Pit* m_pit;
    int  m_row;
    int  m_col;
};

class Player
{
public:
    // Constructor
    Player(Pit* pp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

    // Mutators
    void   stand();
    void   move(int dir);
    void   setDead();

private:
    Pit* m_pit;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class Pit
{
public:
    // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();

    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     spiderCount() const;
    int     numberOfSpidersAt(int r, int c) const;
    void    display() const;

    // Mutators
    bool   addSpider(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSpider(int r, int c);
    bool   moveSpiders();

    // Helper functions
    bool isPosInBounds(int r, int c) const;

private:
    int      m_rows;
    int      m_cols;
    Player* m_player;
    Spider* m_spiders[MAXSPIDERS];
    int      m_nSpiders;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nSpiders);
    ~Game();

    // Mutators
    void play();

private:
    Pit* m_pit;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();

//These functions are kind of redundant with part of the skeleton, but oh well.
int dirToRowMod(int dir) {
    switch (dir) {
    case UP:
        return -1;
    case DOWN:
        return 1;
    case RIGHT:
    case LEFT:
    default:
        return 0;
    }
}
int dirToColMod(int dir) {
    switch (dir) {
    case UP:
    case DOWN:
        return 0;
    case RIGHT:
        return 1;
    case LEFT:
        return -1;
    default:
        return 0;
    }
}
bool isPosInBounds(int row, int col, Pit* pit) {
    return (row >= 1 && row <= pit->rows() && col >= 1 && col <= pit->cols());
}

///////////////////////////////////////////////////////////////////////////
//  Spider implementation
///////////////////////////////////////////////////////////////////////////

Spider::Spider(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** A spider must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
    {
        cout << "***** Spider created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
}

int Spider::row() const
{
    return m_row;
}

int Spider::col() const
{
    // DID: Return the column the spider is at.
    return m_col; 
}

void Spider::move()
{
    // Attempt to move in a random direction; if it can't move, don't move

    //Figure out the intended row and column to move to
    int dir = randInt(0, NUMDIRS - 1);
    int intendedRow = m_row + dirToRowMod(dir);
    int intendedCol = m_col + dirToColMod(dir);

    //if out of bounds, don't move
    if (!m_pit->isPosInBounds(intendedRow, intendedCol)) {
        return;
    }

    m_row = intendedRow;
    m_col = intendedCol;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** The player must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    m_age++;

    // DID:   If there's a grid position in the indicated direction that is
    //        adjacent to the player and vacant, move the player there.  If
    //        there's a grid position in the indicated direction that is
    //        adjacent to the player and contains one or more spiders, with
    //        a grid position next to and on the other side of the spider(s),
    //        jump over the spider(s), killing (destroying) one, and landing
    //        at that position (and dying if a spider is there).  If there's
    //        a grid position in the indicated direction that is adjacent to
    //        the player and contains one or more spiders, with no grid
    //        position next to and on the other side of the spider(s),
    //        jump over the spider(s), killing (destroying) one, but landing
    //        at that dead spider's position (and dying if another spider
    //        is there).  In any other case, don't move.

    //Find the square being moved to based on current square and direction
    int intendedRow = m_row + dirToRowMod(dir);
    int intendedCol = m_col + dirToColMod(dir);

    //If it would be out of bounds, do nothing
    if (!m_pit->isPosInBounds(intendedRow, intendedCol)) return;

    //If there are no spiders, just move there
    if (m_pit->numberOfSpidersAt(intendedRow, intendedCol) == 0) {
        m_col = intendedCol;
        m_row = intendedRow;
        return;
    }

    //Apply the direction again to figure out where the player would be jumping to (if possible)
    int jumpTargetRow = intendedRow + dirToRowMod(dir);
    int jumpTargetCol = intendedCol + dirToColMod(dir);

    //If there are spiders but there's no space behind, kill 1 spider and move there (dying if there are any left)
    if (!m_pit->isPosInBounds(jumpTargetRow, jumpTargetCol)) {

        m_pit->destroyOneSpider(intendedRow, intendedCol);

        m_row = intendedRow;
        m_col = intendedCol;

        if (m_pit->numberOfSpidersAt(intendedRow, intendedCol) > 0) {
            setDead();
        }

    }

    //If there are spiders and there is a space behind, kill 1 spider and move to the space behind (dying if there's a spider)
    else {

        m_pit->destroyOneSpider(intendedRow, intendedCol);

        m_row = jumpTargetRow;
        m_col = jumpTargetCol;

        if (m_pit->numberOfSpidersAt(m_row, m_col) > 0) {
            setDead();
        }

    }

    return;
}

bool Player::isDead() const
{
    return m_dead;  // this is wrong -- replace it
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Pit implementation
///////////////////////////////////////////////////////////////////////////

Pit::Pit(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Pit created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nSpiders = 0;
}

Pit::~Pit()
{
    // DID:  Delete the player and all remaining dynamically allocated spiders.
    delete m_player;
    for (int i = 0; i < m_nSpiders; i++) {
        delete m_spiders[i];
    }
}

int Pit::rows() const
{
    // DID:  Return the number of rows in the pit.
    return m_rows;
}

int Pit::cols() const
{
    return m_cols;
}

Player* Pit::player() const
{
    return m_player;
}

int Pit::spiderCount() const
{
    return m_nSpiders;
}

int Pit::numberOfSpidersAt(int r, int c) const
{
    // DID:  Return the number of spiders at row r, column c.
    int numSpiders = 0;
    for (int i = 0; i < m_nSpiders; i++) {
        if (m_spiders[i]->row() == r && m_spiders[i]->col() == c) {
            numSpiders++;
        }
    }
    return numSpiders;
}

void Pit::display() const
{
    // Position (row,col) in the pit coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++) {
        for (c = 0; c < cols(); c++) {

            // Indicate each spider's position
            if (numberOfSpidersAt(r+1, c+1) == 0) {
                grid[r][c] = '.';
            }
            else if (numberOfSpidersAt(r+1, c+1) == 1) {
                grid[r][c] = 'S';
            }
            else if (numberOfSpidersAt(r+1, c+1) < 10) {
                grid[r][c] = '0' + numberOfSpidersAt(r+1, c+1);
            }
            else {
                grid[r][c] = '9';
            }

        }
    }

    // Indicate player's position
    if (m_player != nullptr)
    {
        char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (m_player->isDead())
            gridChar = '*';
        else
            gridChar = '@';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, spider, and player info
    cout << "There are " << spiderCount() << " spiders remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Pit::addSpider(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a spider on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;
    
    //DID
    // If there are MAXSPIDERS existing spiders, return false.
    // Otherwise, dynamically allocate a new spider at coordinates (r,c).
    // Save the pointer to newly allocated spider and return true.

    if (spiderCount() == MAXSPIDERS) return false;

    Spider *spider = new Spider(this, r, c);
    m_spiders[m_nSpiders] = spider;
    m_nSpiders++;

    return true;
}

bool Pit::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a spider
    if (numberOfSpidersAt(r, c) > 0)
        return false;

    // Dynamically allocate a new Player and add it to the pit
    m_player = new Player(this, r, c);
    return true;
}

bool Pit::destroyOneSpider(int r, int c)
{
    for (int i = 0; i < m_nSpiders; i++) {
        if (m_spiders[i]->row() == r && m_spiders[i]->col() == c) {
            for (; i < m_nSpiders - 1; i++) {
                m_spiders[i] = m_spiders[i + 1];
            }
            m_spiders[i] = nullptr;
            m_nSpiders--;
            return true;
        }
    }
    return false;  // this is wrong -- replace it
}

bool Pit::moveSpiders()
{
    for (int k = 0; k < m_nSpiders; k++)
    {
        // DID:  Have the k-th spider in the pit make one move.
        //        If that move results in that spider being in the same
        //        position as the player, the player dies.
        m_spiders[k]->move();
        if (m_spiders[k]->row() == m_player->row() && m_spiders[k]->col() == m_player->col()) {
            m_player->setDead();
        }
    }

    // return true if the player is still alive, false otherwise
    return !m_player->isDead();
}

bool Pit::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nSpiders)
{
    if (nSpiders < 0)
    {
        cout << "***** Cannot create Game with negative number of spiders!" << endl;
        exit(1);
    }
    if (nSpiders > MAXSPIDERS)
    {
        cout << "***** Trying to create Game with " << nSpiders
            << " spiders; only " << MAXSPIDERS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1 && cols == 1 && nSpiders > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the spiders!" << endl;
        exit(1);
    }

    // Create pit
    m_pit = new Pit(rows, cols);

    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_pit->addPlayer(rPlayer, cPlayer);

    // Populate with spiders
    while (nSpiders > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a spider where the player is
        if (r == rPlayer && c == cPlayer)
            continue;
        m_pit->addSpider(r, c);
        nSpiders--;
    }
}

Game::~Game()
{
    delete m_pit;
}

void Game::play()
{
    m_pit->display();
    Player* player = m_pit->player();
    if (player == nullptr)
        return;

    while (!m_pit->player()->isDead() && m_pit->spiderCount() > 0)
    {
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin, action);
        if (action.size() == 0)
            player->stand();
        else
        {
            switch (action[0])
            {
            default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
            case 'q':
                return;
            case 'u':
            case 'd':
            case 'l':
            case 'r':
                player->move(decodeDirection(action[0]));
                break;
            }
        }
        if (!player->isDead())
            m_pit->moveSpiders();
        m_pit->display();
    }
    if (m_pit->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

int decodeDirection(char dir)
{
    switch (dir)
    {
    case 'u':  return UP;
    case 'd':  return DOWN;
    case 'l':  return LEFT;
    case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

bool directionToDeltas(int dir, int& rowDelta, int& colDelta)
{
    switch (dir)
    {
    case UP:     rowDelta = -1; colDelta = 0; break;
    case DOWN:   rowDelta = 1; colDelta = 0; break;
    case LEFT:   rowDelta = 0; colDelta = -1; break;
    case RIGHT:  rowDelta = 0; colDelta = 1; break;
    default:     return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 3, 2);
    Game g(9, 10, 42);

    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
