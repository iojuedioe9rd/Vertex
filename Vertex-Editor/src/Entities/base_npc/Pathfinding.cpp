#include "Pathfinding.h"

Pathfinding::Pathfinding(const std::vector<std::vector<uint8_t>>& grid) : m_Grid(grid) {}

float Pathfinding::heuristic(const Node& a, const Node& b) {
    return glm::abs(a.position.x - b.position.x) + glm::abs(a.position.y - b.position.y);
}

bool Pathfinding::isValid(int x, int y) {
    return (x >= 0 && x < m_Grid.size() && y >= 0 && y < m_Grid[0].size() && m_Grid[x][y] == 0);
}

std::vector<Node> Pathfinding::aStar(Node start, Node end) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_map<int, std::unordered_map<int, Node>> allNodes;
    std::unordered_map<int, std::unordered_map<int, bool>> closedSet;

    start.gCost = 0;
    start.hCost = heuristic(start, end);
    openSet.push(start);

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.position == end.position) {
            std::vector<Node> path;
            while (current.position != start.position) {
                path.push_back(current);
                current = allNodes[current.position.x][current.position.y];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet[current.position.x][current.position.y] = true;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) continue;
                int neighborX = current.position.x + dx;
                int neighborY = current.position.y + dy;

                if (!isValid(neighborX, neighborY) || closedSet[neighborX][neighborY]) {
                    continue;
                }

                Node neighbor = { { neighborX, neighborY }, current.gCost + 1, heuristic({ { neighborX, neighborY }, 0, 0 }, end) };

                if (allNodes[neighbor.position.x][neighbor.position.y].gCost == 0 || neighbor.gCost < allNodes[neighbor.position.x][neighbor.position.y].gCost) {
                    allNodes[neighbor.position.x][neighbor.position.y] = current;
                    openSet.push(neighbor);
                }
            }
        }
    }

    return {};
}