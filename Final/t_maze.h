#include <ctime>
#include "node_list.h"
using namespace std;

class maze
{
public:
    int max_row, max_col, max_depth;
    vector<vector<vector<int>>> matrix_map;
    vector<int> pos = vector<int>(3, 0);

    vector<int> next = pos;
    vector<int> start = pos;
    vector<int> goal = pos;
    vector<int> path;
    bool solution = false;

    maze();
    maze(int);
    maze(int, int);

    void display();
    bool north();
    bool south();
    bool east();
    bool west();
    bool up();
    bool down();
    bool apply(int);
    void register_last_rule();
    float heuristic(vector<int>);
    void display_path(vector<int>);
    void reset_path();

    vector<int> simple_hill_climb();
    vector<int> steepest_ascent_hill_climbing();
    bool acceptance(float, float, float);
    vector<int> simulated_annealing();
    vector<int> trace_back_path(node_list);
    vector<int> bfs();
    bool verify(vector<int>);
    vector<int> dfs();
    node_list propagate(node_list, node);
    vector<int> a_star();
};

//custom map()

maze::maze()
{
    cout << "Enter the max number of rows : ";
    cin >> max_row;
    cout << "Enter the max number of columns : ";
    cin >> max_col;
    max_depth = 1;

    matrix_map = vector<vector<vector<int>>>(max_depth, vector<vector<int>>(max_row, vector<int>(max_col, 0)));

    cout << endl;
    cout << "Now enter the " << max_row * max_col << " elements for the maze" << endl;

    bool value;

    for (int i = 0; i < max_row; i++)
    {
        for (int j = 0; j < max_col; j++)
        {
            cin >> value;
            matrix_map[i][j][0] = value;
        }
    }

    int i, j;
    for (int k = max_depth - 1; k >= 0; k--)
    {
        i = max_row - 1;
        for (; i >= 0; i--)
        {
            j = max_col - 1;
            for (; j >= 0; j--)
            {
                // cout << i << j << endl;
                if (matrix_map[i][j][k] == 0)
                {
                    goal[0] = i;
                    goal[1] = j;
                    goal[2] = k;
                    break;
                }
            }
            if (matrix_map[i][j][k] == 0)
                break;
        }
        if (matrix_map[i][j][k] == 0)
            break;
    }
}

// constructor for scatter

maze::maze(int rows)
{
    path.push_back(0);
    srand((int)time(0));

    max_row = rows;
    max_col = rows;
    max_depth = rows;

    matrix_map = vector<vector<vector<int>>>(rows, vector<vector<int>>(rows, vector<int>(rows, 1)));
    int value;
    bool pos_set = false;

    for (int k = 0; k < rows; k++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < rows; j++)
            {

                value = rand() % 10;

                if (value == 1)
                    matrix_map[i][j][k] = 1;
                else
                {
                    if (!pos_set)
                    {
                        pos[0] = next[0] = start[0] = i;
                        pos[1] = next[1] = start[1] = j;
                        pos[2] = next[2] = start[2] = k;

                        pos_set = true;
                    }
                    matrix_map[i][j][k] = 0;
                }

                //wrong
                // matrix_map[i][j][k] = 0;
            }
        }
    }

    int i, j;
    for (int k = max_depth - 1; k >= 0; k--)
    {
        i = max_row - 1;
        for (; i >= 0; i--)
        {
            j = max_col - 1;
            for (; j >= 0; j--)
            {
                // cout << i << j << endl;
                if (matrix_map[i][j][k] == 0)
                {
                    goal[0] = i;
                    goal[1] = j;
                    goal[2] = k;
                    break;
                }
            }
            if (matrix_map[i][j][k] == 0)
                break;
        }
        if (matrix_map[i][j][k] == 0)
            break;
    }
}
// constructor straight path from middle;

