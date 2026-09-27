#include <array>
#include "API.h"

class MazeMapper {
    private:
    std::array<std::array<int, 16>, 17> horz;
    std::array<std::array<int, 17>, 16> vert;
    
    public:
    MazeMapper() {};

    std::array<std::array<int, 16>, 17> buildHorizontal(int rotation, std::array<int, 2> curPosition) {
        std::array<std::array<int, 16>, 17> horz;
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

    std::array<std::array<int, 17>, 16> buildVertical(int rotation, std::array<int, 2> curPosition) {
        std::array<std::array<int, 17>, 16> vert;
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