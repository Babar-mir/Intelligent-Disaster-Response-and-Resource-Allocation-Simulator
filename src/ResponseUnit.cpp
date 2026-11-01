#include "ResponseUnit.h"
#include <stdexcept>
#include <algorithm>

ResponseUnit::ResponseUnit(
    int id, int LID, const std::vector<std::string>& caps):
    unitID(id),
    locID(LID),
    availability(UnitAvailability::Available),
    capabilities(caps){
        if(id<=0)throw std::invalid_argument("Unit Id must be positive.");
        
        if(LID<=0)throw std::invalid_argument("Location Id must be positive.");
    }


int ResponseUnit::getUnitID() const {
    return unitID;
}

int ResponseUnit::getLocationID() const {
    return locID;
}

UnitAvailability ResponseUnit::getAvailability() const {
    return availability;
}

const std::vector<std::string>&
ResponseUnit::getCapabilities() const {
    return capabilities;
}

void ResponseUnit::setLocationID(int locID) {
    if (locID <= 0)
        throw std::invalid_argument("Location ID must be positive.");

    this->locID = locID;
}

void ResponseUnit::setAvailability(UnitAvailability state) {
    availability = state;
}



bool ResponseUnit::matchesCapabilities(
    const Incident& incident
) const {

    const auto& required =incident.getMandatoryCapabilities();

    const auto& optional =incident.getOptionalCapabilities();

    for (const auto& capability : required) {
        if (std::find(capabilities.begin(),capabilities.end(),capability) == capabilities.end()) {
            return false;
        }
    }

    if (optional.empty()) {
        return true;
    }

    int matched = 0;

    for (const auto& capability : optional) {
        if (std::find(
                capabilities.begin(),
                capabilities.end(),
                capability
            ) != capabilities.end()) {
            matched++;
        }
    }

    double compatibility =static_cast<double>(matched) / optional.size();

    return compatibility >= 0.75;
}