#include <iostream>
#include <vector>
#include <cstdlib>   //for rand() and srand()
#include <ctime>     //for time()
#include <windows.h> //for sleep() and system()

using namespace std;

const int ROWS = 5;  // height of the grid
const int COLS = 10; // width of grid

// it will  hold (x, y) coordinates
struct Position
{
    int x, y;

    // for  comparing two positions
    bool operator==(const Position &p) const
    {
        return x == p.x && y == p.y;
    }
};

class Entity
{
protected:
    Position pos;

public:
    Entity(int x, int y) { pos = {x, y}; }

    virtual void move() = 0;

    Position getPos() const { return pos; }
    void setPos(Position p) { pos = p; }
};

class Player : public Entity
{
public:
    Player(int x, int y) : Entity(x, y) {}

    void move(char c)
    {
        switch (tolower(c))
        {
        case 'w': // up
            if (pos.y > 0)
                pos.y--;
            break;
        case 's': // down
            if (pos.y < ROWS - 1)
                pos.y++;
            break;
        case 'a': // left
            if (pos.x > 0)
                pos.x--;
            break;
        case 'd': // right
            if (pos.x < COLS - 1)
                pos.x++;
            break;
        }
    }

    void move() override {}
};

class Zombie : public Entity
{
public:
    Zombie(int x, int y) : Entity(x, y) {}

    void move() override
    {
        // zombies move left
        pos.x--;

        // if they go off the left edge, wrap around to the right
        if (pos.x < 0)
            pos.x = COLS - 1;
    }
};

class Game
{
private:
    Player player;
    vector<Zombie> zombies;
    bool gameOver;
    bool playerWon;
    int turn;

public:
    Game() : player(0, 0) // initialize player at (0, 0)
    {
        // random number generator
        srand(time(0));

        // create 8 zombies at random positions (was 5)
        for (int i = 0; i < 8; i++)
            zombies.push_back(Zombie(rand() % COLS, rand() % ROWS));

        gameOver = false;
        playerWon = false;
        turn = 0;
    }

    // draw(): it will clear screen and draws the grid
    void draw()
    {
        system("cls"); // clear the console
        for (int y = 0; y < ROWS; y++)
        {
            for (int x = 0; x < COLS; x++)
            {
                Position p = {x, y};
                bool drawn = false;

                // it will draw player
                if (p == player.getPos())
                {
                    cout << 'P';
                    drawn = true;
                }
                // it will draw zombies
                else
                {
                    for (auto &z : zombies)
                    {
                        if (p == z.getPos())
                        {
                            cout << 'Z';
                            drawn = true;
                            break; // it will stop after drawing one zombie
                        }
                    }
                }

                // this will draw empty space
                if (!drawn)
                    cout << '.';
            }
            cout << "\n";
        }
        cout << "\nTurn: " << turn << " | W/A/S/D move | Q quit\n";
    }

    void checkStatus() // for win or loss
    {
        Position playerPos = player.getPos();

        // Win if player reaches the right-most column
        if (playerPos.x == COLS - 1)
        {
            playerWon = true;
            gameOver = true;
        }

        // Lose if a zombie is on the same position as the player
        for (auto &z : zombies)
        {
            if (playerPos == z.getPos())
            {
                playerWon = false;
                gameOver = true;
            }
        }
    }

    void run()
    {
        while (!gameOver)
        {
            draw(); // draw the board

            char c;
            cin >> c;
            if (tolower(c) == 'q')
                break; // quit game

            player.move(c); // move player and it will check for win or loss
            checkStatus();
            if (gameOver)
                break;

            for (auto &z : zombies) // moves zombies
                z.move();

            checkStatus(); // check again if player is caught by zombies or not
            if (gameOver)
                break;

            // nxt  turn
            turn++;
            Sleep(50); // small delay so to make it feel responsive
        }

        system("cls");
        draw();
        cout << "\n=============================\n";
        if (playerWon)
            cout << " YOU WIN! You reached the goal!\n";
        else
            cout << " GAME OVER! A zombie caught you!\n";

        cout << "Total turns: " << turn << "\n";
        cout << "=============================\n";
    }
};

int main()
{
    cout << "=== ZOMBIE SURVIVAL GAME ===\n";

    cout << "Goal: Reach the RIGHT-MOST edge without getting caught!\n";
    cout << "Controls: W(up), A(left), S(down), D(right), Q(quit)\n";
    system("pause");
    Game g;
    g.run();

    return 0;
}