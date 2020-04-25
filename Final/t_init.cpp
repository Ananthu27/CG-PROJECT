#include <iostream>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <GL/glut.h>
#include "t_maze.h"
#include "translate.h"

using namespace std;
using namespace std::chrono;

const float PI = 3.141593;
// #define RAD(O) (((O)*PI) / 180.0)

// objects
int d = 5;
maze m(d);
translate t;

// light parameters
float ambient = 10.0;
float diffuse = 100.0;
// float specular = 0.0;

// window parameters
float side = 120;
float length = 16.0 * side;
float width = 9.0 * side;

// graphic parameters
float least_count = 0.001;
float offset = 20.0;
float len;

// graphic timer parameters (dynamic)
float angle = 0.0;
float speed = 15;
int i_count = 0;
float move_x = 0, move_y = 0, move_z = 0;
float save_x = move_x;
float save_y = move_y;
float save_z = move_z;

// control variables
bool rotation = true;
float rotate_h = 0;
float rotate_v = 0;
float light = true;
bool xray = false;
int option = 0;
bool Pause = false;
bool help = false;

vector<int> path;

void init();
void display_single_node(int, int, int, float, float, bool, float, bool, bool, bool);
void display_boundary();
void display_marker_node(int, int, int, float, bool, float, bool);
void display_3d_map();
void drawLight();
void display();
void reshape(int, int);
void timer(int);
void handle_key_stroke(unsigned char, int, int);
void calc_stats();

// colors
const float background[] = {0.7, 0.7, 0.8};
const GLubyte base[3] = {197, 202, 233};
const GLubyte start_and_goal[3] = {232, 234, 246};
const GLubyte wall[3] = {121, 134, 203};
const GLubyte wall_outline[3] = {159, 168, 218};
const GLubyte marker[3] = {48, 63, 159};
const GLubyte marker_outline[3] = {57, 73, 171};

// help menu
char help_text[] = "-- 3D-Maze solver ---\n"
                   "--- Commands ---\n"
                   " 'p' - Pause | Play\n"
                   " 'r' - Restart\n"
                   " 'e' - Exit\n"
                   " ' ' - Toggle Rotation\n"
                   " 'h' - Toggle Help Menu\n"
                   " 'l' - Toggle Light\n"
                   " 's' - Rotate Clockwise\n"
                   " 'f' - Rotate Counter-Clockwise\n"
                   " 'n' - Load New Maze\n\n"
                   " --- Solve Maze with --- \n"
                   " '1' - Breadth-First Search\n"
                   " '2' - Depth-First Search\n"
                   " '3' - Simple Hill Climbing algorithm\n"
                   " '4' - Steepest Ascent Hill Climbing\n"
                   " '5' - Simulated Annealing\n"
                   " '6' - A*\n";

//main program
int main(int argc, char **argv)
{
    cout << "\033[2J\033[1;1H";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(length, width);

    glutCreateWindow("Simulation Search Alglorithms");
    glutDisplayFunc(display);
    glutKeyboardFunc(handle_key_stroke);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    init();
    // calc_stats();
    path.clear();
    path = m.bfs();

    glutMainLoop();
}

void init()
{
    glClearColor(background[0], background[1], background[2], 1.0);
    glEnable(GL_DEPTH_TEST);
}

