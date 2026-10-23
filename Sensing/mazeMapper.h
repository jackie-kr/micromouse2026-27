#include <array>
#include <iostream>
#include "../include/API.h"
#include "../include/Util.h"
#include "mazeGen.h"
#include <vector>

class MazeMapper {
    MazeGen gen;
    const int range = 3;

    std::array<std::array<int, 16>, 17> omniscientHorz = gen.getHorz();
    std::array<std::array<int, 17>, 16> omniscientVert = gen.getVert();

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

        int maxUp = std::min(range, 16 - curPosition[1]);
        int maxDown = std::min(range, 1 + curPosition[1]);
        for (int i = 0; i < maxUp; i++) {
            if (omniscientHorz[16 - (curPosition[1] + 1 + i)][curPosition[0]] == 1) {
                horz[curPosition[1] + 1 + i][curPosition[0]] = 1;
                break;
            }
            else horz[curPosition[1] + 1 + i][curPosition[0]] = 0;
        }
        for (int i = 0; i < maxDown; i++) {
            if (omniscientHorz[16 - (curPosition[1] - i)][curPosition[0]] == 1) {
                horz[curPosition[1] - i][curPosition[0]] = 1;
                break;
            }
            else horz[curPosition[1] - i][curPosition[0]] = 0;
        }
        return horz;
    }

    std::array<std::array<int, 17>, 16> buildVertical(std::array<int, 2> curPosition, int rotation) {
        int maxLeft = std::min(range, 1 + curPosition[0]);
        for (int i = 0; i < maxLeft; i++) {
            if (omniscientVert[15 - curPosition[1]][curPosition[0] - i] == 1){
                vert[curPosition[1]][curPosition[0]-i] = 1;
                break;
            }
            else vert[curPosition[1]][curPosition[0]-1] = 0;
        }
        int maxRight = std::min(range, 16 - curPosition[0]);
        for (int i = 0; i < maxRight; i++) {
            if (omniscientVert[15 - curPosition[1]][curPosition[0] + 1 + i] == 1){
                vert[curPosition[1]][curPosition[0] + 1 + i] = 1;
                break;
            }
            else vert[curPosition[1]][curPosition[0] + 1 + i] = 0;
        }
        return vert;
    }

    // Old implementation for buildHorizontal that can only detect adjacent walls. Use if the map does not reflect the value in mazeGen.h
    std::array<std::array<int, 16>, 17> buildHorizontalOld(std::array<int, 2> curPosition, int rotation) {
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
                //down
                if (API::wallFront()) horz[curPosition[1]][curPosition[0]] = 1;
                else horz[curPosition[1]][curPosition[0]] = 0;
                break;
            case 3:
                //right
                if (API::wallLeft()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                else horz[curPosition[1] + 1][curPosition[0]] = 0;
                if (API::wallRight()) horz[curPosition[1]][curPosition[0]] = 1;
                else horz[curPosition[1]][curPosition[0]] = 0;
                break;
        }
        return horz;
    }

    // Old implementation for buildVertical that can only detect adjacent walls. Use if the map does not reflect the value in mazeGen.h
    std::array<std::array<int, 17>, 16> buildVerticalOld(std::array<int, 2> curPosition, int rotation) {
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
                // down
                if (API::wallLeft()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                else vert[curPosition[1]][curPosition[0] + 1] = 0;
                if (API::wallRight()) vert[curPosition[1]][curPosition[0]] = 1;
                else vert[curPosition[1]][curPosition[0]] = 0;
                break;
            case 3:
                // right
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
                completed = vert[i][j] != 2 && completed;
                completed = horz[j][i] != 2 && completed;

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
