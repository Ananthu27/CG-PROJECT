#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "maze.h"
#include "translate.h"

using namespace std;

// objects
maze m(20);
translate t;

// window parameters
float side = 100;
float length = 16.0 * side;
float width = 9.0 * side;

// graphic parameters
float offset = 20.0;
float len;

// graphic timer parameters (dynamic)
float angle = 0.0;
float speed = 15;
int i_count = 0;
float move_x = 0, move_y = 0;
float save_x = move_x;
float save_y = move_y;

vector<int> path;

void init();
void display_single_node(int, int, float, float, bool, float, bool, bool, bool);
void display();
void reshape(int, int);
void timer(int);

// colors
const float background[] = {0.8, 0.7, 0.7};
const GLubyte base[3] = {255, 204, 188};
const GLubyte start_and_goal[3] = {251, 233, 231};
const GLubyte wall[3] = {255, 138, 101};
const GLubyte wall_outline[3] = {255, 171, 145};
const GLubyte marker[3] = {230, 74, 25};
const GLubyte marker_outline[3] = {244, 81, 30};

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(length, width);
    glutCreateWindow("this is a simple starting window");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();

    // m.display();

    // m.display_path(path = m.simple_hill_climb());
    // m.reset_path();

    // m.display_path(path = m.steepest_ascent_hill_climbing());
    // m.reset_path();

    m.display_path(path = m.simulated_annealing());
    m.reset_path();

    glutMainLoop();
}

void init()
{
    glClearColor(background[0], background[1], background[2], 1.0);
    glEnable(GL_DEPTH_TEST);
}

void display_single_node(int i, int j, float len, float z_pos, bool border = 0, float shrink = 0, bool lock = true, bool three_d = false, bool minimap = false)
{
    float offset_x = j * len;
    float offset_y = i * len;
    float save_x = move_x;
    float save_y = move_y;
    float depth = len / float(450);

    if (lock)
        move_x = move_y = 0;

    float x1 = (offset + offset_x) + shrink + move_x;
    float y1 = (width - (offset + offset_y)) - shrink + move_y;
    float x2 = (offset + offset_x + len) - shrink + move_x;
    float y2 = (width - (offset + offset_y + len)) + shrink + move_y;

    if (minimap)
    {
        x1 = t.translate_x_to_minimap(x1);
        x2 = t.translate_x_to_minimap(x2);
        y1 = t.translate_y_to_minimap(y1, width);
        y2 = t.translate_y_to_minimap(y2, width);
    }

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
    glVertex3f(x1, y1, z_pos);
    glVertex3f(x1, y2, z_pos);
    glVertex3f(x2, y2, z_pos);
    glVertex3f(x2, y1, z_pos);

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
        glVertex3f(x1, y1, depth);
        glVertex3f(x1, y2, depth);
        glVertex3f(x2, y2, depth);
        glVertex3f(x2, y1, depth);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // right
        glVertex3f(x2, y2, z_pos);
        glVertex3f(x2, y2, depth);
        glVertex3f(x2, y1, depth);
        glVertex3f(x2, y1, z_pos);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // left
        glVertex3f(x1, y1, z_pos);
        glVertex3f(x1, y1, depth);
        glVertex3f(x1, y2, depth);
        glVertex3f(x1, y2, z_pos);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // front
        glVertex3f(x1, y2, z_pos);
        glVertex3f(x1, y2, depth);
        glVertex3f(x2, y2, depth);
        glVertex3f(x2, y2, z_pos);

        glEnd();

        if (border == 0)
            glBegin(GL_POLYGON);
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        // back
        glVertex3f(x2, y1, z_pos);
        glVertex3f(x2, y1, depth);
        glVertex3f(x1, y1, depth);
        glVertex3f(x1, y1, z_pos);

        glEnd();
    }

    if (lock)
    {
        move_x = save_x;
        move_y = save_y;
    }
}

