#include "include/mazeSolver.h"
#include "include/graphList.h"

GraphList MazeSolver::createGraph(std::array<std::array<int, 16>, 17> horz,
    std::array<std::array<int, 17>, 16> vert) {
    size_t vert_size = vert.size();
    size_t horz_size = horz.size();

    // loop over each node of the graph and check for walls
    // Start at bottom left corner (0, 0) and go to top right corner (15, 15)
    // Based on coordinates used by mms
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            // only need to check up and the right
            // because of bi-directional graphs

            // check for wall to the right
            if (x < 15 && vert[15 - y][x + 1] == 0) {
                graphL.add_edge(x + (y * 16), (x + 1) + (y * 16));
                // cerr << "No vertical wall between " << x + (y * 16) << " and " << (x + 1) + (y * 16) << std::endl;
            } else if (x < 15 && vert[15 - y][x + 1] == 1) {
                // cerr << "Vertical wall between " << x + (y * 16) << " and " << (x + 1) + (y * 16) << std::endl;
            }

            // cerr << "Current coordinates (vert wall space): <" << 15 - y << ", " << x + 1 << "> with value " << vert[15 - y][x + 1] << std::endl;

            // check for wall above
            if (y < 15 && horz[15 - y][x] == 0) {
                graphL.add_edge(x + (y * 16), x + ((y + 1) * 16));
                // cerr << "No horizontal wall between " << x + (y * 16) << " and " << x + ((y + 1) * 16) << std::endl;
            } else if (y < 15 && horz[15 - y][x] == 1) {
                // cerr << "Horizontal wall between " << x + (y * 16) << " and " << x + ((y + 1) * 16) << std::endl;
            }

            // cerr << "Current coordinates (horz wall space): <" << 15 - y << ", " << x << "> with value " << vert[15 - y][x] << std::endl;

            // Label the nodes with their coordinates for debugging
            API::setText(x, y, std::to_string(x + (y * 16)));
        }
    }

    return graphL;
}

void MazeSolver::addEdge(int u, int v) {
    graphL.add_edge_directional(u, v);
}

void MazeSolver::print() {
    // graphM.print();
    graphL.print();
}

// convert a vertex to coordinates (x, y)
pair<int, int> MazeSolver::convertVertToCoords(int vert) {
    return {vert % 16, vert / 16};
}

// get a direction to travel given a change in position
MazeSolver::DIRECTION MazeSolver::getDirFromOffset(int dx, int dy) {
    // convert 2 ints to 1 using its bits (corresponding directions are already set up correctly)
    return (DIRECTION) ((dx & 0b11) | (((dy) & 0b11) << 2));
}

// checks if the current vert is in the center (goal)
bool MazeSolver::foundCenter(int vert) {
    auto coords = convertVertToCoords(vert);
    return (coords.first == 7 || coords.first == 8) && (coords.second == 7 || coords.second == 8);
}

// uses BFS to find shortest path to center
vector<MazeSolver::DIRECTION> MazeSolver::findCenterBFS(int startX, int startY) {
    // setup needed ds, parent is used to reconstruct path
    unordered_map<int, vector<int>>& adjList = graphL.getList();
    vector<unsigned int> seen(adjList.size(), false);
    vector<int> parent(adjList.size(), -1);
    queue<int> q;

    // start BFS from current position
    int startPos = startX + (16 * startY);
    std::cerr << "Starting BFS from position: " << startPos << std::endl;
    seen[startPos] = true;
    q.push(startPos);

    // BFS loop
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        // end early if found the center
        if (foundCenter(current)) {
            return constructPath(parent, current, startX, startY);
        }
        // iterate over neighbors of current node
        for (const auto& neighbor : adjList.at(current)) {
            // cerr << "Current: " << current << ", Neighbor: " << neighbor << std::endl;
            if (!seen[neighbor]) {
                seen[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    // if we reach here, the center wasn't found, return path to 0
    return constructPath(parent, 0, startX, startY); // wasn't found
}

// reconstruct path from parent and find the directions from 0
vector<MazeSolver::DIRECTION> MazeSolver::constructPath(vector<int>& parent, int start, int startX, int startY) {
    vector<int> path;
    vector<DIRECTION> directions;
    for (int current = start; current >= 0; current = parent[current])
        path.push_back(current);
    reverse(path.begin(), path.end());
    pair<int, int> prevCoords = {startX, startY}; // starting position of the mouse
    for (int i = 1; i < path.size(); i++) {
        pair<int, int> coords = convertVertToCoords(path.at(i));
        API::setColor(coords.first, coords.second, MazeSolver::colors[i % 3]);
        directions.push_back(getDirFromOffset(coords.first - prevCoords.first, coords.second - prevCoords.second));
        prevCoords = coords;
    }
    return directions;
}
