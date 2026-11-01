#pragma once
#include "ResponseUnit.h"


class RescueTeam: public ResponseUnit{
    public:
        RescueTeam(int id, int locID,
               const std::vector<std::string>& caps);
               
               std::string getUnitType() const override;
               bool canHandle(const Incident& incident) const override;
};