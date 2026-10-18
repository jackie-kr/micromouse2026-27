#include <array>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
// #include "../Sensing/mazeMapper.h"
#include "API.h"
#include "graphList.h"

class MazeSolver {
    private:
        // store the maze in graph form for solving algorithms
        GraphList graphL;
        char colors[3] = {'r', 'b', 'g'};

    public:
        MazeSolver() {};

        // ENUM for direction placeholder
        // Who chose this placeholder values?? why is there no consistent pattern?
        enum DIRECTION {
            UP = 0b0100, // 4
            UP_RIGHT = 0b0101, // 5
            RIGHT = 0b0001, // 1
            DOWN_RIGHT = 0b1101, // 13
            DOWN = 0b1100, // 12
            DOWN_LEFT = 0b1111, // 15
            LEFT = 0b0011, // 3
            UP_LEFT = 0b0111 // 7
        };

        void createGraph(std::array<std::array<int, 16>, 17> horz,
            std::array<std::array<int, 17>, 16> vert);

        void print();

        // convert a vertex to coordinates (x, y)
        pair<int, int> convertVertToCoords(int vert);

        // get a direction to travel given a change in position
        DIRECTION getDirFromOffset(int dx, int dy);

        // checks if the current vert is in the center (goal)
        bool foundCenter(int vert);

        // uses BFS to find shortest path to center
        vector<DIRECTION> findCenterBFS();

        // reconstruct path from parent and find the directions from 0
        vector<DIRECTION> constructPath(vector<int>& parent, int start);
};
