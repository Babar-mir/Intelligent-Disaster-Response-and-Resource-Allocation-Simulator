
#include <iostream>
#include <cassert>
#include <vector>

#include "Incident.h"
#include "Ambulance.h"
#include "FireTruck.h"
#include "RescueTeam.h"
#include "ResourceAllocator.h"

int main() {
    RoadNetwork network;

    for (int i = 1; i <= 5; i++) {
        network.addLocation(i);
    }

    network.addRoad(1, 2, 2);
    network.addRoad(1, 3, 5);
    network.addRoad(2, 3, 2);
    network.addRoad(3, 4, 2);
    network.addRoad(4, 5, 2);
    {
        Incident incident(1, "Medical", 4, 500, 4,IncidentStatus::Pending, 1000,
            {"FirstAid"}, {}
        );

        Ambulance nearUnit(1, 3, {"FirstAid"});
        Ambulance farUnit(2, 1, {"FirstAid"});

        std::vector<Incident> incidents = {incident};
        std::vector<ResponseUnit*> units = {&nearUnit, &farUnit};

        ResourceAllocator allocator;
        auto assignments =
            allocator.allocateResources(incidents, units, network);

        assert(assignments.size() == 1);
        assert(assignments[0].getUnitID() == 1);
        assert(nearUnit.getAvailability() == UnitAvailability::Busy);
    }

    {
        Incident incident(2, "Medical", 3, 200, 3,IncidentStatus::Pending, 1000,{"FirstAid"}, {}
        );

        Ambulance busyUnit(3, 3, {"FirstAid"});
        Ambulance availableUnit(4, 1, {"FirstAid"});

        busyUnit.setAvailability(UnitAvailability::Busy);

        std::vector<Incident> incidents = {incident};
        std::vector<ResponseUnit*> units = {
            &busyUnit, &availableUnit
        };

        ResourceAllocator allocator;
        auto assignments =
            allocator.allocateResources(incidents, units, network);

        assert(assignments.size() == 1);
        assert(assignments[0].getUnitID() == 4);
    }

    // Test 3: Incompatible units must be skipped
    {
        Incident incident(
            3, "Flood", 4, 500, 3,
            IncidentStatus::Pending, 1000,
            {"WaterRescue"}, {}
        );

        Ambulance unit(5, 1, {"FirstAid"});

        std::vector<Incident> incidents = {incident};
        std::vector<ResponseUnit*> units = {&unit};

        ResourceAllocator allocator;
        auto assignments =
            allocator.allocateResources(incidents, units, network);

        assert(assignments.empty());
        assert(unit.getAvailability() == UnitAvailability::Available);
    }

    // Test 4: Higher-priority incident gets allocated first
    {
        Incident lowPriority(
            4, "Medical", 2, 100, 3,
            IncidentStatus::Pending, 1000,
            {"FirstAid"}, {}
        );

        Incident highPriority(
            5, "Medical", 5, 1000, 3,
            IncidentStatus::Pending, 1000,
            {"FirstAid"}, {}
        );

        Ambulance unit1(6, 1, {"FirstAid"});
        Ambulance unit2(7, 2, {"FirstAid"});

        std::vector<Incident> incidents = {
            lowPriority, highPriority
        };

        std::vector<ResponseUnit*> units = {
            &unit1, &unit2
        };

        ResourceAllocator allocator;
        auto assignments =
            allocator.allocateResources(incidents, units, network);

        assert(assignments.size() == 2);
        assert(assignments[0].getIncidentID() == 5);
        assert(assignments[1].getIncidentID() == 4);
    }

    // Test 5: No suitable unit available
    {
        Incident incident(
            6, "Rescue", 5, 800, 3,
            IncidentStatus::Pending, 1000,
            {"MountainRescue"}, {}
        );

        Ambulance unit(8, 1, {"FirstAid"});

        std::vector<Incident> incidents = {incident};
        std::vector<ResponseUnit*> units = {&unit};

        ResourceAllocator allocator;
        auto assignments =
            allocator.allocateResources(incidents, units, network);

        assert(assignments.empty());
        assert(incidents[0].getStatus() == IncidentStatus::Pending);
    }

    std::cout << "All ResourceAllocator tests passed.\n";

    return 0;
}
