#pragma once

#include <ctime>

enum class AssignmentStatus {
    Active,
    Completed,
    Cancelled
};

class Assignment {
private:
    int assignmentID;
    int incidentID;
    int unitID;
    std::time_t assignedAt;
    std::time_t completedAt;
    AssignmentStatus status;

public:
    Assignment(int id, int incident, int unit, std::time_t time);

    int getAssignmentID() const;
    int getIncidentID() const;
    int getUnitID() const;

    std::time_t getAssignedAt() const;
    std::time_t getCompletedAt() const;

    AssignmentStatus getStatus() const;

    void complete(std::time_t time);
    void cancel();
};