#include <iostream>
#include <string>
#include <cmath>
#include "include/API.h"
#include "include/Util.h"
#include "include/mazeSolver.h"
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
    MazeSolver solver;
    bool completedMap = false;

    log_position(curPosition);

    mapper.buildHorizontal(curPosition, rotation);
    mapper.buildVertical(curPosition, rotation);

    while (!completedMap) {
        // Update maze visually to show mapped walls
        if (API::wallFront()) {
            char wallDir = 'n';
            switch (rotation) {
                case 0:
                    wallDir = 'n';
                    break;
                case 1:
                    wallDir = 'w';
                    break;
                case 2:
                    wallDir = 's';
                    break;
                case 3:
                    wallDir = 'e';
                    break;
            }
            API::setWall(curPosition[0], curPosition[1], wallDir);
        }
        if (API::wallLeft()) {
            char wallDir = 'n';
            switch ((rotation + 1) % 4) {
                case 0:
                    wallDir = 'n';
                    break;
                case 1:
                    wallDir = 'w';
                    break;
                case 2:
                    wallDir = 's';
                    break;
                case 3:
                    wallDir = 'e';
                    break;
            }
            API::setWall(curPosition[0], curPosition[1], wallDir);
        }
        if (API::wallRight()) {
            char wallDir = 'n';
            switch ((rotation + 3) % 4) {
                case 0:
                    wallDir = 'n';
                    break;
                case 1:
                    wallDir = 'w';
                    break;
                case 2:
                    wallDir = 's';
                    break;
                case 3:
                    wallDir = 'e';
                    break;
            }
            API::setWall(curPosition[0], curPosition[1], wallDir);
        }

        log("---------------------");
        log_position(curPosition);
        int corridors = 0;
        if (!API::wallFront()) corridors += 1;
        if (!API::wallLeft()) corridors += 1;
        if (!API::wallRight()) corridors += 1;
        if (corridors > 1) {
            std::array<int, 2> backWall = Util::getWallBack(curPosition, rotation);
            std::array<int, 2> frontWall = Util::getWallFront(curPosition, rotation);
            std::array<int, 2> leftWall = Util::getWallLeft(curPosition, rotation);
            std::array<int, 2> rightWall = Util::getWallRight(curPosition, rotation);

            int backVal;
            int frontVal;
            int rightVal;
            int leftVal;
            if (rotation % 2 == 0) {
                backVal = tremaux.returnHorz()[backWall[1]][backWall[0]];
                frontVal = tremaux.returnHorz()[frontWall[1]][frontWall[0]];
            }
            else {
                backVal = tremaux.returnVert()[backWall[1]][backWall[0]];
                frontVal = tremaux.returnVert()[frontWall[1]][frontWall[0]];
            }

            if (rotation % 2 == 0) {
                rightVal = tremaux.returnVert()[rightWall[1]][rightWall[0]];
                leftVal = tremaux.returnVert()[leftWall[1]][leftWall[0]];
            }
            else {
                rightVal = tremaux.returnHorz()[rightWall[1]][rightWall[0]];
                leftVal = tremaux.returnHorz()[leftWall[1]][leftWall[0]];
            }

            log("Tremaux Last Tile: " + Util::arrayToString(tremaux.returnLastTile()));
            log("Tremaux Last Choice: " + Util::arrayToString(tremaux.returnLastChoice()));
            log("Tremaux Last Choice Was Horz: " + std::to_string(tremaux.returnLastChoiceIsHorz()));
            int result = tremaux.tremauxCheck(curPosition, rotation);
            //log("Tremaux result: " + std::to_string(result));

            log("Back Path: " + Util::arrayToString(backWall));
            if (rotation % 2 == 0) log("\tBack Path Value: " + std::to_string(backVal) + " -> " + std::to_string(tremaux.returnHorz()[backWall[1]][backWall[0]]));
            else log("\tBack Path Value: " + std::to_string(backVal) + " -> " + std::to_string(tremaux.returnVert()[backWall[1]][backWall[0]]));

            if (!API::wallFront()) {
                log("Front Path: " + Util::arrayToString(frontWall));
                if (rotation % 2 == 0) log("\tFront Path Value: " + std::to_string(frontVal) + " -> " + std::to_string(tremaux.returnHorz()[frontWall[1]][frontWall[0]]));
                else log("\tFront Path Value: " + std::to_string(backVal) + " -> " +  std::to_string(tremaux.returnVert()[frontWall[1]][frontWall[0]]));
            }
            if (!API::wallLeft()) {
                log("Left Path: " + Util::arrayToString(leftWall));
                if (rotation % 2 == 0) log("\tLeft Path Value: " + std::to_string(leftVal) + " -> " + std::to_string(tremaux.returnVert()[leftWall[1]][leftWall[0]]));
                else log("\tLeft Path Value: " + std::to_string(backVal) + " -> " +  std::to_string(tremaux.returnHorz()[leftWall[1]][leftWall[0]]));
            }
            if (!API::wallRight()) {
                log("Right Path: " + Util::arrayToString(rightWall));
                if (rotation % 2 == 0) log("\tRight Path Value: " + std::to_string(rightVal) + " -> " + std::to_string(tremaux.returnVert()[rightWall[1]][rightWall[0]]));
                else log("\tRight Path Value: " + std::to_string(backVal) + " -> " +  std::to_string(tremaux.returnHorz()[rightWall[1]][rightWall[0]]));
            }

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

        completedMap = mapper.mapCompleted();

        //log("MAZE SENSING:");
        log_maze(horz, vert);
    }

    auto horz = mapper.buildHorizontal(curPosition, rotation);
    auto vert = mapper.buildVertical(curPosition, rotation);
    log_maze(horz, vert);

    solver.createGraph(horz, vert);
    solver.print();

    vector<MazeSolver::DIRECTION> dirPath = solver.findCenterBFS();

    // Move the mouse to follow the path found by BFS
    // 0 = up
    // 1 = left
    // 2 = down
    // 3 = right
    int orientation = 0; // starting facing up {

    std::cerr << "Starting to solve the maze" << std::endl;

    for (const auto& dir : dirPath) {
        std::cerr << dir << '\n';
        int actual_dir = 0;
        // map enum to compass directoin
        switch (dir) {
            case MazeSolver::UP:
                actual_dir = 0;
                break;
            case MazeSolver::LEFT:
                actual_dir = 1;
                break;
            case MazeSolver::DOWN:
                actual_dir = 2;
                break;
            case MazeSolver::RIGHT:
                actual_dir = 3;
                break;
            default:
                actual_dir = 0;
        }

        // check if we need to turn at all
        if (actual_dir != orientation) {
            std::cerr << "We need to turn!" << std::endl;

            if (actual_dir == (orientation + 1) % 4) {
                // we need to turn left
                API::turnLeft();
                orientation++;
            }

            if (actual_dir == (orientation + 3) % 4) {
                // Turn right
                API::turnRight();
                orientation--;
            }

            if  (orientation < 0) {
                orientation = 3;
            }

            if  (orientation > 3) {
                orientation = 0;
            }
        }

        if (!API::wallFront()) {
            API::moveForward(1); // move forward one cell
        } else {
            std::cerr << "Hey we're gonna hit a wall qwq" << std::endl;
        }
    }
}
