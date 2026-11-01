#pragma once

#include "ResponseUnit.h"

class Ambulance : public ResponseUnit {
public:
    Ambulance(int id, int locID,
              const std::vector<std::string>& caps);

    std::string getUnitType() const override;
    bool canHandle(const Incident& incident) const override;
};
