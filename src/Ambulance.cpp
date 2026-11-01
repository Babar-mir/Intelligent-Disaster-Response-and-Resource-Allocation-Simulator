
#include "Ambulance.h"

Ambulance::Ambulance(
    int id, int locID, const std::vector<std::string>& caps)
    : ResponseUnit(id, locID, caps) {}

std::string Ambulance::getUnitType() const {
    return "Ambulance";
}

bool Ambulance::canHandle(const Incident& incident) const {
    return matchesCapabilities(incident);
}
