#include <array>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include "include/API.h"
#include "include/graphMatrix.h"
#include "include/graphList.h"

class MazeSolver {
    private:
        // store the maze in graph form for solving algorithms
        GraphMatrix graphM{256};
        GraphList graphL;

    public:
        MazeSolver() {};

        // ENUM for direction placeholder
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
            std::array<std::array<int, 17>, 16> vert) {
                size_t vert_size = vert.size();
                size_t horz_size = horz.size();

                // loop over each node of the graph and check for walls
                for (int y = 0; y < 16; y++) {
                    for (int x = 0; x < 16; x++) {
                        // only need to check to the bottom and the right
                        // because of bi-directional graphs

                        // if not at the rightmost edge
                        if (x != 15) {
                            if (vert[y][x + 1] == 0) {
                                // std::cout << "Adding edge between: (" << x << ", " << y << ") and (" << x << ", " << y+1 << ")." << std::endl;
                                // graphM.add_edge(x + (y * 16), (x + (y * 16)) + 1);
                                graphL.add_edge(x + (y * 16), (x + (y * 16)) + 1);
                            }
                        }

                        // if not at the bottom edge
                        if (y != 15) {
                            if (horz[y + 1][x] == 0) {
                                // std::cout << "Adding edge between: (" << x << ", " << y << ") and (" << x << ", " << y+1 << ")." << std::endl;
                                // graphM.add_edge(x + (y * 16), (x + (y * 16)) + 1);
                                graphL.add_edge(x + (y * 16), (x + ((y + 1) * 16)));
                            }
                        }

                        // check for proper coordinate mess
                        // std::cout << "Coordinate: (" << x << ", " << y << "), node: " << x + (y * 16) << std::endl;
                    }
                }
            }

        void print() {
            // graphM.print();
            graphL.print();
        }

        // convert a vertex to coordinates (x, y)
        pair<int, int> convertVertToCoords(int vert) {
            return {vert % 16, vert / 16};
        }

        // get a direction to travel given a change in position
        DIRECTION getDirFromOffset(int dx, int dy) {
            // convert 2 ints to 1 using its bits (corresponding directions are already set up correctly)
            return (DIRECTION) ((dx & 0b11) | (((dy) & 0b11) << 2));
        }

        // checks if the current vert is in the center (goal)
        bool foundCenter(int vert) {
            auto coords = convertVertToCoords(vert);
            return (coords.first == 7 || coords.first == 8) && (coords.second == 7 || coords.second == 8);
        }

        // uses BFS to find shortest path to center
        vector<DIRECTION> findCenterBFS() {
            // setup needed ds, parent is used to reconstruct path
            unordered_map<int, vector<int>>& adjList = graphL.getList();
            vector<unsigned int> seen(adjList.size(), false);
            vector<int> parent(adjList.size(), -1);
            queue<int> q;
            seen[0] = true;
            q.push(0);
            while (!q.empty()) {
                int current = q.front();
                q.pop();
                // end early if found the center
                if (foundCenter(current)) {
                    return constructPath(parent, current);
                }
                for (const auto& neighbor : adjList.at(current)) {
                    if (!seen[neighbor]) {
                        seen[neighbor] = true;
                        parent[neighbor] = current;
                        q.push(neighbor);
                    }
                }
            }
            return constructPath(parent, 0); // wasn't found
        }

        // reconstruct path from parent and find the directions from 0
        vector<DIRECTION> constructPath(vector<int>& parent, int start) {
            vector<int> path;
            vector<DIRECTION> directions;
            for (int current = start; current >= 0; current = parent[current])
                path.push_back(current);
            reverse(path.begin(), path.end());
            pair<int, int> prevCoords = {0, 0};
            for (int i = 1; i < path.size(); i++) {
                pair<int, int> coords = convertVertToCoords(path.at(i));
                directions.push_back(getDirFromOffset(coords.first - prevCoords.first, coords.second - prevCoords.second));
                prevCoords = coords;
            }
            return directions;
        }
};

int main() {
    MazeSolver solver;

    std::array<std::array<int, 16>, 17> horz {{
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    }};

    std::array<std::array<int, 17>, 16> vert {{
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    }};

    solver.createGraph(horz, vert);
    // solver.print();
    vector<MazeSolver::DIRECTION> dirPath = solver.findCenterBFS();
    for (const auto& dir : dirPath) {
        cout << dir << '\n';
    }
}
