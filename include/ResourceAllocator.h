#pragma once

#include <vector>
#include "Incident.h"
#include "ResponseUnit.h"
#include "RoadNetwork.h"
#include "Assignment.h"

class ResourceAllocator {
private:
    int nextAssignmentID;

    double calculatePriority(const Incident& incident) const;

public:
    ResourceAllocator();

    std::vector<Assignment> allocateResources(std::vector<Incident>& incidents,
        std::vector<ResponseUnit*>& units,
        const RoadNetwork& network);
    void setNextAssignmentID(int id);
};

