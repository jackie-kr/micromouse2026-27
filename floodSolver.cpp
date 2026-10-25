// Micromouse floodfill maze-solving algorithm
// Compatible with MMS Simulator
// Written in C++ for robotics competitions
#include "include/API.h"
#include <array>
#include <queue>
#include <string>
#include <iostream>
#include <utility>
#include "flood.h"
#include <vector>
#include "Sensing/mazeMapper.h"
#include "Sensing/arrayTranslator.h"
using namespace std;
Flood floodfill;

enum Action { FORWARD, LEFT, RIGHT, IDLE, AROUND};

const int SIZE = 16;

int curRow = 0, curCol = 0, curDir = 0;  // 0=north,1=east,2=south,3=west

void log(const string& text) {
    cerr << text << endl;
}

float getStat(const std::string& statName) {
    std::cout << "getStat " << statName << std::endl;
    std::string response;
    std::cin >> response;
    try {
        return std::stof(response); // handles both int and float
    } catch (...) {
        std::cerr << "Failed to parse stat: " << statName << std::endl;
        return -1;
    }
}

bool atGoal(int row, int col, std::vector<std::pair<int, int>> goals) {
    bool at_goal = false;
    for (const auto& goal : goals) {
        if (row == goal.first && col == goal.second) {
            at_goal = true;
            std::cerr << "At goal (" << to_string(goal.second) << ", " << to_string(goal.first) << ").\n";
            break;
        }
    }

    return at_goal;
}

void Flood::setWall(int row, int col, int direction) {
    if(direction == 0) {
        maze.vertical_walls[row][col].first = 1; // North wall
        if(row+1 < 16) maze.vertical_walls[row + 1][col].second = 1; // South wall of the cell above
    }
    if(direction == 1) {
        maze.horizontal_walls[row][col].second = 1; // East wall
        if(col + 1 < 16) maze.horizontal_walls[row][col + 1].first = 1; // West wall of the cell to the right
    }
    if(direction == 2) {
        maze.vertical_walls[row][col].second = 1; // South wall
        if(row - 1 >= 0) maze.vertical_walls[row - 1][col].first = 1; // North wall of the cell below
    }
    if(direction == 3) {
        maze.horizontal_walls[row][col].first = 1; // West wall
        if(col - 1 >= 0) maze.horizontal_walls[row][col - 1].second = 1; // East wall of the cell to the left
    }
    char cdir = "nesw"[direction];
    // API::setWall(col, row, cdir);
}

void Flood::setWalls(std::array<std::array<int, 16>, 17> horz, std::array<std::array<int, 17>, 16> vert) {
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            // if there's a wall to the right
            if (x < 15 && vert[15 - y][x + 1] == 1) {
                // set the current coord to have a wall to the right
                // std::cerr << "Wall at right of (" << x << ", " << 15 - y << ")." << std::endl;
                maze.horizontal_walls[15 - y][x].second = 1;
                maze.horizontal_walls[15 - y][x + 1].first = 1;
            } else if (x == 15) {
                // at the far right
                maze.horizontal_walls[15 - y][x].second = 1;
            }

            if (x == 0) {
                // Make sure left is filled
                maze.horizontal_walls[15 - y][x].first = 1;
            }

            // Check for wall below
            if (y < 15 && horz[15 - y][x] == 1) {
                // std::cerr << "Wall below (" << x << ", " << 15 - y << ")." << std::endl;
                maze.vertical_walls[15 - y][x].second = 1;
                maze.vertical_walls[15 - y - 1][x].first = 1;
            } else if (y == 15) {
                // If at top
                maze.vertical_walls[15 - y][x].second = 1;
            }

            if (y == 0) {
                maze.vertical_walls[15 - y][x].first = 1;
            }
        }
    }
}

