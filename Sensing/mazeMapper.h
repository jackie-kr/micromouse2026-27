#include <array>
#include <iostream>
#include "../include/API.h"
#include <vector>

class MazeMapper {

    // dont ask me why we need to write these things out in full. i dont know
    // but whenever we dont, random walls appear. its scary. im scared.
    std::array<std::array<int, 16>, 17> horz = {{
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    }};;
    std::array<std::array<int, 17>, 16> vert {{
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    }};

    public:
    MazeMapper() {};

    std::array<std::array<int, 16>, 17> buildHorizontal(std::array<int, 2> curPosition, int rotation) {
        switch (rotation) {
            case 0:
                //up
                if (API::wallFront()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                else horz[curPosition[1] + 1][curPosition[0]] = 0;
                break;
            case 1:
                // left
                if (API::wallLeft()) horz[curPosition[1]][curPosition[0]] = 1;
                else horz[curPosition[1]][curPosition[0]] = 0;
                if (API::wallRight()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                else horz[curPosition[1] + 1][curPosition[0]] = 0;
                break;
            case 2:
                if (API::wallFront()) horz[curPosition[1]][curPosition[0]] = 1;
                else horz[curPosition[1]][curPosition[0]] = 0;
                break;
            case 3:
                if (API::wallLeft()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                else horz[curPosition[1] + 1][curPosition[0]] = 0;
                if (API::wallRight()) horz[curPosition[1]][curPosition[0]] = 1;
                else horz[curPosition[1]][curPosition[0]] = 0;
                break;
        }
        return horz;
    }

    std::array<std::array<int, 17>, 16> buildVertical(std::array<int, 2> curPosition, int rotation) {
        switch (rotation) {
            case 0:
                //up
                if (API::wallLeft()) vert[curPosition[1]][curPosition[0]] = 1;
                else vert[curPosition[1]][curPosition[0]] = 0;
                if (API::wallRight()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                else vert[curPosition[1]][curPosition[0] + 1] = 0;
                break;
            case 1:
                // left
                if (API::wallFront()) vert[curPosition[1]][curPosition[0]] = 1;
                else vert[curPosition[1]][curPosition[0]] = 0;
                break;
            case 2:
                if (API::wallLeft()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                else vert[curPosition[1]][curPosition[0] + 1] = 0;
                if (API::wallRight()) vert[curPosition[1]][curPosition[0]] = 1;
                else vert[curPosition[1]][curPosition[0]] = 0;
                break;
            case 3:
                if (API::wallFront()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                else vert[curPosition[1]][curPosition[0] + 1] = 0;
                break;
        }
        return vert;
    }

    bool mapCompleted() {
        bool completed = true;

        // check for unmapped walls
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 17; j++) {
                completed = vert[j][i] != 2 && completed;
                completed = horz[i][j] != 2 && completed;

                if (!completed) return false;
            }
        }

        return completed;
    }

    std::array<std::array<int, 16>, 17> getHorizontal() {
        return horz;
    }
    std::array<std::array<int, 17>, 16> getVertical() {
        return vert;
    }

    std::vector<std::pair<int, int>> getUnexplored(std::array<std::array<int, 16>, 16> manhattanDistances) {
        // Find the best unknown horizontal wall
        int distance = 255;
        bool horizontal = false;
        std::pair<int, int> bestHorzPair = {-1, -1};
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 16; j++) {
                // get the two potential places we can check this wall
                if (horz[i][j] == 2 && manhattanDistances[i][j] < distance) {
                    bestHorzPair = {i, j};
                    distance = manhattanDistances[i][j];
                    horizontal = true;
                }
            }
        }

        // check for best vertical pair
        distance = 255;
        std::pair<int, int> bestVertPair = {-1, -1};
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 17; j++) {
                // get the two potential places we can check this wall
                if (vert[i][j] == 2 && manhattanDistances[i][j] < distance) {
                    bestVertPair = {i, j};
                    distance = manhattanDistances[i][j];
                    horizontal = false;
                }
            }
        }

        std::vector<std::pair<int, int>> result;
        if (bestHorzPair.first != -1) {
            result.push_back({bestHorzPair.first, bestHorzPair.second}); // Add the best horizontal pairs
            result.push_back({bestHorzPair.first - 1, bestHorzPair.second});
        }

        if (bestVertPair.first != -1) {
            result.push_back({bestVertPair.first, bestVertPair.second}); // Add the best vertical pairs
            result.push_back({bestVertPair.first, bestVertPair.second - 1});
        }
        return result;
    }
};
