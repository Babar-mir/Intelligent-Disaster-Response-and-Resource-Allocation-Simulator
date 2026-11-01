#include "ResourceAllocator.h"

#include <algorithm>
#include <ctime>
#include <limits>
#include<stdexcept>

ResourceAllocator::ResourceAllocator()
    : nextAssignmentID(1) {}

double ResourceAllocator::calculatePriority(
    const Incident& incident) const {
    return incident.getPriorityScore();
}


std::vector<Assignment> ResourceAllocator::allocateResources(
    std::vector<Incident>& incidents,
    std::vector<ResponseUnit*>& units,
    const RoadNetwork& network
) {
    std::vector<Assignment> newAssignments;
    std::vector<Incident*> pendingIncidents;

    for (auto& incident : incidents) {
        if (incident.getStatus() == IncidentStatus::Pending) {
            pendingIncidents.push_back(&incident);
        }
    }
    std::sort(
        pendingIncidents.begin(),
        pendingIncidents.end(),
        [this](const Incident* a, const Incident* b) {
            double priorityA = calculatePriority(*a);
            double priorityB = calculatePriority(*b);

            if (priorityA != priorityB) {
                return priorityA > priorityB;
            }

            return a->getReportTime() < b->getReportTime();
        }
    );

    for (Incident* incident : pendingIncidents) {
        ResponseUnit* bestUnit = nullptr;

        double bestDistance =
            std::numeric_limits<double>::infinity();
        for (ResponseUnit* unit : units) {
            if (unit == nullptr ||
                unit->getAvailability() != UnitAvailability::Available) {
                continue;
            }

            if (!unit->canHandle(*incident)) {
                continue;
            }

            PathResult route = network.findShortestPath(
                unit->getLocationID(),
                incident->getLocId()
            );

            if (!route.reachable) {
                continue;
            }

            if (route.distance < bestDistance) {
                bestDistance = route.distance;
                bestUnit = unit;
            }
        }
        if (bestUnit == nullptr) {
            continue;
        }
        Assignment assignment(
            nextAssignmentID++,
            incident->getIncidentId(),
            bestUnit->getUnitID(),
            std::time(nullptr)
        );

        newAssignments.push_back(assignment);
        bestUnit->setAvailability(UnitAvailability::Busy);
        incident->setStatus(IncidentStatus::Assigned);
    }

    return newAssignments;
}

void ResourceAllocator::setNextAssignmentID(int id) {
    if (id <= 0) {
        throw std::invalid_argument(
            "Assignment ID must be positive."
        );
    }

    nextAssignmentID = id;
}