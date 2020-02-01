#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <GL/glut.h>
#include <cstdlib>
#include <cctype>
using namespace std;

#define WINDOW_HEI  500
#define WINDOW_WID  500

enum { SET_COLOR, DRAW_POINT, DRAW_LINE, DRAW_POLY };

typedef struct
{
    int type;
    unsigned size;
    vector<int> xy;
    double r, g, b;
}Command;

vector<Command> commands; 

void initGL(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WID, 0.0, WINDOW_HEI);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (unsigned i = 0; i < commands.size(); i++)
    {
        Command cmd = commands[i];

        switch (cmd.type)
        {
        case SET_COLOR:
            glColor3f(cmd.r, cmd.g, cmd.b);
            break;
        case DRAW_POINT:
            glPointSize(cmd.size);
            glBegin(GL_POINTS);
            glVertex3f(cmd.xy[0], cmd.xy[1], 0);
            glEnd();
            break;
        case DRAW_LINE:
            glLineWidth(cmd.size);
            glBegin(GL_LINES);
            glVertex3f(cmd.xy[0], cmd.xy[1], 0);
            glVertex3f(cmd.xy[2], cmd.xy[3], 0);
            glEnd();
            break;
        case DRAW_POLY:
            glBegin(GL_POLYGON);
            for (unsigned j = 0; j < cmd.xy.size(); j += 2)
                glVertex3f(cmd.xy[j], cmd.xy[j + 1], 0);
            glEnd();
            break;
        }
    }
    glFlush();
}

vector<string> split(string line)
{
    vector<string> parts;
    unsigned head = 0;
    while (head < line.length() && isspace(line[head]))
        head++;
    if (head >= line.length())
        return parts;
    while (head < line.length())
    {
        unsigned tail = head;
        while (tail < line.length() && !isspace(line[tail]))
            tail++;
        if (tail < line.length())
            parts.push_back(line.substr(head, tail - head));
        else 
            parts.push_back(line.substr(head));
        head = tail;
        while (head < line.length() && isspace(line[head]))
            head++;
    }
    return parts;
}

Command fill_set_color(vector<string> parts)
{
    Command command;
    command.type = SET_COLOR;
    if (parts.size() != 4)
    {
        cout << "Invalid set_color command found in file" << endl;
        exit(1);
    }
    command.r = atof(parts[1].c_str());
    command.g = atof(parts[2].c_str());
    command.b = atof(parts[3].c_str());
    return command;
}

Command fill_draw_point(vector<string> parts)
{
    Command command;
    command.type = DRAW_POINT;
    if (parts.size() != 4)
    {
        cout << "Invalid draw_point command found in file" << endl;
        exit(1);
    }
    command.size = atoi(parts[1].c_str());
    for (int i = 0; i < 2; i++)
        command.xy.push_back(atoi(parts[i + 2].c_str()));
    return command;
}

Command fill_draw_line(vector<string> parts)
{
    Command command;
    command.type = DRAW_LINE;
    if (parts.size() != 6)
    {
        cout << "Invalid draw_line command found in file" << endl;
        exit(1);
    }
    command.size = atoi(parts[1].c_str());
    for (int i = 0; i < 4; i++)
        command.xy.push_back(atoi(parts[i + 2].c_str()));
    return command;
}

Command fill_draw_polygon(vector<string> parts)
{
    Command command;
    command.type = DRAW_POLY;
    if (parts.size() < 2)
    {
        cout << "Invalid draw_polygon command found in file" << endl;
        exit(1);
    }
    command.size = atoi(parts[1].c_str());

    if (parts.size() < command.size * 2 + 2)
    {
        cout << "Invalid draw_polygon command found in file" << endl;
        exit(1);
    }
    for (unsigned i = 0; i < command.size * 2; i++)
        command.xy.push_back(atoi(parts[i + 2].c_str()));
    return command;
}

vector<Command> load_file(string filename)
{
    vector<Command> commands;
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Unable to open file: \"" << filename << "\"" << endl;
        exit(1);
    }
    string line;
    while (getline(file, line))
    {
        vector<string> parts = split(line);
        if (!parts.empty())
        {
            Command command;
            if (parts[0] == "set_color")
                command = fill_set_color(parts);
            else if (parts[0] == "draw_point")
                command = fill_draw_point(parts);
            else if (parts[0] == "draw_line")
                command = fill_draw_line(parts);
            else if (parts[0] == "draw_polygon")
                command = fill_draw_polygon(parts);

            commands.push_back(command);
        }
    }
    return commands;
}

int main(int argc, char** argv)
{
    cout << "Type of chart(column.txt, line.txt, point.txt, area.txt): ";
    string filename;
    getline(cin, filename);

    commands = load_file(filename);

    glutInit(&argc, argv);
    glutCreateWindow("Chart");
    glutInitWindowSize(WINDOW_WID, WINDOW_HEI);
    initGL();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}