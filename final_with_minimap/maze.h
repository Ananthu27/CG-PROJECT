#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>
using namespace std;

class maze
{
public:
    int max_row, max_col;
    vector<vector<int>> matrix_map;
    vector<int> pos = vector<int>(2, 0);

    vector<int> next = pos;
    vector<int> start = pos;
    vector<int> goal = pos;
    vector<int> path;
    bool solution = false;

    maze(int);
    maze(int, int);
    maze(int, int, int);
    void display();
    bool north();
    bool south();
    bool east();
    bool west();
    bool apply(int);
    void register_last_rule();
    float heuristic(vector<int>);
    void display_path(vector<int>);
    void reset_path();

    vector<int> simple_hill_climb();
    vector<int> steepest_ascent_hill_climbing();
    bool acceptance(float, float, float);
    vector<int> simulated_annealing();
};

// constructor for scatter

maze::maze(int rows)
{
    path.push_back(0);
    srand((int)time(0));

    max_row = rows;
    max_col = rows;
    matrix_map = vector<vector<int>>(rows, vector<int>(rows, 1));
    int value;
    bool pos_set = false;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
        {

            value = rand() % 7;
            if (value == 1)
                matrix_map[i][j] = 1;
            else
            {
                if (!pos_set)
                {
                    pos[0] = next[0] = start[0] = i;
                    pos[1] = next[1] = start[1] = j;

                    pos_set = true;
                }
                matrix_map[i][j] = 0;
            }
        }
    }

    int j;
    for (int i = max_row - 1; i >= 0; i--)
    {
        j = max_col - 1;
        for (; j >= 0; j--)
        {
            cout << i << j << endl;
            if (matrix_map[i][j] == 0)
            {
                goal[0] = i;
                goal[1] = j;
                break;
            }
        }
        if (matrix_map[i][j] == 0)
            break;
    }
}
// constructor straight path from middle;

maze::maze(int rows, int cols)
{
    path.push_back(0);

    max_row = rows;
    max_col = cols;
    matrix_map = vector<vector<int>>(rows, vector<int>(cols, 1));
    int midc = cols / 2;
    bool pos_set = false;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (j == midc)
            {
                if (!pos_set)
                {
                    pos[0] = i;
                    pos[1] = j;
                    start = next = pos;
                    pos_set = true;
                }
                matrix_map[i][j] = 0;
            }
        }
    }

    // setting the goal state
    int j;
    for (int i = max_row - 1; i >= 0; i--)
    {
        j = max_col - 1;
        for (; j >= 0; j--)
        {
            cout << i << j << endl;
            if (matrix_map[i][j] == 0)
            {
                goal[0] = i;
                goal[1] = j;
                break;
            }
        }
        if (matrix_map[i][j] == 0)
            break;
    }
}

// constructor with random right and left

maze::maze(int rows, int cols, int freq)
{
    path.push_back(0);

    max_row = rows;
    max_col = cols;
    matrix_map = vector<vector<int>>(rows, vector<int>(cols, 1));

    if (freq < 2)
    {
        int midc = cols / 2;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (j == midc)
                {
                    matrix_map[i][j] = 0;
                }
            }
        }
    }

    else
    {
        int count = 0;
        int s = 0;
        int remember = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (s == 0 && j >= remember)
                    count++;

                if (count % freq == 0)
                {
                    count++;
                    if (s == 1)
                        s = 0;

                    else
                    {
                        s = 1;
                        remember = j - 1;
                    }
                }

                switch (s)
                {
                case 0:
                    if (j >= remember)
                        matrix_map[i][j] = 0;
                    break;
                case 1:
                    if (j == remember)
                    {
                        matrix_map[i][j] = 0;
                    }
                    break;
                default:
                    break;
                }
            }
            if (s == 1)
                count++;
        }
    }

    // setting the goal state
    int j;
    for (int i = max_row - 1; i >= 0; i--)
    {
        j = max_col - 1;
        for (; j >= 0; j--)
        {
            cout << i << j << endl;
            if (matrix_map[i][j] == 0)
            {
                goal[0] = i;
                goal[1] = j;
                break;
            }
        }
        if (matrix_map[i][j] == 0)
            break;
    }
}

// display the matrix map

void maze ::display()
{
    cout << endl;
    for (auto row : matrix_map)
    {
        for (auto value : row)
        {
            cout << value;
        }
        cout << endl;
    }
    cout << endl;

    cout << "starting position : ";
    for (auto value : start)
        cout << value << " ";
    cout << endl;

    cout << "goal position : ";
    for (auto value : goal)
        cout << value << " ";
    cout << endl;
    cout << endl;
}

// rules

bool maze::north()
{
    if (pos[0] - 1 > 0 && (matrix_map[pos[0] - 1][pos[1]]) == 0)
    {
        next[0] = pos[0] - 1;
        next[1] = pos[1];
        return true;
    }
    else
        return false;
}

bool maze::south()
{
    if (pos[0] + 1 < max_row && (matrix_map[pos[0] + 1][pos[1]]) == 0)
    {
        next[0] = pos[0] + 1;
        next[1] = pos[1];
        return true;
    }
    else
        return false;
}

bool maze::east()
{
    if (pos[1] + 1 < max_col && (matrix_map[pos[0]][pos[1] + 1]) == 0)
    {
        next[1] = pos[1] + 1;
        next[0] = pos[0];
        return true;
    }
    else
        return false;
}

