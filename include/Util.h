#pragma once

#include <array>
#include <string>
#include <algorithm>
#include "API.h"

class Util {
    inline static const int range = 3;

    public:
    // To my knowledge there isn't a way to make it so that we can have this be just one argument for either horz or vert without it being more trouble than its worth?
    // There are definitely ways, but if we want a readable solution this is probably the best way. i think.
    static std::array<int, 2> wallFront(std::array<int, 2> position, int rotation, std::array<std::array<int, 16>, 17> horz, std::array<std::array<int, 17>, 16> vert) {
        // The amount of walls ahead of the mouse that can be detected. I wanted this to be a static const, but there was some weird cpp shenanigans happening when I tried, so its just here for now.
        // If whoever is reading this has a better way to do this, by all means please do.

        int max = range;
        switch (rotation) {
            case 0: // up
                max = std::min(range, 16 - position[1]);
                for (int i = 0; i < max; i++) {
                    if (horz[16 - (position[1] + 1 + i)][position[0]] == 1) return {position[0], position[1] + 1 + i};
                }
                break;
            case 1: // left
                max = std::min(range, 1 + position[0]);
                for (int i = 0; i < max; i++) {
                    if (vert[15 - position[1]][position[0] - i] == 1) return {position[0] - i, position[1]};
                }
                break;
            case 2: // down
                max = std::min(range, 1 + position[1]);
                for (int i = 0; i < max; i++) {
                    if (horz[16 - (position[1] - i)][position[0]] == 1) return {position[0], position[1] - i};
                }
                break;
            case 3: // right
                max = std::min(range, 16 - position[0]);
                for (int i = 0; i < max; i++) {
                    if (vert[15 - position[1]][position[0] + 1 + i] == 1) return {position[0] + 1 + i, position[1]};
                }
                break;
        }
        return {-1, -1};
    }

    static std::array<int, 2> wallLeft(std::array<int, 2> position, int rotation, std::array<std::array<int, 16>, 17> horz, std::array<std::array<int, 17>, 16> vert) {
        switch (rotation) {
            case 0:
                return wallFront(position, 1, horz, vert);
                break;
            case 1:
                return wallFront(position, 2, horz, vert);
                break;
            case 2:
                return wallFront(position, 3, horz, vert);
                break;
            case 3:
                return wallFront(position, 0, horz, vert);
                break;
        }
        return {-1, -1};
    }

    static std::array<int, 2> wallRight(std::array<int, 2> position, int rotation, std::array<std::array<int, 16>, 17> horz, std::array<std::array<int, 17>, 16> vert) {
        switch (rotation) {
            case 0:
                return wallFront(position, 3, horz, vert);
                break;
            case 1:
                return wallFront(position, 0, horz, vert);
                break;
            case 2:
                return wallFront(position, 1, horz, vert);
                break;
            case 3:
                return wallFront(position, 2, horz, vert);
                break;
        }
        return {-1, -1};
    }

    static int getDetectionRange() {
        return range;
    }
    // NOTE: these return in terms of {x, y}. In the arrays, we use the convention array[y][x]. These return them in x, y to make readability a bit easier. If needed later, this can be changed.
    static std::array<int, 2> getWallFront(std::array<int, 2> position, int rotation) {
        switch (rotation) {
            case 0: // up
                return {position[0], position[1] + 1}; // horz
                break;
            case 1: // left
                return position; // vert
                break;
            case 2: // down
                return position; // horz
                break;
            case 3: // right
                return {position[0] + 1, position[1]}; // vert
                break;
        }
        return{-1, -1};
    }
    static std::array<int, 2> getWallLeft(std::array<int, 2> position, int rotation) {
        switch (rotation) {
            case 0:
                return position; // vert
                break;
            case 1:
                return position; // horz
                break;
            case 2:
                return {position[0] + 1, position[1]}; // vert
                break;
            case 3:
                return {position[0], position[1] + 1}; // horz
                break;
        }
        return{-1, -1};
    }
    static std::array<int, 2> getWallBack(std::array<int, 2> position, int rotation) {
        switch (rotation) {
            case 0:
                return position; // horz
                break;
            case 1:
                return {position[0] + 1, position[1]}; // vert
                break;
            case 2:
                return {position[0], position[1] + 1}; // horz
                break;
            case 3:
                return position; // vert
                break;
        }
        return{-1, -1};
    }
    static std::array<int, 2> getWallRight(std::array<int, 2> position, int rotation) {
        switch (rotation) {
            case 0:
                return {position[0] + 1, position[1]}; // vert
                break;
            case 1:
                return {position[0], position[1] + 1}; // horz
                break;
            case 2:
                return position; // vert
                break;
            case 3:
                return position; // horz
                break;
        }
        return{-1, -1};
    }

    static std::string arrayToString(std::array<int, 2> array) {
        std::string result = "{";
        for (int i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
            result += std::to_string(array[i]);
            if(i < (sizeof(array) / sizeof(array[0]) - 1)) {
                result += ", ";   
            }
        }
        return result + "}";
    }

    static int getOppositeDirection(int rotation) {
        switch (rotation) {
            case 0:
                return 2;
            case 1:
                return 3;
            case 2:
                return 0;
            case 3:
                return 1;
        }
    }
    static int isAdjecentTile(std::array<int, 2> a, std::array<int, 2> b) {
        bool adjacentX = false;
        bool adjacentY = false;
        if (a[0] == b[0] + 1 || a[0] == b[0] - 1) adjacentX = true;
        if (a[1] == b[1] + 1 || a[1] == b[1] - 1) adjacentY = true;
        if(adjacentX ^ adjacentY) return true;
        return false;
    }

    static void log(const std::string& text) {
        std::cerr << text << std::endl;
    }
};