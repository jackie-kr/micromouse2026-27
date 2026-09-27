#include <iostream>
#include <string>
#include <cmath>
#include "arrayTranslator.h"
#include "mazeMapper.h"

#include "API.h"


//x, y
std::array<int, 2> curPosition {0, 0};

std::array<std::array<int, 16>, 17> horz;
std::array<std::array<int, 17>, 16> vert;

// 0 = forward
// 1 = left
// 2 = down
// 3 = right
int rotation = 0;

bool inCenter = false;

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    MazeMapper mapper;

    while (!inCenter) {
        if (!API::wallRight()) {
            API::turnRight();
            rotation -= 1;
            if  (rotation < 0) {
                rotation = 3;
            }
        }
        if (!API::wallRight()) {

        }
        while (API::wallFront()) {
            API::turnLeft();
            rotation += 1;
            if  (rotation > 3) {
                rotation = 0;
            }
        }

        API::moveForward(1);
        
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

        auto horz = mapper.buildHorizontal(rotation, curPosition);
        auto vert = mapper.buildVertical(rotation, curPosition);

        std::string posResult = "curPosition: <";
        for (int i = 0; i < sizeof(curPosition) / sizeof(curPosition[0]); ++i) {
            posResult += std::to_string(curPosition[i]);

            if (i < sizeof(curPosition) / sizeof(curPosition[0]) - 1) posResult += ", ";
        }
        posResult += ">";

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

        log(posResult);
    }
}