#include "RoadNetwork.h"

#include <stdexcept>
#include <cmath>
#include <queue>
#include <functional>
#include <limits>
#include <algorithm>

void RoadNetwork::addLocation(int locationID) {
    if (locationID <= 0) {
        throw std::invalid_argument(
            "Location ID must be positive."
        );
    }

    adjacencyList.emplace(locationID,std::vector<std::pair<int, double>>{}
    );
}

void RoadNetwork::addRoad(
    int location1,
    int location2,
    double distance
) {
    if (location1 <= 0 || location2 <= 0) {
        throw std::invalid_argument(
            "Location IDs must be positive."
        );
    }

    if (location1 == location2) {
        throw std::invalid_argument(
            "A road cannot connect a location to itself."
        );
    }

    if (distance <= 0.0 || !std::isfinite(distance)) {
        throw std::invalid_argument(
            "Road distance must be finite and positive."
        );
    }

    if (adjacencyList.find(location1) == adjacencyList.end() ||
        adjacencyList.find(location2) == adjacencyList.end()) {
        throw std::invalid_argument(
            "Both locations must exist before adding a road."
        );
    }

    adjacencyList.at(location1).push_back(
        {location2, distance}
    );

    adjacencyList.at(location2).push_back(
        {location1, distance}
    );
}

PathResult RoadNetwork::findShortestPath(
    int source,
    int destination
) const {
    if (adjacencyList.find(source) == adjacencyList.end() ||
        adjacencyList.find(destination) == adjacencyList.end()) {
        throw std::invalid_argument("Source or destination does not exist.");
    }

    using QueueEntry = std::pair<double, int>;

    std::priority_queue<
        QueueEntry,
        std::vector<QueueEntry>,
        std::greater<QueueEntry>
    > pq;

    std::unordered_map<int, double> distances;
    std::unordered_map<int, int> previous;

    for (const auto& location : adjacencyList) {
        distances[location.first] =
            std::numeric_limits<double>::infinity();
    }

    distances[source] = 0.0;
    pq.push({0.0, source});

    while (!pq.empty()) {
        double currentDistance = pq.top().first;
        int currentLocation = pq.top().second;
        pq.pop();

        if (currentDistance > distances[currentLocation]) {
            continue;
        }

        if (currentLocation == destination) {
            break;
        }

        for (const auto& road : adjacencyList.at(currentLocation)) {
            int neighbor = road.first;
            double roadDistance = road.second;

            double newDistance =
                currentDistance + roadDistance;

            if (newDistance < distances[neighbor]) {
                distances[neighbor] = newDistance;
                previous[neighbor] = currentLocation;

                pq.push({newDistance, neighbor});
            }
        }
    }

    
    if (std::isinf(distances[destination])) {
        return {
            std::numeric_limits<double>::infinity(),
            {},
            false
        };
    }

   
    std::vector<int> path;

    int current = destination;

    while (current != source) {
        path.push_back(current);
        current = previous.at(current);
    }

    path.push_back(source);

    std::reverse(path.begin(), path.end());

    return {
        distances[destination],
        path,
        true
    };
}