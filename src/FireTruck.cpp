
#include "FireTruck.h"

FireTruck::FireTruck(
    int id, int locID, const std::vector<std::string>& caps)
    : ResponseUnit(id, locID, caps) {}

std::string FireTruck::getUnitType() const {
    return "FireTruck";
}

bool FireTruck::canHandle(const Incident& incident) const {
    return matchesCapabilities(incident);
}