maze::maze(int rows, int cols)
{
    path.push_back(0);

    max_row = rows;
    max_col = cols;
    max_depth = 2;
    matrix_map = vector<vector<vector<int>>>(rows, vector<vector<int>>(rows, vector<int>(rows, 1)));

    int midc = cols / 2;
    bool pos_set = false;

    for (int k = 0; k < max_depth; k++)
    {
        for (int i = 0; i < max_row; i++)
        {
            for (int j = 0; j < max_col; j++)
            {
                if (j == midc)
                {
                    if (!pos_set)
                    {
                        pos[0] = i;
                        pos[1] = j;
                        pos[2] = k;
                        start = next = pos;
                        pos_set = true;
                    }
                    matrix_map[i][j][k] = 0;
                }
            }
        }
    }

    // constructing base

    for (int k = 0; k < max_depth; k++)
    {
        for (int i = 1; i < max_row - 1; i++)
        {
            for (int j = 1; j < max_col - 1; j++)
            {
                matrix_map[i][j][k] = !k;
            }
        }
    }

    // setting the goal state
    int i, j;
    for (int k = 0; k < max_depth; k++)
    {
        i = max_row - 1;
        for (; i >= 0; i--)
        {
            j = max_col - 1;
            for (; j >= 0; j--)
            {
                // cout << i << j << endl;
                if (matrix_map[i][j][k] == 0)
                {
                    goal[0] = i;
                    goal[1] = j;
                    goal[2] = k;
                    break;
                }
            }
            if (matrix_map[i][j][k] == 0)
                break;
        }
        if (matrix_map[i][j][k] == 0)
            break;
    }
}

// display the matrix map

void maze ::display()
{
    cout << endl;

    for (int k = 0; k < max_depth; k++)
    {
        for (int i = 0; i < max_row; i++)
        {
            for (int j = 0; j < max_col; j++)
            {
                cout << matrix_map[i][j][k];
            }
            cout << endl;
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
    if (pos[0] - 1 >= 0 && (matrix_map[pos[0] - 1][pos[1]][pos[2]]) == 0)
    {
        next[0] = pos[0] - 1;
        next[1] = pos[1];
        next[2] = pos[2];
        return true;
    }
    else
        return false;
}

bool maze::south()
{
    if (pos[0] + 1 < max_row && (matrix_map[pos[0] + 1][pos[1]][pos[2]]) == 0)
    {
        next[0] = pos[0] + 1;
        next[1] = pos[1];
        next[2] = pos[2];
        return true;
    }
    else
        return false;
}

bool maze::east()
{
    if (pos[1] + 1 < max_col && (matrix_map[pos[0]][pos[1] + 1][pos[2]]) == 0)
    {
        next[1] = pos[1] + 1;
        next[0] = pos[0];
        next[2] = pos[2];
        return true;
    }
    else
        return false;
}

bool maze::west()
{
    if (pos[1] - 1 >= 0 && (matrix_map[pos[0]][pos[1] - 1][pos[2]]) == 0)
    {
        next[1] = pos[1] - 1;
        next[0] = pos[0];
        next[2] = pos[2];
        return true;
    }
    else
        return false;
}

bool maze ::up()
{
    if (pos[2] + 1 < max_depth && (matrix_map[pos[0]][pos[1]][pos[2] + 1] == 0))
    {
        next[1] = pos[1];
        next[0] = pos[0];
        next[2] = pos[2] + 1;
        return true;
    }
    else
        return false;
}

bool maze ::down()
{
    if (pos[2] + -1 >= 0 && (matrix_map[pos[0]][pos[1]][pos[2] - 1] == 0))
    {
        next[1] = pos[1];
        next[0] = pos[0];
        next[2] = pos[2] - 1;
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
    case 5:
        return up();
        break;
    case 6:
        return down();
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
    return (sqrt((pow((cur[0] - goal[0]), 2) + pow((cur[1] - goal[1]), 2) + pow(cur[2] - goal[2], 2))));
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

        case 5:
            cout << "up ";
            break;

        case 6:
            cout << "down ";
            break;
        }
    }
    cout << endl;

    switch (para_path[0])
    {
    case 0:
        cout << "NO ALGORITHM APPLIED" << endl;
        break;
    case 3:
        cout << "algorithm used :: SIMPLE HILL CLIMB" << endl;
        break;
    case 4:
        cout << "algorithm used :: STEEPEST-ASCENT HILL CLIMBING" << endl;
        break;
    case 5:
        cout << "algorithm used :: SIMULATED ANNEALING " << endl;
        break;
    case 1:
        cout << "algorithm used :: BREADTH FIRST SEARCH" << endl;
        break;
    case 2:
        cout << "algorithm used :: DEPTH FIRST SERACH" << endl;
        break;
    case 6:
        cout << "algorithm used :: A*" << endl;
        break;
    }
}

void maze ::reset_path()
{
    path = vector<int>(1, 0);
}

// all search algorithms

// simple hill climbing algorithm

vector<int> maze ::simple_hill_climb()
{
    //pushing 1 to say simple hill climbing was used
    path.clear();
    path.push_back(3);

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
    pos[2] = next[2] = start[2];

    return path;
}

// steepest accent hill climbing algorithm

vector<int> maze ::steepest_ascent_hill_climbing()
{
    path.clear();
    path.push_back(4);

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
    pos[2] = next[2] = start[2];

    return path;
}

// simulated annealing

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
    path.clear();
    path.push_back(5);
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
    pos[2] = next[2] = start[2];

    return path;
}

