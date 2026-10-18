#include <iostream>
#include <string>
#include <cmath>
#include "include/API.h"
#include "include/Util.h"
#include "Sensing/arrayTranslator.h"
#include "Sensing/mazeMapper.h"
#include "tremaux.h"

//x, y
std::array<int, 2> curPosition {0, 0};

// 0 = forward
// 1 = left
// 2 = down
// 3 = right
int rotation = 0;

bool inCenter = false;

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

void log_position(std::array<int, 2> pos) {
    std::string posResult = "curPosition: <";
    for (int i = 0; i < sizeof(pos) / sizeof(pos[0]); ++i) {
        posResult += std::to_string(pos[i]);

        if (i < sizeof(pos) / sizeof(pos[0]) - 1) posResult += ", ";
    }
    posResult += ">";
    log(posResult);
}

void log_maze(std::array<std::array<int, 16>, 17> horz, std::array<std::array<int, 17>, 16> vert) {
        // Flipping the arrays to make them readable for the translator.
        // TODO: 
        std::array<std::array<int, 16>, 17> flippedHorz;
        int horzSize = sizeof(flippedHorz) / sizeof(flippedHorz[0]);
        for (int i = 0; i < horzSize; ++i) {
            flippedHorz[i] = horz[horzSize - 1 - i];
        }

        std::array<std::array<int, 17>, 16> flippedVert;
        int vertSize = sizeof(flippedVert) / sizeof(flippedVert[0]);
        for (int i = 0; i < vertSize; ++i) {
            flippedVert[i] = vert[vertSize - 1 - i];
        }



        arrayTranslator converter(flippedHorz, flippedVert);

        std::string mazeResult = "";
        auto maze = converter.translate();
        for (int i = 0; i < 33; i++) {
            for (int j = 0; j < 65; j++) {
                mazeResult += maze[i][j];
            }
            mazeResult += ("\n");
        }
        log(mazeResult);
}

int turnRight() {
    API::turnRight();
    rotation -= 1;
    if  (rotation < 0) {
        rotation = 3;
    }
    return rotation;
}
int turnLeft() {
    API::turnLeft();
    rotation += 1;
    if (rotation > 3) {
        rotation = 0;
    }
    return rotation;
}

int main(int argc, char* argv[]) {
    MazeMapper mapper;
    Tremaux tremaux;

    log_position(curPosition);
    
    mapper.buildHorizontal(curPosition, rotation);
    mapper.buildVertical(curPosition, rotation);

    while (true) {
        int corridors = 0;
        if (!API::wallFront()) corridors += 1;
        if (!API::wallLeft()) corridors += 1;
        if (!API::wallRight()) corridors += 1;
        if (corridors > 1) {
            log("DOIN TREMAUX!!!");

            std::array<int, 2> backWall = Util::getWallBack(curPosition, rotation);
            std::array<int, 2> frontWall = Util::getWallFront(curPosition, rotation);
            std::array<int, 2> leftWall = Util::getWallLeft(curPosition, rotation);
            std::array<int, 2> rightWall = Util::getWallRight(curPosition, rotation);

            log("Back Path: " + Util::arrayToString(backWall));
            if (rotation % 2 == 0) log("\tBack Path Value: " + std::to_string(tremaux.returnHorz()[backWall[1]][backWall[0]]));
            else log("\tBack Path Value: " + std::to_string(tremaux.returnVert()[backWall[1]][backWall[0]]));
            
            if (!API::wallFront()) {
                log("Front Path: " + Util::arrayToString(frontWall));
                if (rotation % 2 == 0) log("\tFront Path Value: " + std::to_string(tremaux.returnHorz()[frontWall[1]][frontWall[0]]));
                else log("\tFront Path Value: " + std::to_string(tremaux.returnVert()[frontWall[1]][frontWall[0]]));
            }
            if (!API::wallLeft()) {
                log("Left Path: " + Util::arrayToString(leftWall));
                if (rotation % 2 == 0) log("\tLeft Path Value: " + std::to_string(tremaux.returnVert()[leftWall[1]][leftWall[0]]));
                else log("\tLeft Path Value: " + std::to_string(tremaux.returnHorz()[leftWall[1]][leftWall[0]]));
            }
            if (!API::wallRight()) {
                log("Right Path: " + Util::arrayToString(rightWall));
                if (rotation % 2 == 0) log("\tRight Path Value: " + std::to_string(tremaux.returnVert()[rightWall[1]][rightWall[0]]));
                else log("\tRight Path Value: " + std::to_string(tremaux.returnHorz()[rightWall[1]][rightWall[0]]));
            }

            
            int result = tremaux.tremauxCheck(curPosition, rotation);
            log("Tremaux result: " + std::to_string(result));
            log("TREMAUX SENSING");
            switch (result) {
                case 0:
                    API::moveForward();
                    break;
                case 1:
                    turnLeft();
                    API::moveForward();
                    break;
                case 2:
                    turnLeft();
                    turnLeft();
                    API::moveForward();
                    break;
                case 3:
                    turnRight();
                    API::moveForward();
                    break;
            }
        }
        else {
            if (API::wallFront()) {
                if (!API::wallLeft()) turnLeft();
                else if (!API::wallRight()) turnRight();
                else{
                    turnLeft();
                    turnLeft();
                }
            }
            API::moveForward();
        }
        switch (rotation)
        {
            case 0: // up
                curPosition[1] += 1;
                break;
            case 1: // left
                curPosition[0] -= 1;
                break;
            case 2: // down
                curPosition[1] -= 1;
                break;
            case 3: // right
                curPosition[0] += 1;
                break;
        }

        if (curPosition[0] == std::ceil(API::mazeWidth() / 2) && curPosition[1] == std::ceil(API::mazeHeight() / 2)) inCenter = true;

        auto horz = mapper.buildHorizontal(curPosition, rotation);
        auto vert = mapper.buildVertical(curPosition, rotation);

        log("MAZE SENSING:");
        log_maze(horz, vert);
        log_position(curPosition);
    }
}
