
#include <iostream>
#include <cassert>
#include <vector>
#include <ctime>

#include "Incident.h"
#include "Ambulance.h"
#include "ResourceAllocator.h"
#include "RoadNetwork.h"
#include "Assignment.h"

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
    network.addRoad(4, 5, 2);

    // Create incident
    std::vector<Incident> incidents;

    incidents.emplace_back(
        1, "Medical Emergency", 5, 600, 4,
        IncidentStatus::Pending,
        1000,
        std::vector<std::string>{"FirstAid"},
        std::vector<std::string>{}
    );

    // Register response unit
    Ambulance ambulance(1, 1, {"FirstAid"});

    std::vector<ResponseUnit*> units = {&ambulance};

    ResourceAllocator allocator;

    // Step 1: Allocate resources
    auto assignments =
        allocator.allocateResources(incidents, units, network);

    assert(assignments.size() == 1);

    assert(incidents[0].getStatus() == IncidentStatus::Assigned);

    assert(ambulance.getAvailability() == UnitAvailability::Busy);

    // Step 2: Verify shortest route
    PathResult route =
        network.findShortestPath(1, incidents[0].getLocId());

    assert(route.reachable);
    assert(route.distance == 5.0);

    // Step 3: Complete assignment
    std::cout << "Assigned at: "
          << assignments[0].getAssignedAt() << '\n';

    std::cout << "Completing at: 2000\n";
    std::time_t completionTime =assignments[0].getAssignedAt() + 3600;

    assignments[0].complete(completionTime);

    incidents[0].setStatus(IncidentStatus::Resolved);

    ambulance.setAvailability(UnitAvailability::Available);

    ambulance.setLocationID(incidents[0].getLocId());

    // Step 4: Verify final state
    assert(assignments[0].getStatus() == AssignmentStatus::Completed);

    assert(incidents[0].getStatus() == IncidentStatus::Resolved);

    assert(ambulance.getAvailability() == UnitAvailability::Available);

    assert(ambulance.getLocationID() == 4);

    // Step 5: Verify assignment timestamps
    assert(assignments[0].getCompletedAt() == completionTime);
    assert(assignments[0].getCompletedAt() >
       assignments[0].getAssignedAt());

    std::cout << "All integration tests passed.\n";

    return 0;
}