void Flood::detectWalls(vector<int> sensorDistances, int row, int col, int direction){
    if(row < 0 || row >= 16 || col < 0 || col >= 16) return; // out of bounds
    if(API::wallFront()) { // North Wall
        setWall(row, col, direction);
        // Serial.print("Wall detected at ");
        // Serial.print(direction);
        // Serial.print(" with distance: ");
        // Serial.println(sensorDistances[2]);
    }
    if(API::wallLeft()) {
        setWall(row, col, (direction + 3) % 4);
        // Serial.print("Wall detected at ");
        // Serial.print((direction + 3) % 4);
        // Serial.print(" with distance: ");
        // Serial.println(sensorDistances[0]);
    }
    //if(sensorDistances[1] < wall_threshhold) setWall(row, col, direction);
    if(API::wallRight()) {
        setWall(row, col, (direction + 1) % 4);
        // Serial.print("Wall detected at ");
        // Serial.print((direction + 1) % 4);
        // Serial.print(" with distance: ");
        // Serial.println(sensorDistances[4]);
    }
}
bool Flood::hasWall(int row, int col, int dir) {
    if(dir == 0) return maze.vertical_walls[row][col].first; // North wall
    if(dir == 1) return maze.horizontal_walls[row][col].second; // East wall
    if(dir == 2) return maze.vertical_walls[row][col].second; // South wall
    if(dir == 3) return maze.horizontal_walls[row][col].first; // West wall
    return false; // No walls
}

void Flood::floodfill(std::vector<pair<int, int>> goals) {
    queue<pair<int, int>> q;
    for (auto& row : maze.manhattan_distances)
        row.fill(255); // Re-initialize all distances to a large number
    array<array<bool, 16>, 16> reached = {};

    for (auto goal : goals) {
        if (goal.first < 16  && goal.second < 16) {
            // log("Exploring to goal: (" + to_string(goal.second) + ", " + to_string(goal.first) + ")");
            maze.manhattan_distances[goal.first][goal.second] = 0; //row,column
            q.push({goal.first, goal.second});
            reached[goal.first][goal.second] = true; // Marking the goal cells as reached
        }
    }

    // BFS to fill the manhattan distances
    while(!q.empty()){
        pair<int,int> cell = q.front();
        q.pop();

        if(cell.first > 15 || cell.second > 15 || cell.first < 0 || cell.second < 0) continue; // out of bounds

        if(cell.second > 0 && !maze.horizontal_walls[cell.first][cell.second].first && !reached[cell.first][cell.second - 1]) {// No wall to the left
            maze.manhattan_distances[cell.first][cell.second - 1] = maze.manhattan_distances[cell.first][cell.second] + 1;
            q.push({cell.first, cell.second - 1});
            reached[cell.first][cell.second - 1] = true; // Marking the cell as reached
        }
        if(cell.first < 15 && !maze.vertical_walls[cell.first][cell.second].first && !reached[cell.first + 1][cell.second]) {// No wall above
            maze.manhattan_distances[cell.first + 1][cell.second] = maze.manhattan_distances[cell.first][cell.second] + 1;
            q.push({cell.first + 1, cell.second});
            reached[cell.first + 1][cell.second] = true; // Marking the cell as reached
        }
        if(cell.second < 15 && !maze.horizontal_walls[cell.first][cell.second].second && !reached[cell.first][cell.second + 1]) {// No wall to the right
            maze.manhattan_distances[cell.first][cell.second + 1] = maze.manhattan_distances[cell.first][cell.second] + 1;
            q.push({cell.first, cell.second + 1});
            reached[cell.first][cell.second + 1] = true; // Marking the cell as reached
        }
        if(cell.first > 0 && !maze.vertical_walls[cell.first][cell.second].second && !reached[cell.first - 1][cell.second]) {// No wall below
            maze.manhattan_distances[cell.first - 1][cell.second] = maze.manhattan_distances[cell.first][cell.second] + 1;
            q.push({cell.first - 1, cell.second});
            reached[cell.first - 1][cell.second] = true; // Marking the cell as reached
        }
    }

    for (int r = 0; r < 16; ++r) {
        for (int c = 0; c < 16; ++c) {
            int dist = maze.manhattan_distances[r][c];
            if (dist != 255) {
                API::setText(c, r, to_string(dist));  // NOTE: col=x, row=y
            }
        }
    }
}

