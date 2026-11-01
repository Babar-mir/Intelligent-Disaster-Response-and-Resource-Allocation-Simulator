#pragma once
#include<string>
#include<vector>
#include "Incident.h"



enum class UnitAvailability{
    Available,
    Busy
};


class ResponseUnit{
    int unitID, locID;
    UnitAvailability availability;
    std::vector<std::string> capabilities;

    public:
    ResponseUnit(int id, int locID,
                 const std::vector<std::string>& caps);

    virtual ~ResponseUnit() = default;

    int getUnitID() const;
    int getLocationID() const;
    UnitAvailability getAvailability() const;
    const std::vector<std::string>& getCapabilities() const;

    void setLocationID(int locID);
    void setAvailability(UnitAvailability state);

    virtual std::string getUnitType() const = 0;
    virtual bool canHandle(const Incident& incident) const = 0;
    bool matchesCapabilities(const Incident& incident) const;
};