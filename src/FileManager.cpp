#include "FileManager.h"

#include "Ambulance.h"
#include "FireTruck.h"
#include "RescueTeam.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <algorithm>
#include <ctime>
#include <unordered_set>

namespace {

std::vector<std::string> split(
    const std::string& value,
    char delimiter
) {
    std::vector<std::string> result;
    std::stringstream stream(value);
    std::string item;

    while (std::getline(stream, item, delimiter)) {
        result.push_back(item);
    }

    if (!value.empty() && value.back() == delimiter) {
        result.emplace_back();
    }

    return result;
}

template <typename T>
T parseNumber(const std::string& value) {
    std::stringstream stream(value);
    T result{};
    char extra;

    if (!(stream >> result) || (stream >> extra)) {
        throw std::runtime_error("Invalid numeric CSV value.");
    }

    return result;
}

std::string join(
    const std::vector<std::string>& values
) {
    std::string result;

    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            result += "|";
        }

        result += values[i];
    }

    return result;
}

std::string availabilityToString(
    UnitAvailability state
) {
    return state == UnitAvailability::Available
        ? "Available" : "Busy";
}

UnitAvailability stringToAvailability(
    const std::string& value
) {
    if (value == "Available")
        return UnitAvailability::Available;

    if (value == "Busy")
        return UnitAvailability::Busy;

    throw std::runtime_error("Invalid unit availability.");
}

std::string incidentStatusToString(
    IncidentStatus status
) {
    switch (status) {
        case IncidentStatus::Pending:
            return "Pending";

        case IncidentStatus::Assigned:
            return "Assigned";

        case IncidentStatus::Resolved:
            return "Resolved";
    }

    throw std::runtime_error("Invalid incident status.");
}

IncidentStatus stringToIncidentStatus(
    const std::string& value
) {
    if (value == "Pending")
        return IncidentStatus::Pending;

    if (value == "Assigned")
        return IncidentStatus::Assigned;

    if (value == "Resolved")
        return IncidentStatus::Resolved;

    throw std::runtime_error("Invalid incident status.");
}

std::string assignmentStatusToString(
    AssignmentStatus status
) {
    switch (status) {
        case AssignmentStatus::Active:
            return "Active";

        case AssignmentStatus::Completed:
            return "Completed";

        case AssignmentStatus::Cancelled:
            return "Cancelled";
    }

    throw std::runtime_error("Invalid assignment status.");
}

void validateRow(
    const std::vector<std::string>& fields,
    std::size_t expected
) {
    if (fields.size() != expected) {
        throw std::runtime_error(
            "Malformed CSV row: incorrect number of fields."
        );
    }
}

void validateUniqueID(
    std::unordered_set<int>& ids,
    int id
) {
    if (id <= 0 || !ids.insert(id).second) {
        throw std::runtime_error(
            "Invalid or duplicate ID in CSV file."
        );
    }
}

} // anonymous namespace


FileManager::FileManager(const std::string& directory)
    : dataDirectory(directory) {
    if (dataDirectory.empty()) {
        throw std::invalid_argument(
            "Data directory cannot be empty."
        );
    }

    std::filesystem::create_directories(dataDirectory);
}


// ---------------- SAVE INCIDENTS ----------------

void FileManager::saveIncidents(
    const std::vector<Incident>& incidents
) const {
    std::ofstream file(
        dataDirectory + "/incidents.csv"
    );

    if (!file)
        throw std::runtime_error("Cannot save incidents.");

    file << "incidentID,type,severity,affectedPopulation,"
            "locationID,status,reportTime,"
            "mandatoryCapabilities,optionalCapabilities\n";

    for (const auto& incident : incidents) {
        file << incident.getIncidentId() << ","
             << incident.getType() << ","
             << incident.getSeverity() << ","
             << incident.getAffPop() << ","
             << incident.getLocId() << ","
             << incidentStatusToString(incident.getStatus()) << ","
             << incident.getReportTime() << ","
             << join(incident.getMandatoryCapabilities()) << ","
             << join(incident.getOptionalCapabilities())
             << "\n";
    }

    if (!file)
        throw std::runtime_error("Error writing incidents.");
}


// ---------------- SAVE RESPONSE UNITS ----------------

void FileManager::saveUnits(
    const std::vector<std::unique_ptr<ResponseUnit>>& units
) const {
    std::ofstream file(
        dataDirectory + "/response_units.csv"
    );

    if (!file)
        throw std::runtime_error("Cannot save response units.");

    file << "unitID,unitType,locationID,availability,capabilities\n";

    for (const auto& unit : units) {
        if (!unit)
            throw std::runtime_error("Null response unit.");

        file << unit->getUnitID() << ","
             << unit->getUnitType() << ","
             << unit->getLocationID() << ","
             << availabilityToString(unit->getAvailability()) << ","
             << join(unit->getCapabilities())
             << "\n";
    }

    if (!file)
        throw std::runtime_error("Error writing response units.");
}


// ---------------- SAVE ASSIGNMENTS ----------------

void FileManager::saveAssignments(
    const std::vector<Assignment>& assignments
) const {
    std::ofstream file(
        dataDirectory + "/assignments.csv"
    );

    if (!file)
        throw std::runtime_error("Cannot save assignments.");

    file << "assignmentID,incidentID,unitID,"
            "assignedAt,completedAt,status\n";

    for (const auto& assignment : assignments) {
        file << assignment.getAssignmentID() << ","
             << assignment.getIncidentID() << ","
             << assignment.getUnitID() << ","
             << assignment.getAssignedAt() << ","
             << assignment.getCompletedAt() << ","
             << assignmentStatusToString(assignment.getStatus())
             << "\n";
    }

    if (!file)
        throw std::runtime_error("Error writing assignments.");
}