int Flood::getNextMove(int row, int col /*int direction //for optimize movement(for later improvement)*/) {
    int minDist = 255;
    int bestDirection = -1;

    // int nextRow = row;
    // int nextCol = col;

    if(!hasWall(row, col, 0) && maze.manhattan_distances[row + 1][col] < minDist) {
        minDist = maze.manhattan_distances[row + 1][col];
        bestDirection = 0;
    }
    if(!hasWall(row, col, 1) && maze.manhattan_distances[row][col + 1] < minDist) {
        minDist = maze.manhattan_distances[row][col + 1];
        bestDirection = 1;
    }
    if(!hasWall(row, col, 2) && maze.manhattan_distances[row - 1][col] < minDist) {
        minDist = maze.manhattan_distances[row - 1][col];
        bestDirection = 2;
    }
    if(!hasWall(row, col, 3) && maze.manhattan_distances[row][col - 1] < minDist) {
        minDist = maze.manhattan_distances[row][col - 1];
        bestDirection = 3;
    }

    // Serial.print("Let's move to the direction coded as ");
    // Serial.println(bestDirection);
    // cerr << "Let's move to the direction coded as " << bestDirection << endl;

    return bestDirection; // Return the best direction to move based on the minimum distance
}


Action rotateTo(int newDir) {
    int diff = (newDir - curDir + 4) % 4;
    if (diff == 0) return FORWARD;
    if (diff == 1) return RIGHT;
    if (diff == 3) return LEFT;
    if (diff == 2) return AROUND;  // U-turn
    return IDLE;
}


void moveForwardUpdatePos() {
    if (curDir == 0) curRow++;
    if (curDir == 1) curCol++;
    if (curDir == 2) curRow--;
    if (curDir == 3) curCol--;

}

Action solver(std::vector<pair<int, int>> goals, std::array<std::array<int, 16>, 17> horz, std::array<std::array<int, 17>, 16> vert) {
    // floodfill.detectWalls({}, curRow, curCol, curDir);
    floodfill.setWalls(horz, vert);
    floodfill.floodfill(goals);
    int bestDir = floodfill.getNextMove(curRow, curCol);

    Action action = rotateTo(bestDir);
    // std::cerr << "Moved to: (" << curRow << ", " << curCol << ")" << std::endl;
    //API::setColor(curRow, curCol, 'g');
    // std::cerr << "Action decided: " << action << std::endl;
    if (action == IDLE) {
        // std::cerr << "No valid move found, staying idle." << std::endl;
    }
    return action;
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

int translateRotation(int current) {
    switch (current) {
        case 0:
            return 0;
        case 1:
            return 3;
        case 2:
            return 2;
        case 3:
            return 1;
        default:
            return -1;
    }
}

int main(int argc, char* argv[]) {
    MazeMapper mapper;
    log("Running...");

    auto horz = mapper.buildHorizontal({curCol, curRow}, translateRotation(curDir));
    auto vert = mapper.buildVertical({curCol, curRow}, translateRotation(curDir));

    while (!mapper.mapCompleted()) {
        // log("New goal found hopefully");
        auto goals = mapper.getUnexplored(floodfill.maze.manhattan_distances);
        // log("New goal found: " + to_string(goals.size()) + " goals");

        for (auto goal : goals) {
            API::setColor(goal.second, goal.first, 'g');
            // log("New goal: (" + to_string(goal.second) + ", " + to_string(goal.first) + ")");
        }

        while (!atGoal(curRow, curCol, goals)) {
            Action action = solver(goals, horz, vert);

            if (action == FORWARD) {
                API::moveForward();
                API::setColor(curCol, curRow, 'c');
                API::setText(curCol, curRow, to_string(floodfill.maze.manhattan_distances[curRow][curCol]));
                moveForwardUpdatePos();
            } else if (action == LEFT) {
                API::turnLeft();
                curDir = (curDir + 3) % 4;
            } else if (action == RIGHT) {
                API::turnRight();
                curDir = (curDir + 1) % 4;
            } else if (action == AROUND) {
                API::turnRight();
                API::turnRight();
                curDir = (curDir + 2) % 4;
            }

            horz = mapper.buildHorizontal({curCol, curRow}, translateRotation(curDir));
            vert = mapper.buildVertical({curCol, curRow}, translateRotation(curDir));

            log_maze(horz, vert);
        }

        for (auto goal : goals) {
            // log("Reached goal: (" + to_string(goal.second) + ", " + to_string(goal.first) + ")");
            API::clearColor(goal.second, goal.first);
        }
    }
    return 0;
}
