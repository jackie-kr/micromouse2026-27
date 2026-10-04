#include <array>
#include <vector>
#include <iostream>
#include <queue>
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

        bool foundCenter(int vert) {
            int x = vert % 16;
            int y = vert / 16;
            return (x == 7 || x == 8) && (y == 7 || y == 8);
        }

        vector<DIRECTION> findCenterBFS() {
            unordered_map<int, vector<int>>& adjList = graphL.getList();
            vector<unsigned int> seen(adjList.size(), false);
            vector<int> parent(adjList.size(), -1);
            queue<int> q;
            seen[0] = true;
            q.push(0);
            while (!q.empty()) {
                int current = q.front();
                q.pop();
                if (foundCenter(current)) {
                    return constructPath(parent);
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

        vector<DIRECTION> constructPath(vector<int>& parent) {
            
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
