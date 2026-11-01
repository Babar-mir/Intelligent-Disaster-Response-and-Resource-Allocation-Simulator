#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

struct PathResult {
    double distance;std::vector<int> path;
    bool reachable;
};

class RoadNetwork {
private:
    std::unordered_map<int,std::vector<std::pair<int, double>>> adjacencyList;

public:
    void addLocation(int locationID);

    void addRoad(
        int location1,
        int location2,
        double distance
    );

    PathResult findShortestPath(
        int source,
        int destination
    ) const;
};