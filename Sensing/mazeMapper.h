#include <array>
#include "../include/API.h"

class MazeMapper {

    // dont ask me why we need to write these things out in full. i dont know
    // but whenever we dont, random walls appear. its scary. im scared.
    std::array<std::array<int, 16>, 17> horz = {{
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
    
    public:
    MazeMapper() {};

    std::array<std::array<int, 16>, 17> buildHorizontal(std::array<int, 2> curPosition, int rotation) {
        switch (rotation) {
            case 0:
                //up
                if (API::wallFront()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                break;
            case 1:
                // left
                if (API::wallLeft()) horz[curPosition[1]][curPosition[0]] = 1;
                if (API::wallRight()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                break;
            case 2:
                if (API::wallFront()) horz[curPosition[1]][curPosition[0]] = 1;
                break;
            case 3:
                if (API::wallLeft()) horz[curPosition[1] + 1][curPosition[0]] = 1;
                if (API::wallRight()) horz[curPosition[1]][curPosition[0]] = 1;
                break;
        }
        return horz;
    }

    std::array<std::array<int, 17>, 16> buildVertical(std::array<int, 2> curPosition, int rotation) {
        switch (rotation) {
            case 0:
                //up
                if (API::wallLeft()) vert[curPosition[1]][curPosition[0]] = 1;
                if (API::wallRight()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                break;
            case 1:
                // left
                if (API::wallFront()) vert[curPosition[1]][curPosition[0]] = 1;
                break;
            case 2:
                if (API::wallLeft()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                if (API::wallRight()) vert[curPosition[1]][curPosition[0]] = 1;
                break;
            case 3:
                if (API::wallFront()) vert[curPosition[1]][curPosition[0] + 1] = 1;
                break;
        }
        return vert;
    }

    std::array<std::array<int, 16>, 17> getHorizontal() {
        return horz;
    }
    std::array<std::array<int, 17>, 16> getVertical() {
        return vert;
    }
};
