#include <iostream>
#include <math.h>

using namespace std;

class node
{

public:
    vector<int> cordinate;
    float f, g, h;
    bool active = true;
    // node *parent;
    vector<int> pcordinate;

    // creating the start node
    node(vector<int>, float);
    // creating normal node
    node(vector<int>, float, node *);
    //toggle active
    inline void toggle_active()
    {
        active = !active;
    }
    // display
    void display();
};

node ::node(vector<int> a, float h_dash)
{
    cordinate.push_back(a[0]);
    cordinate.push_back(a[1]);
    cordinate.push_back(a[2]);
    h = h_dash;
    g = 0;
    f = g + h;
    // parent = NULL;
}

node ::node(vector<int> a, float h_dash, node *p)
{
    cordinate.push_back(a[0]);
    cordinate.push_back(a[1]);
    cordinate.push_back(a[2]);

    pcordinate.push_back(p->cordinate[0]);
    pcordinate.push_back(p->cordinate[1]);
    pcordinate.push_back(p->cordinate[2]);

    // parent = p;
    h = h_dash;
    g = p->g + sqrt((pow((cordinate[0] - p->cordinate[0]), 2) + pow((cordinate[1] - p->cordinate[1]), 2) + pow((cordinate[2] - p->cordinate[2]), 2)));
    f = g + h;
}

void node ::display()
{
    cout << "x : " << cordinate[0] << endl;
    cout << "y : " << cordinate[1] << endl;
    cout << "z : " << cordinate[2] << endl;
    cout << "f : " << f << endl;
    cout << "active : " << active << endl;
    if (pcordinate.size() == 3)
    {
        cout << "---PARENT---" << endl;
        cout << "x : " << pcordinate[0] << endl;
        cout << "y : " << pcordinate[1] << endl;
        cout << "z : " << pcordinate[2] << endl;
    }
    cout << endl;
}