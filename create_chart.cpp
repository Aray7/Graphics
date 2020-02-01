#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iomanip>
using namespace std;

#define WINDOW_HEI  500
#define WINDOW_WID  500

#define GRID_R  1.0
#define GRID_G  1.0
#define GRID_B  1.0
#define GRID_THICK 1

#define CHART_R  0.5
#define CHART_G  1.0
#define CHART_B  0.5

#define POINT_SIZE 6
#define LINE_THICK 6


typedef struct
{
    int x0, y0, x1, y1;
}Box;

void set_color(ofstream &out, double r, double g, double b)
{
    out << "set_color " << fixed << setprecision(1) << r << " " << g << " " << b <<endl;
}

void draw_line(ofstream &out, int wid, int x0, int y0, int x1, int y1)
{
    out << "draw_line " << wid << " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
}

void draw_point(ofstream &out, int size, int x0, int y0)
{
    out << "draw_point " << size << " " << x0 << " " << y0 << endl;
}

void draw_polygon(ofstream &out, int n, int x0, int y0, int x1, int y1,
    int x2, int y2, int x3, int y3)
{
    out << "draw_polygon " << n << " " << x0 << " " << y0 << " ";
    out << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << endl;
}

Box generate_grid(ofstream &out)
{
    set_color(out, GRID_R, GRID_G, GRID_B);

    int border_wid = 2.0*WINDOW_WID/100.0;
    int border_hei = 2.0*WINDOW_HEI/100.0;
    int tick_wid = 60.0*border_wid/100.0;
    int tick_hei = 60.0*border_hei/100.0;

    int grid_wid = (WINDOW_WID - 2*(border_wid + tick_wid)) / 10;
    int grid_hei = (WINDOW_HEI - 2*(border_hei + tick_hei)) / 10;

    int x0, y0, x1, y1;
    x0 = border_wid;
    x1 = WINDOW_WID - (border_wid + tick_wid);
    y0 = (border_hei + tick_hei);
    for (int i = 0; i < 11; i++)
    {
        y1 = y0 + i*grid_hei;
        draw_line(out, GRID_THICK, x0, y1, x1, y1);
    }

    x0 = border_wid + tick_wid;
    y0 = border_hei;
    y1 = WINDOW_HEI - (border_hei + tick_hei);
    draw_line(out, GRID_THICK, x0, y0, x0, y1);

    x0 = border_wid + tick_wid;
    y0 = border_hei;
    y1 = border_hei + tick_hei;
    for (int i = 1; i < 11; i++)
    {
        x1 = x0 + i*grid_wid;
        draw_line(out, GRID_THICK, x1, y0, x1, y1);
    }

    Box box;
    box.x0 = border_wid + tick_wid;
    box.y0 = border_hei + tick_hei;
    box.x1 = WINDOW_WID - box.x0;
    box.y1 = WINDOW_HEI - box.y0;
    return box;
}

void generate_column_chart(vector<int> data)
{
    string filename = "column.txt";
    cout << "Saving chart in " << filename << "..." << endl;
    ofstream file(filename.c_str());

    Box plot_area = generate_grid(file);

    int sx = (plot_area.x1 - plot_area.x0) / (double) data.size();
    int sy = (plot_area.y1 - plot_area.y0) / (double) data.back();

    int half_wid = sx / 4;

    set_color(file, CHART_R, CHART_G, CHART_B);

    for (unsigned i = 0; i < data.size(); i++)
    {
        int x = plot_area.x0 + (i + 1)*sx;
        int y = plot_area.y0 + data[i]*sy;
        int x0 = x - half_wid;
        int y0 = y;
        int x1 = x + half_wid;
        int y1 = y;
        int x2 = x1;
        int y2 = plot_area.y0;
        int x3 = x0;
        int y3 = plot_area.y0;

        draw_polygon(file, 4, x0, y0, x1, y1, x2, y2, x3, y3);
    }
    file.close();
}

void generate_point_chart(vector<int> data)
{
    string filename = "point.txt";
    cout << "Saving chart in " << filename << "..." << endl;
    ofstream file(filename.c_str());

    Box plot_area = generate_grid(file);

    int sx = (plot_area.x1 - plot_area.x0) / (double) data.size();
    int sy = (plot_area.y1 - plot_area.y0) / (double) data.back();

    set_color(file, CHART_R, CHART_G, CHART_B);

    for (unsigned i = 0; i < data.size(); i++)
    {
        int x = plot_area.x0 + (i + 1)*sx;
        int y = plot_area.y0 + data[i]*sy;
        draw_point(file, POINT_SIZE, x, y);
    }
    file.close();
}

void generate_line_chart(vector<int> data)
{
    string filename = "line.txt";
    cout << "Saving chart in " << filename << "..." << endl;
    ofstream file(filename.c_str());

    Box plot_area = generate_grid(file);

    int sx = (plot_area.x1 - plot_area.x0) / (double) data.size();
    int sy = (plot_area.y1 - plot_area.y0) / (double) data.back();

    set_color(file, CHART_R, CHART_G, CHART_B);

    for (unsigned i = 0; i < data.size() - 1; i++)
    {
        int x0 = plot_area.x0 + (i + 1)*sx;
        int y0 = plot_area.y0 + data[i]*sy;
        int x1 = plot_area.x0 + (i + 2)*sx;
        int y1 = plot_area.y0 + data[i + 1]*sy;

        draw_line(file, LINE_THICK, x0, y0, x1, y1);
    }
    file.close();
}

void generate_area_chart(vector<int> data)
{
    string filename = "area.txt";
    cout << "Saving chart in " << filename << "..." << endl;
    ofstream file(filename.c_str());

    Box plot_area = generate_grid(file);

    int sx = (plot_area.x1 - plot_area.x0) / (double) data.size();
    int sy = (plot_area.y1 - plot_area.y0) / (double) data.back();

    set_color(file, CHART_R, CHART_G, CHART_B);

    for (unsigned i = 0; i < data.size() - 1; i++)
    {
        int x0 = plot_area.x0 + (i + 1)*sx;
        int y0 = plot_area.y0 + data[i]*sy;
        int x1 = plot_area.x0 + (i + 2)*sx;
        int y1 = plot_area.y0 + data[i + 1]*sy;

        draw_polygon(file, 4, x0, plot_area.y0, x0, y0, x1, y1, x1, plot_area.y0);
    }
    file.close();
}

vector<int> load_file(string filename)
{
    vector<int> data;
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Unable to open file: \"" << filename << "\"";
        exit(1);
    }
    string line;
    while(getline(file,line))
    {
        stringstream ss(line.c_str());
        while(!ss.eof())
        {
            int value;
            if (ss >> value)
                data.push_back(value);
        }
    }
    return data;
}

int main()
{
    cout << "Data file?" << endl;
    cout << ": ";
    string filename;
    getline(cin, filename);

    vector<int> data = load_file(filename);

    cout << "\nType of chart?" << endl;
    cout << "(C)olumn" << endl;
    cout << "(P)oint" << endl;
    cout << "(L)ine" << endl;
    cout << "(A)rea" << endl;
    cout << ": ";
    char option;
    cin >> option;
    switch (option)
    {
    case 'c': case 'C':
        generate_column_chart(data);
        break;
    case 'p': case 'P':
        generate_point_chart(data);
        break;
    case 'l': case 'L':
        generate_line_chart(data);
        break;
    case 'a': case 'A':
        generate_area_chart(data);
        break;
    default:
        cout << "Invalid chart type." << endl;
    }
}