void display_single_node(int i, int j, int k,
                         float len, float z_pos,
                         bool border = 0, float shrink = 0, bool lock = true, bool three_d = false)
{
    float offset_x = j * len;
    float offset_y = i * len;
    float save_x = move_x;
    float save_y = move_y;
    float save_z = move_z;
    float depth = len / float(450);

    if (lock)
        move_x = move_y = move_z = 0;

    float z_level = k * depth + move_z / float(450);

    float x1 = (offset + offset_x) + shrink + move_x;
    float y1 = (width - (offset + offset_y)) - shrink + move_y;
    float x2 = (offset + offset_x + len) - shrink + move_x;
    float y2 = (width - (offset + offset_y + len)) + shrink + move_y;

    // translation

    x1 = t.translate_x(x1);
    x2 = t.translate_x(x2);
    y1 = t.translate_y(y1);
    y2 = t.translate_y(y2);

    if (border == 0)
        glBegin(GL_POLYGON);
    else
    {
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
    }

    // bottom
    glVertex3f(x1, y1, z_pos + z_level);
    glVertex3f(x1, y2, z_pos + z_level);
    glVertex3f(x2, y2, z_pos + z_level);
    glVertex3f(x2, y1, z_pos + z_level);

    glEnd();
    if (three_d)
    {
        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // top
        glVertex3f(x1, y1, depth + z_level);
        glVertex3f(x1, y2, depth + z_level);
        glVertex3f(x2, y2, depth + z_level);
        glVertex3f(x2, y1, depth + z_level);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // right
        glVertex3f(x2, y2, z_pos + z_level);
        glVertex3f(x2, y2, depth + z_level);
        glVertex3f(x2, y1, depth + z_level);
        glVertex3f(x2, y1, z_pos + z_level);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // left
        glVertex3f(x1, y1, z_pos + z_level);
        glVertex3f(x1, y1, depth + z_level);
        glVertex3f(x1, y2, depth + z_level);
        glVertex3f(x1, y2, z_pos + z_level);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // front
        glVertex3f(x1, y2, z_pos + z_level);
        glVertex3f(x1, y2, depth + z_level);
        glVertex3f(x2, y2, depth + z_level);
        glVertex3f(x2, y2, z_pos + z_level);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // back
        glVertex3f(x2, y1, z_pos + z_level);
        glVertex3f(x2, y1, depth + z_level);
        glVertex3f(x1, y1, depth + z_level);
        glVertex3f(x1, y1, z_pos + z_level);

        glEnd();
    }

    if (lock)
    {
        move_x = save_x;
        move_y = save_y;
        move_z = save_z;
    }
}

void display_boundary()
{
    float x1 = 0.0;
    float y1 = width;
    float x2 = offset * 2 + (len * m.max_col);
    float y2 = width - (offset * 2 + (len * m.max_row));

    float depth = len / float(450);
    float z1 = 0.0;
    float z2 = m.max_depth * depth;

    // translate points

    x1 = t.translate_x(x1);
    x2 = t.translate_x(x2);
    y1 = t.translate_y(y1);
    y2 = t.translate_y(y2);

    // displaying the boundary
    {
        // bottom
        glBegin(GL_POLYGON);
        {
            glVertex3f(x1, y1, z1 - least_count);
            glVertex3f(x1, y2, z1 - least_count);
            glVertex3f(x2, y2, z1 - least_count);
            glVertex3f(x2, y1, z1 - least_count);
        }
        glEnd();

        // top
        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(x1, y1, z2);
            glVertex3f(x1, y2, z2);
            glVertex3f(x2, y2, z2);
            glVertex3f(x2, y1, z2);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(x1, y1, z1);
            glVertex3f(x1, y1, z2);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(x2, y1, z1);
            glVertex3f(x2, y1, z2);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(x1, y2, z1);
            glVertex3f(x1, y2, z2);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(x2, y2, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    // displaying the start block

    x1 = offset + len * m.start[1];
    y1 = width - (offset + len * m.start[0]);
    z1 = m.start[2] * depth;

    float s = len / (float)(450);

    x1 = t.translate_x(x1);
    y1 = t.translate_y(y1);

    glColor3ubv(start_and_goal);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y1, z1 + least_count);
    glVertex3f(x1, y1 - s, z1 + least_count);
    glVertex3f(x1 + s, y1 - s, z1 + least_count);
    glVertex3f(x1 + s, y1, z1 + least_count);
    glEnd();

    // displaying the goal block

    x1 = offset + len * m.goal[1];
    y1 = width - (offset + len * m.goal[0]);
    z1 = m.goal[2] * depth;

    x1 = t.translate_x(x1);
    y1 = t.translate_y(y1);

    glColor3ubv(start_and_goal);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y1, z1 + least_count);
    glVertex3f(x1, y1 - s, z1 + least_count);
    glVertex3f(x1 + s, y1 - s, z1 + least_count);
    glVertex3f(x1 + s, y1, z1 + least_count);
    glEnd();
}

