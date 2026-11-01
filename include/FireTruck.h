
#pragma once

#include "ResponseUnit.h"

class FireTruck : public ResponseUnit {
public:
    FireTruck(int id, int locID,
              const std::vector<std::string>& caps);

    std::string getUnitType() const override;
    bool canHandle(const Incident& incident) const override;
};
