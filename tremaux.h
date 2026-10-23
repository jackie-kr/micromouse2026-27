#pragma once

#include <array>
#include "include/API.h"
#include "include/Util.h"

class Tremaux {
    private:
    std::array<std::array<int, 16>, 17> horz = {{
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
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    }};;
    std::array<std::array<int, 17>, 16> vert {{
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    }};

    // If the last choice is the back wall, dont add to it.
    std::array<int, 2> lastChoice {-1, -1};
    bool lastChoiceIsHorz = false; // not the most elegant solution, but it should work to distinguish between horizontal and vertical choices
    std::array<int, 2> lastTile {-1, -1};

    public:
    Tremaux() {};

    // returns an int based off of which direction to go
    // 0 = forward
    // 1 = left
    // 2 = down (i dont think this should happen?)
    // 3 = right
    int tremauxCheck(std::array<int, 2> position, int rotation){
        int backVal = 0;
        int frontVal = 0;
        int leftVal = 0;
        int rightVal = 0;
        std::array<int, 2> backWall = Util::getWallBack(position, rotation);
        std::array<int, 2> frontWall = Util::getWallFront(position, rotation);
        std::array<int, 2> leftWall = Util::getWallLeft(position, rotation);
        std::array<int, 2> rightWall = Util::getWallRight(position, rotation);

        if (rotation % 2 == 0) {
            if ((lastChoice != backWall) || ((lastChoice == backWall) && !lastChoiceIsHorz) || (position == lastTile)) horz[backWall[1]][backWall[0]] += 1;
            else Util::log("Not adding to tile behind!");
            backVal = horz[backWall[1]][backWall[0]];
            frontVal = horz[frontWall[1]][frontWall[0]];
            leftVal = vert[leftWall[1]][leftWall[0]];
            rightVal = vert[rightWall[1]][rightWall[0]];
        }
        else {
            if (((lastChoice != backWall) || ((lastChoice == backWall) && lastChoiceIsHorz)) || (position == lastTile)) vert[backWall[1]][backWall[0]] += 1;
            else Util::log("Not adding to tile behind!");
            backVal = vert[backWall[1]][backWall[0]];
            frontVal = vert[frontWall[1]][frontWall[0]];
            leftVal = horz[leftWall[1]][leftWall[0]];
            rightVal = horz[rightWall[1]][rightWall[0]];
        }

        lastTile = position;

        if (API::wallFront()) frontVal = 2;
        if (API::wallLeft()) leftVal = 2;
        if (API::wallRight()) rightVal = 2;

        if (((frontVal == 1 || leftVal == 1 || rightVal == 1) && backVal < 2) || (frontVal > 1 && rightVal > 1 && leftVal > 1)) { // highest priority, if there is a 1 option and the back is not 2, then go back!! alternatively, if its a dead end, go back.
            if (rotation % 2 == 0) {
                horz[backWall[1]][backWall[0]] += 1;
                lastChoiceIsHorz = true;
            }
            else {
                vert[backWall[1]][backWall[0]] += 1;
                lastChoiceIsHorz = false;
            }
            lastChoice = std::array<int, 2> {-1, -1};

            return 2;
        }
        else if (!API::wallFront() && frontVal <= leftVal && frontVal <= rightVal){ // second highest priority, go forward if the front value is the lowest option
            if (rotation % 2 == 0) {
                horz[frontWall[1]][frontWall[0]] += 1;
                lastChoiceIsHorz = true;
            }
            else {
                vert[frontWall[1]][frontWall[0]] += 1;
                lastChoiceIsHorz = false;
            }
            lastChoice = frontWall;

            return 0;
        }
        else if (!API::wallRight() && rightVal < frontVal && rightVal <= leftVal){
            if (rotation % 2 == 0) {
                vert[rightWall[1]][rightWall[0]] += 1;
                lastChoiceIsHorz = false;
            }
            else {
                horz[rightWall[1]][rightWall[0]] += 1;
                lastChoiceIsHorz = true;
            }
            lastChoice = rightWall;

            return 3;
        }
        else {
            if (rotation % 2 == 0) {
                vert[leftWall[1]][leftWall[0]] += 1;
                lastChoiceIsHorz = false;
            }
            else {
                horz[leftWall[1]][leftWall[0]] += 1;
                lastChoiceIsHorz = true;
            }
            lastChoice = leftWall;

            return 1;
        }
        return 2;
    }
    
    std::array<std::array<int, 16>, 17> returnHorz() {
        return horz;
    }
    std::array<std::array<int, 17>, 16> returnVert() {
        return vert;
    }
    std::array<int, 2> returnLastTile() {
        return lastTile;
    }
    std::array<int, 2> returnLastChoice() {
        return lastChoice;
    }
    bool returnLastChoiceIsHorz() {
        return lastChoiceIsHorz;
    }
};