void display_marker_node(int i, int j, int k, float len, bool border = 0, float shrink = 0)
{
    float offset_x = j * len;
    float offset_y = i * len;

    float depth = len / (float)450;
    float z1 = k * depth + move_z / (float)450;
    float z0 = k * depth + depth / (float)2 + move_z / (float)450;
    float z2 = (k + 1) * depth + move_z / (float)450;

    float x1 = (offset + offset_x) + shrink + move_x;
    float y1 = (width - (offset + offset_y)) - shrink + move_y;
    float x2 = (offset + offset_x + len) - shrink + move_x;
    float y2 = (width - (offset + offset_y + len)) + shrink + move_y;
    float x0 = x1 + (x2 - x1) / float(2);
    float y0 = y1 + (y2 - y1) / float(2);

    // translation

    x0 = t.translate_x(x0);
    x1 = t.translate_x(x1);
    x2 = t.translate_x(x2);
    y0 = t.translate_y(y0);
    y1 = t.translate_y(y1);
    y2 = t.translate_y(y2);

    {
        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // bottom left
        glVertex3f(x0, y0, z1);
        glVertex3f(x1, y1, z0);
        glVertex3f(x1, y2, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // bottom front
        glVertex3f(x0, y0, z1);
        glVertex3f(x1, y2, z0);
        glVertex3f(x2, y2, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // bottom right
        glVertex3f(x0, y0, z1);
        glVertex3f(x2, y2, z0);
        glVertex3f(x2, y1, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // bottom back
        glVertex3f(x0, y0, z1);
        glVertex3f(x1, y1, z0);
        glVertex3f(x2, y1, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // top left
        glVertex3f(x0, y0, z2);
        glVertex3f(x1, y1, z0);
        glVertex3f(x1, y2, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // top front
        glVertex3f(x0, y0, z2);
        glVertex3f(x1, y2, z0);
        glVertex3f(x2, y2, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // top right
        glVertex3f(x0, y0, z2);
        glVertex3f(x2, y2, z0);
        glVertex3f(x2, y1, z0);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }
        // top back
        glVertex3f(x0, y0, z2);
        glVertex3f(x1, y1, z0);
        glVertex3f(x2, y1, z0);

        glEnd();
    }
}

void display_3d_map()
{
    glRotatef(-60, 1.0, 0.0, 0.0);
    glRotatef(angle + rotate_h, 0.0, 0.0 + rotate_v, 1.0);

    // displaying light
    {
        float xl = offset + len * (m.start[1] + 0.5) + move_x;
        float yl = width - (offset + len * (m.start[0] + 0.5)) + move_y;
        float zl = (((m.start[2]) + 1) * len + least_count) / (float)450 + move_z / (float)450;

        xl = t.translate_x(xl);
        yl = t.translate_y(yl);

        GLfloat Position[] = {xl, yl, zl, 1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, Position);

        // use to spot light position

        // glPointSize(5);
        // glColor3f(0.0, 0.0, 0.0);
        // glBegin(GL_POINTS);
        // glVertex3f(xl, yl, zl);
        // glEnd();
    }

    // displaying boundary
    {
        glColor3ubv(base);
        glLineWidth(1);
        display_boundary();
    }

    // // displaying wall
    {
        glColor3ubv(wall);

        for (int k = 0; k < m.max_depth; k++)
        {
            for (int i = 0; i < m.max_row; i++)
            {
                for (int j = 0; j < m.max_col; j++)
                {
                    if (m.matrix_map[i][j][k] == 1)
                    {
                        display_single_node(i, j, k, len, 0.0, 0, 0, true, true);
                    }
                }
            }
        }

        // border

        glColor3ubv(wall_outline);

        for (int k = 0; k < m.max_depth; k++)
        {
            for (int i = 0; i < m.max_row; i++)
            {
                for (int j = 0; j < m.max_col; j++)
                {
                    if (m.matrix_map[i][j][k] == 1)
                    {
                        display_single_node(i, j, k, len, 0.0, 1, 0, true, true);
                    }
                }
            }
        }
    }

    // displaying the current position
    {
        float shrink = len * 0.250;
        glColor3ubv(marker);
        display_marker_node(m.start[0], m.start[1], m.start[2], len, false, shrink);
        //border
        glColor3ubv(marker_outline);
        display_marker_node(m.start[0], m.start[1], m.start[2], len, true, shrink);
    }
}

void drawLight()
{
    // GLfloat mod_ambient[] = {0.2, 0.2, 0.2, 1.0};
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mod_ambient);

    // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    float Ambient[] = {(float)0.01 * ambient, (float)0.01 * ambient, (float)0.01 * ambient, 1.0};
    float Diffuse[] = {(float)0.01 * diffuse, (float)0.01 * diffuse, (float)0.01 * diffuse, 1.0};
    // float Specular[] = {(float)0.01 * specular, (float)0.01 * specular, (float)0.01 * specular, 1.0};

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    int units = (m.max_row >= m.max_col) ? m.max_row : m.max_col;
    len = (((m.max_row >= m.max_col) ? width : length) - offset * 2) / units;

    glTranslatef(0.0, 0.0, -6.0);
    float line = 2.7;

    if (help)
    {
        glColor3ubv(marker);
        glRasterPos2f(-5, line);
        for (char *c = help_text; *c != '\0'; c++)
        {
            if (*c == '\n')
            {
                line -= 0.2;
                glRasterPos2f(-5, line);
            }
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    }

    if (light)
    {
        drawLight();
    }
    display_3d_map();

    glutSwapBuffers();
    glDisable(GL_LIGHTING);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, length / width, 2.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
    char ch;

    // update path movement here
    if (!Pause)
    {
        switch (path[i_count])
        {
        case 1:
            if (move_y >= (len + save_y))
            {
                i_count++;
                save_y = move_y = len + save_y;
            }
            else
                move_y += speed;
            break;

        case 2:
            if (move_y <= (save_y - len))
            {
                i_count++;
                save_y = move_y = save_y - len;
            }
            else
                move_y -= speed;
            break;

        case 3:
            if (move_x >= (len + save_x))
            {
                i_count++;
                save_x = move_x = len + save_x;
            }
            else
                move_x += speed;
            break;

        case 4:
            if (move_x <= (save_x - len))
            {
                i_count++;
                save_x = move_x = save_x - len;
            }
            else
                move_x -= speed;
            break;

        case 5:
            if (move_z >= (len + save_z))
            {
                i_count++;
                save_z = move_z = len + save_z;
            }
            else
                move_z += speed;
            break;

        case 6:
            if (move_z <= (save_z - len))
            {
                i_count++;
                save_z = move_z = save_z - len;
            }
            else
                move_z -= speed;
            break;
        }
    }
    if (rotation)
    {
        angle += 0.3;
    }
}

vector<int> path_id(int p)
{
    path.clear();
    switch (p)
    {
    case 1:
        path = m.bfs();
        break;
    case 2:
        path = m.dfs();
        break;
    case 3:
        path = m.simple_hill_climb();
        break;
    case 4:
        path = m.steepest_ascent_hill_climbing();
        break;
    case 5:
        path = m.simulated_annealing();
        break;
    case 6:
        path = m.a_star();
        break;
    }
    return path;
}

void calc_stats()
{
    cout << "--- Stats ---" << endl;
    cout << "\nAlgorithm \t\tRules \tTime" << endl;
    auto start = high_resolution_clock::now();
    auto stop = start;

    for (int p = 1; p <= 6; p++)
    {
        path.clear();

        start = high_resolution_clock::now();
        path = path_id(p);
        stop = high_resolution_clock::now();

        auto duratoin = duration_cast<microseconds>(stop - start);

        switch (p)
        {
        case 1:
            cout << "Breadth First Search  ";
            break;
        case 2:
            cout << "Depth First Search    ";
            break;
        case 3:
            cout << "Simple Hill Climbing  ";
            break;
        case 4:
            cout << "Steepest Hill Climbing";
            break;
        case 5:
            cout << "Simulated Annealing   ";
            break;
        case 6:
            cout << "A*                    ";
            break;
        }
        cout << " \t" << path.size();
        cout << " \t" << duratoin.count() << endl;
    }
}

void handle_key_stroke(unsigned char c, int x, int y)
{
    int p = (int)c - 48;

    if (c != 'h' && help)
    {
        help = false;
        rotation = true;
        Pause = false;
    }
    switch (c)
    {
    case ' ':
        rotation = !rotation;
        break;

    case 's':
        if (!rotation)
            rotate_h -= 4;
        break;

    case 'f':
        if (!rotation)
            rotate_h += 4;
        break;

    case '1':
        path.clear();
        path = m.bfs();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case '2':
        path.clear();
        path = m.dfs();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case '3':
        path.clear();
        path = m.simple_hill_climb();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case '4':
        path.clear();
        path = m.steepest_ascent_hill_climbing();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case '5':
        path.clear();
        path = m.simulated_annealing();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case '6':
        path.clear();
        path = m.a_star();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case 'r':
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case 'p':
        Pause = !Pause;
        break;

    case 'l':
        light = !light;
        break;

    case 'h':
        help = !help;
        if (help)
        {
            Pause = true;
            rotation = false;
        }
        else
        {
            Pause = false;
            rotation = true;
        }
        break;

    case 'n':
        m = maze(d);
        path = m.simple_hill_climb();
        move_x = move_y = move_z = 0;
        save_x = save_y = save_z = 0;
        i_count = 1;
        break;

    case 'e':
        exit(0);
        break;

    default:
        break;
    }
}