// BFS

vector<int> maze::trace_back_path(node_list space)
{
    path.clear();
    path.push_back(0);

    if (space.find(node(goal, 0)) > -1)
    {
        node_list temp_path;

        while (heuristic(space.list[space.list.size() - 1].cordinate) != 0)
        {
            space.pop();
        }

        node current = space.pop();

        int pid = -1;

        while (space.list.size() > 0)
        {
            if (current.pcordinate.size() == 3)
            {
                pid = space.find(node(current.pcordinate, 0));
                temp_path.add(current);
                current = space.list[pid];
            }
            else
                break;
        }

        int x, y, z = 0;

        vector<int> rpath;

        for (int i = 0; i < temp_path.list.size(); i++)
        {
            x = temp_path.list[i].cordinate[1] - temp_path.list[i].pcordinate[1];
            y = temp_path.list[i].cordinate[0] - temp_path.list[i].pcordinate[0];
            z = temp_path.list[i].cordinate[2] - temp_path.list[i].pcordinate[2];

            if (!x && y < 0 && !z)
                rpath.push_back(1);
            if (!x && y > 0 && !z)
                rpath.push_back(2);

            if (x > 0 && !y && !z)
                rpath.push_back(3);
            if (x < 0 && !y && !z)
                path.push_back(4);

            if (!x && !y && z > 0)
                rpath.push_back(5);
            if (!x && !y && z < 0)
                rpath.push_back(6);
        }
        path.clear();
        path.push_back(0);
        for (int i = rpath.size() - 1; i >= 0; i--)
            path.push_back(rpath[i]);
    }

    return path;
}

vector<int> maze::bfs()
{
    node_list space;
    space.add(node(start, heuristic(start)));

    int count = 0;

    for (int k = 0; k < max_depth; k++)
    {
        for (int i = 0; i < max_row; i++)
        {
            for (int j = 0; j < max_col; j++)
            {
                if (matrix_map[i][j][k] == 0)
                    count++;
            }
        }
    }

    while (space.list.size() < count && (space.find(node(goal, 0))) == -1)
    {
        node current = space.list[space.find_least()];
        space.list[space.find_least()].toggle_active();

        pos.clear();
        next.clear();
        next = pos = current.cordinate;

        for (int r = 1; r <= 6; r++)
        {
            if (apply(r))
            {
                node successor(next, heuristic(next), &current);
                if (space.find(successor) == -1)
                    space.add(successor);
                if (!heuristic(successor.cordinate))
                    break;
            }
        }
    }

    pos[0] = next[0] = start[0];
    pos[1] = next[1] = start[1];
    pos[2] = next[2] = start[2];

    trace_back_path(space);
    path[0] = 1;

    return path;
}

// DFS

bool maze::verify(vector<int> tpath)
{
    pos.clear();
    next.clear();
    pos = next = start;

    for (int i = 1; i < path.size(); i++)
    {
        if (!apply(tpath[i]))
            return false;
        else
            register_last_rule();
    }
    return true;
}