bool maze::west()
{
    if (pos[1] - 1 > 0 && (matrix_map[pos[0]][pos[1] - 1]) == 0)
    {
        next[1] = pos[1] - 1;
        next[0] = pos[0];
        return true;
    }
    else
        return false;
}

bool maze::apply(int rule)
{
    switch (rule)
    {
    case 1:
        return north();
        break;
    case 2:
        return south();
        break;
    case 3:
        return east();
        break;
    case 4:
        return west();
        break;
    default:
        return false;
    }
}

void maze::register_last_rule()
{
    pos = next;
}

float maze::heuristic(vector<int> cur)
{
    return (sqrt((pow((cur[0] - goal[0]), 2) + pow((cur[1] - goal[1]), 2))));
}

void maze ::display_path(vector<int> para_path)
{
    cout << endl;
    for (auto value : para_path)
        cout << value << " ";
    cout << endl;

    for (int i = 1; i < para_path.size(); i++)
    {
        switch (para_path[i])
        {
        case 1:
            cout << "north ";
            break;

        case 2:
            cout << "south ";
            break;

        case 3:
            cout << "east ";
            break;

        case 4:
            cout << "west ";
            break;
        }
    }
    cout << endl;

    switch (para_path[0])
    {
    case 0:
        cout << "NO ALGORITHM APPLIED" << endl;
        break;

    case 1:
        cout << "algorithm used :: SIMPLE HILL CLIMB" << endl;
        break;
    case 2:
        cout << "algorithm used :: STEEPEST-ASCENT HILL CLIMBING" << endl;
        break;
    case 3:
        cout << "algorithm used :: SIMULATED ANNEALING " << endl;
        break;
    }
    cout << endl;
}

void maze ::reset_path()
{
    path = vector<int>(1, 0);
}

// all heuristic algorithms

// 1) simple hill climbing algorithm

vector<int> maze ::simple_hill_climb()
{
    //pushing 1 to say simple hill climbing was used
    path[0] = 1;
    srand((int)time(0));
    int not_applied[] = {1, 1, 1, 1, 1, 1};
    solution = false;

    if (heuristic(start) == 0)
        return path;

    else
    {
        int rule;
        int sum;

        while (heuristic(pos) != 0)
        {
            sum = 0;
            for (int i = 0; i < 6; i++)
            {
                // cout << not_applied[i];
                sum += not_applied[i];
            }
            // cout << endl;

            if (sum == 0)
                break;

            rule = rand() % 6 + 1;
            if (apply(rule) && heuristic(next) < heuristic(pos))
            {
                for (int i = 0; i < 6; i++)
                    not_applied[i] = 1;

                path.push_back(rule);
                register_last_rule();
            }
            else
                not_applied[rule - 1] = 0;
        }
    }

    if (heuristic(pos))
        solution = false;

    pos[0] = next[0] = start[0];
    pos[1] = next[1] = start[1];

    return path;
}

// 2) steepest accent hill climbing algorithm

vector<int> maze ::steepest_ascent_hill_climbing()
{
    path[0] = 2;
    solution = false;

    if (heuristic(start) == 0)
        return path;

    else
    {
        int rule;

        while (heuristic(pos) != 0)
        {
            vector<int> cur = pos;
            for (int r = 1; r <= 6; r++)
            {
                if (apply(r))
                {
                    // cout << r << " is aplicable" << endl;
                    // cout << " from " << pos[0] << pos[1] << pos[2] << endl;
                    if (heuristic(next) < heuristic(cur))
                    {
                        // cout << "applying rule " << r << " is better to reach " << next[0] << next[1] << next[2] << " from " << cur[0] << cur[1] << cur[2] << endl;
                        cur = next;
                        rule = r;
                    }
                    // else
                    // {
                    //     cout << "but " << next[0] << next[1] << next[2] << " is not better than " << cur[0] << cur[1] << cur[2] << endl;
                    // }
                }
            }

            if (heuristic(cur) >= heuristic(pos))
            {
                next = pos;
                break;
            }
            else
            {
                // cout << " applying rule " << rule << " from " << pos[0] << pos[1] << pos[2] << " and reaching " << next[0] << next[1] << next[2] << endl;
                pos = next = cur;
                path.push_back(rule);
            }
        }
    }

    if (heuristic(pos))
        solution = false;

    pos[0] = next[0] = start[0];
    pos[1] = next[1] = start[1];

    return path;
}

// 3) simulated annealing

bool maze::acceptance(float cur_energy, float new_energy, float temp)
{
    if (new_energy < cur_energy)
        return true;

    else if (abs(log(temp) + temp * sin(temp)) >= ((float)rand() / 100000000))
        return true;

    else
        return false;
}

vector<int> maze::simulated_annealing()
{
    path[0] = 3;
    srand((int)time(0));
    solution = false;

    if (heuristic(start) == 0)
        return path;

    else
    {
        int rule;
        int sum;
        float temp = 25 * (max_row >= max_col ? max_row : max_col);
        float cooling_rate = 0.03;

        while (heuristic(pos) != 0 && temp > 1)
        {
            rule = rand() % 6 + 1;
            if (apply(rule))
            {
                if (acceptance(heuristic(pos), heuristic(next), temp))
                {
                    path.push_back(rule);
                    register_last_rule();
                }
            }

            temp *= 1 - cooling_rate;
        }
    }

    if (heuristic(pos))
        solution = false;

    pos[0] = next[0] = start[0];
    pos[1] = next[1] = start[1];

    return path;
}