#include <array>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include "API.h"
#include "graphMatrix.h"
#include "graphList.h"

class MazeSolver {
    private:
        // store the maze in graph form for solving algorithms
        GraphMatrix graphM{256};
        GraphList graphL;

    public:
        MazeSolver() {};

        // ENUM for direction placeholder
        enum DIRECTION {
            UP,
            DOWN,
            LEFT,
            RIGHT
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
                        std::cout << "Coordinate: (" << x << ", " << y << "), node: " << x + (y * 16) << std::endl;
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
        }

        // reconstruct path from parent and find the directions from 0
        vector<DIRECTION> constructPath(vector<int>& parent, int start) {
            vector<int> path;
            vector<DIRECTION> directions;
            for (int current = start; current >= 0; current = parent[current])
                path.push_back(current);
            reverse(path.begin(), path.end());
            for (const auto& vert : path) {
                // TODO: convert vert path to directions from start (vert = 0)
                
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
    solver.print();
}
