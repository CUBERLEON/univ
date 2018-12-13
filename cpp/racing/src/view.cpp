#define PDC_WIDE
#include <curses.h>

#include <json/json.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>
#include "world.h"
#include "utils.h"

using json=nlohmann::json;

void clean_scr(WINDOW* win = stdscr);
void mvwaddellipse(WINDOW* win, unsigned y, unsigned x, char c, int ry, int rx);

int main(int argc, char** argv) {
    try {
        if (argc < 2)
            throw std::invalid_argument("There are no passed arguments!");

        std::string text;
            
        std::fstream in;
        in.open(argv[1], std::fstream::in);
        std::stringstream buf;
        buf << in.rdbuf();
        text = buf.str();
        in.close();
        
        json data = json::parse(text);
        unsigned int ticksCnt = data["states"].size();
        if (ticksCnt < 1)
            throw std::invalid_argument("List of states cannot be empty!");
        unsigned int cockroachesCnt = data["states"][0].size();
        std::vector< std::vector<State> > states(ticksCnt);
        for (unsigned int i = 0; i < ticksCnt; ++i) {
            if (data["states"][i].size() != cockroachesCnt)
                throw std::invalid_argument("List of states contains corrupted data!");
            for (unsigned int j = 0; j < cockroachesCnt; ++j)
                states[i].push_back(State(data["states"][i][j]));
        }
        
        World world(data["world"]);

        initscr();
        if (has_colors() == FALSE) {
            endwin();
            throw std::runtime_error("Your terminal does not support color!");
        }
        start_color();
        raw();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0);
        srand(time(0));
        
        unsigned TPS = 20;
        unsigned TRACK_X = 140, TRACK_Y = unsigned(world.getWidth() / world.getLength() * TRACK_X);
        unsigned MAX_X = TRACK_X, MAX_Y = TRACK_Y + 2;
        
        resize_term(MAX_Y, MAX_X);
        
        WINDOW *track = newwin(TRACK_Y, TRACK_X, 2, 0);
        box(track, ' ' , ' ');
        wrefresh(track);
        
        float x_factor = (TRACK_X-1) / world.getLength();
        float y_factor = (TRACK_Y-1) / world.getWidth();
        
        int color_offset = 100;
        int pair_offset = 10;
        for (unsigned i = 0; i < cockroachesCnt; ++i) {
            init_color(color_offset + i, rand()%807 + 150, rand()%807 + 150, rand()%807 + 150);
            init_pair(pair_offset + i, color_offset + i, COLOR_BLACK);
        }
        init_color(color_offset + cockroachesCnt, 150, 150, 150);
        init_pair(pair_offset + cockroachesCnt, color_offset + cockroachesCnt, COLOR_BLACK);
        
        for (unsigned tick = 0; tick < ticksCnt; ++tick) {
            clean_scr();
            clean_scr(track);
            
            mvprintw(0, 0, "Tick: %d/%d, Cockroaches: %d", tick+1, ticksCnt, cockroachesCnt);
            move(1, 0);
            for (unsigned i = 0; i < cockroachesCnt; ++i) {
                attron(COLOR_PAIR(pair_offset + i));
                printw("#%d('%s'):speed(%.1f, %.1f) ", i+1, world.getCockroaches()[i].getName().c_str(), states[tick][i].getSpeed().first, states[tick][i].getSpeed().second);
                attroff(COLOR_PAIR(pair_offset + i));
            }

            wattron(track, COLOR_PAIR(pair_offset + cockroachesCnt));
            for (unsigned i = 0; i < world.getObstacles().size(); ++i) {
                mvwaddellipse(track, world.getObstacles()[i].getPosY() * y_factor, world.getObstacles()[i].getPosX() * x_factor,
                '#', world.getObstacles()[i].getRadius() * y_factor, world.getObstacles()[i].getRadius() * x_factor);
            }
            for (unsigned i = 0; i < world.getObstacles().size(); ++i) {
                mvwaddch(track, world.getObstacles()[i].getPosY() * y_factor, world.getObstacles()[i].getPosX() * x_factor,
                'O');
            }
            wattroff(track, COLOR_PAIR(pair_offset + cockroachesCnt));
            
            for (unsigned i = 0; i < cockroachesCnt; ++i) {
                wattron(track, COLOR_PAIR(pair_offset + i));
                mvwaddch(track, states[tick][i].getPos().second * y_factor, states[tick][i].getPos().first * x_factor,
                'X');
                wattroff(track, COLOR_PAIR(pair_offset + i));
            }
            
            refresh();
            wrefresh(track);
            napms(1000/TPS);
        }
        
        getch();
        endwin();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void clean_scr(WINDOW* win) {
    unsigned int MAX_X, MAX_Y;
    getmaxyx(stdscr, MAX_Y, MAX_X);
    for (unsigned int i = 0; i < MAX_X; ++i)
        for (unsigned int j = 0; j < MAX_Y; ++j)
            mvwaddch(win, j, i, ' ');
}

void mvwaddellipse(WINDOW* win, unsigned y, unsigned x, char c, int ry, int rx) {
    for (int i = -ry+1; i < ry; ++i)
        for (int j = -rx+1; j < rx; ++j)
            if (sqr(i)*sqr(rx-1)+sqr(j)*sqr(ry-1) <= sqr(rx-1)*sqr(ry-1))
                mvwaddch(win, i+y, j+x, c);
}