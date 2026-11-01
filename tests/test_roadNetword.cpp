
#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include "RoadNetwork.h"

int main() {
    RoadNetwork network;
    for (int i = 1; i <= 5; i++) {
        network.addLocation(i);
    }
    network.addRoad(1, 2, 4);
    network.addRoad(1, 3, 2);
    network.addRoad(2, 3, 3);
    network.addRoad(2, 4, 6);
    network.addRoad(3, 4, 3);
    PathResult result = network.findShortestPath(1, 4);

    assert(result.reachable);
    assert(result.distance == 5.0);
    assert(result.path.front() == 1);
    assert(result.path.back() == 4);
    PathResult unreachable = network.findShortestPath(1, 5);

    assert(!unreachable.reachable);
    assert(unreachable.path.empty());
    PathResult same = network.findShortestPath(2, 2);

    assert(same.reachable);
    assert(same.distance == 0.0);
    assert(same.path.size() == 1);
    assert(same.path[0] == 2);
    bool exceptionThrown = false;

    try {
        network.addRoad(1, 2, -5);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    exceptionThrown = false;

    try {
        network.addRoad(1, 10, 5);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All RoadNetwork tests passed.\n";

    return 0;
}