// ---------------- LOAD INCIDENTS ----------------

bool FileManager::loadIncidents(
    std::vector<Incident>& incidents
) const {
    std::ifstream file(
        dataDirectory + "/incidents.csv"
    );

    if (!file)
        return false;

    std::string line;

    if (!std::getline(file, line) ||
        line != "incidentID,type,severity,affectedPopulation,"
                "locationID,status,reportTime,"
                "mandatoryCapabilities,optionalCapabilities") {
        throw std::runtime_error("Invalid incidents CSV header.");
    }

    std::vector<Incident> loaded;
    std::unordered_set<int> ids;

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        auto fields = split(line, ',');
        validateRow(fields, 9);

        int id = parseNumber<int>(fields[0]);
        validateUniqueID(ids, id);

        auto mandatory = fields[7].empty()
            ? std::vector<std::string>{}
            : split(fields[7], '|');

        auto optional = fields[8].empty()
            ? std::vector<std::string>{}
            : split(fields[8], '|');

        loaded.emplace_back(
            id,
            fields[1],
            parseNumber<int>(fields[2]),
            parseNumber<int>(fields[3]),
            parseNumber<int>(fields[4]),
            stringToIncidentStatus(fields[5]),
            parseNumber<std::time_t>(fields[6]),
            mandatory,
            optional
        );
    }

    if (file.bad())
        throw std::runtime_error("Error reading incidents.");

    incidents = std::move(loaded);
    return true;
}


// ---------------- LOAD RESPONSE UNITS ----------------

bool FileManager::loadUnits(
    std::vector<std::unique_ptr<ResponseUnit>>& units
) const {
    std::ifstream file(
        dataDirectory + "/response_units.csv"
    );

    if (!file)
        return false;

    std::string line;

    if (!std::getline(file, line) ||
        line != "unitID,unitType,locationID,availability,capabilities") {
        throw std::runtime_error("Invalid response units CSV header.");
    }

    std::vector<std::unique_ptr<ResponseUnit>> loaded;
    std::unordered_set<int> ids;

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        auto fields = split(line, ',');
        validateRow(fields, 5);

        int id = parseNumber<int>(fields[0]);
        validateUniqueID(ids, id);

        int locationID = parseNumber<int>(fields[2]);

        auto capabilities = fields[4].empty()
            ? std::vector<std::string>{}
            : split(fields[4], '|');

        std::unique_ptr<ResponseUnit> unit;

        if (fields[1] == "Ambulance") {
            unit = std::make_unique<Ambulance>(
                id, locationID, capabilities
            );
        }
        else if (fields[1] == "FireTruck") {
            unit = std::make_unique<FireTruck>(
                id, locationID, capabilities
            );
        }
        else if (fields[1] == "RescueTeam") {
            unit = std::make_unique<RescueTeam>(
                id, locationID, capabilities
            );
        }
        else {
            throw std::runtime_error("Unknown response unit type.");
        }

        unit->setAvailability(
            stringToAvailability(fields[3])
        );

        loaded.push_back(std::move(unit));
    }

    if (file.bad())
        throw std::runtime_error("Error reading response units.");

    units = std::move(loaded);
    return true;
}


// ---------------- LOAD ASSIGNMENTS ----------------

bool FileManager::loadAssignments(
    std::vector<Assignment>& assignments
) const {
    std::ifstream file(
        dataDirectory + "/assignments.csv"
    );

    if (!file)
        return false;

    std::string line;

    if (!std::getline(file, line) ||
        line != "assignmentID,incidentID,unitID,"
                "assignedAt,completedAt,status") {
        throw std::runtime_error("Invalid assignments CSV header.");
    }

    std::vector<Assignment> loaded;
    std::unordered_set<int> ids;

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        auto fields = split(line, ',');
        validateRow(fields, 6);

        int id = parseNumber<int>(fields[0]);
        validateUniqueID(ids, id);

        int incidentID = parseNumber<int>(fields[1]);
        int unitID = parseNumber<int>(fields[2]);

        std::time_t assignedAt =
            parseNumber<std::time_t>(fields[3]);

        std::time_t completedAt =
            parseNumber<std::time_t>(fields[4]);

        Assignment assignment(
            id, incidentID, unitID, assignedAt
        );

        if (fields[5] == "Completed") {
            assignment.complete(completedAt);
        }
        else if (fields[5] == "Cancelled") {
            assignment.cancel();
        }
        else if (fields[5] != "Active") {
            throw std::runtime_error("Invalid assignment status.");
        }

        loaded.push_back(assignment);
    }

    if (file.bad())
        throw std::runtime_error("Error reading assignments.");

    assignments = std::move(loaded);
    return true;
}


// SAVE ALL 

void FileManager::saveAll(
    const std::vector<Incident>& incidents,
    const std::vector<std::unique_ptr<ResponseUnit>>& units,
    const std::vector<Assignment>& assignments
) const {
    saveIncidents(incidents);
    saveUnits(units);
    saveAssignments(assignments);
}


// LOAD ALL 

void FileManager::loadAll(
    std::vector<Incident>& incidents,
    std::vector<std::unique_ptr<ResponseUnit>>& units,
    std::vector<Assignment>& assignments
) const {
    bool incidentsLoaded = loadIncidents(incidents);
    bool unitsLoaded = loadUnits(units);
    bool assignmentsLoaded = loadAssignments(assignments);

    if (incidentsLoaded != unitsLoaded ||
        incidentsLoaded != assignmentsLoaded) {
        throw std::runtime_error(
            "Incomplete application data: one or more CSV files are missing."
        );
    }
}