vector<int> maze::dfs()
{
    path.clear();
    path.push_back(2);

    node_list space;

    node current(start, heuristic(start));
    space.add(current);

    int count = 0;

    for (int k = 0; k < max_depth; k++)
    {
        for (int i = 0; i < max_row; i++)
        {
            for (int j = 0; j < max_col; j++)
            {
                if (matrix_map[i][j][k] == 0)
                    count++;
            }
        }
    }

    int pr;
    vector<int> rule_applied;

    pos.clear();
    next.clear();
    next = pos = current.cordinate;
    node nextc = current;

    bool backtrack = false;

    while (space.list.size() < count && (space.find(node(goal, 0))) == -1)
    {
        int r = 1;

        if (backtrack)
        {
            backtrack = false;
            r = pr + 1;
        }

        for (; r <= 6; r++)
        {
            if (apply(r) && space.find(node(next, 0)) == -1)
            {
                break;
            }
        }

        if (r > 6)
        {
            if (current.cordinate != start)
            {
                backtrack = true;
                pr = rule_applied[rule_applied.size() - 1];
                rule_applied.pop_back();
                current = space.pop();
                current = space.list[space.find(node(current.pcordinate, 0))];
            }
            else
            {
                break;
            }
        }

        else
        {
            space.list[space.find(current)].toggle_active();
            nextc = node(next, heuristic(next), &current);
            space.add(nextc);

            current = nextc;
            pos.clear();
            next.clear();
            pos = next = current.cordinate;

            rule_applied.push_back(r);

            current = node(next, heuristic(next), &current);
        }
    }

    pos[0] = next[0] = start[0];
    pos[1] = next[1] = start[1];
    pos[2] = next[2] = start[2];

    for (auto value : rule_applied)
        path.push_back(value);

    if (!verify(path))
    {
        path.clear();
        path.push_back(0);
    }
    return path;
}

//attempt 3 a*

node_list maze ::propagate(node_list space, node root)
{
    vector<vector<int>> parents;
    vector<int> latest_child_index;
    bool backtrack = false;

    parents.push_back(root.cordinate);
    node child(start, 0);

    int pr = 0;
    int child_id = 0;

    while (parents.size() > 0)
    {
        if (space.findc(root, 0) > -1)
        {
            if (backtrack)
            {
                backtrack = false;
                if (space.findc(root, pr) > -1)
                    child_id = space.findc(root, pr);
            }
            else
                child_id = space.findc(root, 0);

            child = space.list[child_id];
            space.update(child);
            parents.push_back(child.cordinate);
            latest_child_index.push_back(child_id);
            // update old stuff
            root = child;
        }
        else
        {
            backtrack = true;
            parents.pop_back();
            latest_child_index.pop_back();
            pr = latest_child_index[latest_child_index.size() - 1];
            latest_child_index.pop_back();
            root = space.list[space.find(node(parents[parents.size() - 1], 0))];
            // no more children setup backtrack
        }
    }

    return space;
}

vector<int> maze::a_star()
{
    path.clear();
    path.push_back(6);

    node_list space;
    node current(start, heuristic(start));
    space.add(current);

    pos.clear();
    next.clear();
    pos = next = current.cordinate;
    node nextc = current;
    node found = current;

    while (space.find(node(goal, 0)) == -1)
    {

        if (space.find_least() != -1)
        {
            // printing least
            current = space.list[space.find_least()];
            // cout << "current node is :" << endl;
            // current.display();
        }
        else
            break;

        space.list[space.find(current)].toggle_active();
        pos.clear();
        next.clear();
        pos = next = current.cordinate;

        // cout << "----------------for-------------------" << endl;
        for (int r = 1; r <= 6; r++)
        {
            if (apply(r))
            {
                // cout << r << " is aplicable" << endl;
                nextc = node(next, heuristic(next), &current);
                // cout << "next node is :" << endl;
                // nextc.display();

                if (space.find(nextc) > -1)
                {
                    found = space.list[space.find(nextc)];
                    if (found.cordinate != start)
                    {
                        if (found.active)
                        {
                            if (found.f > nextc.f)
                                space.list[space.find(found)] = nextc;
                        }
                        else
                        {
                            if (found.f > nextc.f)
                            {
                                space.list[space.find(found)] = nextc;
                                space = propagate(space, nextc);
                                // in closed and we need to propagate the change down the tree}
                            }
                        }
                    }
                }
                else
                {
                    space.add(nextc);
                }
            }
        }
        // cout << "----------------endfor----------------" << endl;
    }

    pos[0] = next[0] = start[0];
    pos[1] = next[1] = start[1];
    pos[2] = next[2] = start[2];

    trace_back_path(space);
    path[0] = 6;

    return path;
}