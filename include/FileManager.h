#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Incident.h"
#include "ResponseUnit.h"
#include "Assignment.h"

class FileManager {
private:
    std::string dataDirectory;

public:
    explicit FileManager(
        const std::string& directory = "data"
    );

    void saveIncidents(
        const std::vector<Incident>& incidents
    ) const;

    void saveUnits(
        const std::vector<std::unique_ptr<ResponseUnit>>& units
    ) const;

    void saveAssignments(const std::vector<Assignment>& assignments) const;

    bool loadIncidents(std::vector<Incident>& incidents) const;

    bool loadUnits(std::vector<std::unique_ptr<ResponseUnit>>& units) const;

    bool loadAssignments(std::vector<Assignment>& assignments) const;

    void saveAll(const std::vector<Incident>& incidents,const std::vector<std::unique_ptr<ResponseUnit>>& units,const std::vector<Assignment>& assignments) const;

    void loadAll(std::vector<Incident>& incidents,std::vector<std::unique_ptr<ResponseUnit>>& units,std::vector<Assignment>& assignments) const;
};