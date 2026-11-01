#include "Assignment.h"
#include <stdexcept>

Assignment::Assignment(
    int id, int incident, int unit, std::time_t time)
    : assignmentID(id),
      incidentID(incident),
      unitID(unit),
      assignedAt(time),
      completedAt(0),
      status(AssignmentStatus::Active)
{
    if (id <= 0 || incident <= 0 || unit <= 0)
        throw std::invalid_argument("IDs must be positive.");
}

int Assignment::getAssignmentID() const {
    return assignmentID;
}

int Assignment::getIncidentID() const {
    return incidentID;
}

int Assignment::getUnitID() const {
    return unitID;
}

std::time_t Assignment::getAssignedAt() const {
    return assignedAt;
}

std::time_t Assignment::getCompletedAt() const {
    return completedAt;
}

AssignmentStatus Assignment::getStatus() const {
    return status;
}

void Assignment::complete(std::time_t time) {
    if (status != AssignmentStatus::Active)
        throw std::logic_error("Assignment is not active.");

    if (time < assignedAt)
        throw std::invalid_argument(
            "Completion time cannot precede assignment time.");

    completedAt = time;
    status = AssignmentStatus::Completed;
}

void Assignment::cancel() {
    if (status != AssignmentStatus::Active)
        throw std::logic_error("Assignment is not active.");

    status = AssignmentStatus::Cancelled;
}