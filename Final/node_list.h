#include <vector>
#include "node.h"

using namespace std;

class node_list
{
public:
    vector<node> list;

    // display
    void display();

    // adding nodes
    inline void add(node n)
    {
        list.push_back(n);
    }

    // remove
    inline void remove(int index)
    {
        list.erase(list.begin() + index);
    }

    // searching a node
    int find(node);
    int findc(node, int);

    int find_least();

    void replace(int, node *);

    node pop();
    void update(node);
};

void node_list ::display()
{
    cout << endl;
    cout << "**********************************" << endl;
    cout << " BEGINING OF NODELIST" << endl;
    cout << endl;
    for (int i = 0; i < list.size(); i++)
    {
        list[i].display();
    }
    cout << " THE SIZE IS " << list.size() << endl;
    cout << "**********************************" << endl;
    cout << endl;
}

int node_list ::find(node n)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].cordinate == n.cordinate)
            return i;
    }
    return -1;
}

int node_list ::findc(node n, int from)
{
    for (int i = from; i < list.size(); i++)
        if (list[i].pcordinate == n.cordinate)
            return i;

    return -1;
}

void node_list ::update(node n)
{
    if (find(n) > -1)
    {
        list[find(n)].g = list[find(node(n.pcordinate, 0))].g + sqrt((pow((n.cordinate[0] - n.pcordinate[0]), 2) + pow((n.cordinate[1] - n.pcordinate[1]), 2) + pow((n.cordinate[2] - n.pcordinate[2]), 2)));
        list[find(n)].f = list[find(n)].g + list[find(n)].h;
    }
}

int node_list::find_least()
{
    float min = MAXFLOAT;
    int index = -1;
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].f < min && list[i].active)
        {
            min = list[i].f;
            index = i;
        }
    }
    return index;
}

void node_list ::replace(int k, node *p)
{
    list[k].pcordinate.clear();
    for (auto value : p->cordinate)
        list[k].pcordinate.push_back(value);
}

node node_list ::pop()
{
    node temp = list[list.size() - 1];
    list.pop_back();
    return temp;
}
