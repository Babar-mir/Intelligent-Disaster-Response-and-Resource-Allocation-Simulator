
#include "RescueTeam.h"

RescueTeam::RescueTeam(
    int id, int locID, const std::vector<std::string>& caps)
    : ResponseUnit(id, locID, caps) {}

std::string RescueTeam::getUnitType() const {
    return "RescueTeam";
}

bool RescueTeam::canHandle(const Incident& incident) const {
    return matchesCapabilities(incident);
}
