#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <glm/glm.hpp>

struct Node {
    glm::ivec2 position; // Use glm for position
    float gCost; // Cost from start to this node
    float hCost; // Heuristic cost to end
    float fCost() const { return gCost + hCost; }
    bool operator>(const Node& other) const { return fCost() > other.fCost(); }
};

class Pathfinding {
public:
    Pathfinding(const std::vector<std::vector<uint8_t>>& grid);
    std::vector<Node> aStar(Node start, Node end);

private:
    float heuristic(const Node& a, const Node& b);
    bool isValid(int x, int y);

    std::vector<std::vector<uint8_t>> m_Grid;
};