void display_boundary(bool minimap = false)
{
    float x1 = 0.0;
    float y1 = width;
    float x2 = offset * 2 + (len * m.max_col);
    float y2 = width - (offset * 2 + (len * m.max_row));

    // translate points
    if (minimap)
    {
        x1 = t.translate_x_to_minimap(x1);
        x2 = t.translate_x_to_minimap(x2);
        y1 = t.translate_y_to_minimap(y1, width);
        y2 = t.translate_y_to_minimap(y2, width);
    }

    x1 = t.translate_x(x1);
    x2 = t.translate_x(x2);
    y1 = t.translate_y(y1);
    y2 = t.translate_y(y2);

    // displaying the boundary

    glBegin(GL_LINE_LOOP);
    {
        glVertex3f(x1, y1, 0);
        glVertex3f(x1, y2, 0);
        glVertex3f(x2, y2, 0);
        glVertex3f(x2, y1, 0);
    }
    glEnd();

    // displaying the start block

    x1 = offset + len * m.start[1];
    y1 = width - (offset + len * m.start[0]);

    float s = len / (float)(450);
    if (minimap)
    {
        x1 = t.translate_x_to_minimap(x1);
        y1 = t.translate_y_to_minimap(y1, width);
        s /= t_shrink_to_mini_map;
    }

    x1 = t.translate_x(x1);
    y1 = t.translate_y(y1);

    glColor3ubv(start_and_goal);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x1, y1 - s, 0.0);
    glVertex3f(x1 + s, y1 - s, 0.0);
    glVertex3f(x1 + s, y1, 0.0);
    glEnd();

    // displaying the goal block

    x1 = offset + len * m.goal[1];
    y1 = width - (offset + len * m.goal[0]);

    if (minimap)
    {
        x1 = t.translate_x_to_minimap(x1);
        y1 = t.translate_y_to_minimap(y1, width);
    }

    x1 = t.translate_x(x1);
    y1 = t.translate_y(y1);

    glColor3ubv(start_and_goal);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x1, y1 - s, 0.0);
    glVertex3f(x1 + s, y1 - s, 0.0);
    glVertex3f(x1 + s, y1, 0.0);
    glEnd();
}

void display_minimap()
{
    int units = (m.max_row >= m.max_col) ? m.max_row : m.max_col;
    len = (((m.max_row >= m.max_col) ? width : length) - offset * 2) / units;

    glTranslatef(0.0, 0.0, -2.0);

    //displaying boundary
    {
        glColor3ubv(base);
        display_boundary(true);
    }

    // displaying map
    {
        glColor3ubv(wall);

        for (int i = 0; i < m.max_row; i++)
        {
            for (int j = 0; j < m.max_col; j++)
            {
                if (m.matrix_map[i][j] == 1)
                {
                    display_single_node(i, j, len, 0.0, 0, 0, true, false, true);
                }
            }
        }

        glColor3ubv(wall_outline);

        for (int i = 0; i < m.max_row; i++)
        {
            for (int j = 0; j < m.max_col; j++)
            {
                if (m.matrix_map[i][j] == 1)
                {
                    display_single_node(i, j, len, 0.0, 1, 0, true, false, true);
                }
            }
        }
    }

    // displaying the current position
    {
        float shrink = len * 0.250;
        glColor3ubv(marker);
        display_single_node(m.start[0], m.start[1], len, 0, 0, shrink, false, false, true);
        glColor3ubv(marker_outline);
        display_single_node(m.start[0], m.start[1], len, 0, 1, shrink, false, false, true);
    }
}

void display_3d_map()
{
    glTranslatef(0.0, 0.0, -2.0);
    glRotatef(-50, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);

    //displaying boundary
    {
        glColor3ubv(base);
        display_boundary();
    }

    // // displaying wall
    {
        glColor3ubv(wall);

        for (int i = 0; i < m.max_row; i++)
        {
            for (int j = 0; j < m.max_col; j++)
            {
                if (m.matrix_map[i][j] == 1)
                {
                    display_single_node(i, j, len, 0.0, 0, 0, true, true);
                }
            }
        }

        glColor3ubv(wall_outline);

        for (int i = 0; i < m.max_row; i++)
        {
            for (int j = 0; j < m.max_col; j++)
            {
                if (m.matrix_map[i][j] == 1)
                {
                    display_single_node(i, j, len, 0.0, 1, 0, true, true);
                }
            }
        }
    }

    // displaying the current position
    {
        float shrink = len * 0.250;
        glColor3ubv(marker);
        display_single_node(m.start[0], m.start[1], len, 0, 0, shrink, false, true);
        glColor3ubv(marker_outline);
        display_single_node(m.start[0], m.start[1], len, 0, 1, shrink, false, true);
    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    int units = (m.max_row >= m.max_col) ? m.max_row : m.max_col;
    len = (((m.max_row >= m.max_col) ? width : length) - offset * 2) / units;

    display_minimap();

    display_3d_map();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, length / width, 2.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
    char ch;

    // update path movement here
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
        }
    }

    angle += 0.3;
}