
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include "Incident.h"
#include "ResponseUnit.h"
#include "Ambulance.h"
#include "FireTruck.h"
#include "RescueTeam.h"
#include "Assignment.h"
#include "RoadNetwork.h"
#include "ResourceAllocator.h"
#include "FileManager.h"

using namespace std;

// INPUT HELPERS 

int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        try {
            size_t pos;
            int value = stoi(input, &pos);
            if (pos == input.size())
                return value;
        }
        catch (...) {}
        cout << "Invalid input. Enter an integer.\n";
    }
}

string readString(const string& prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

vector<string> readCapabilities(const string& prompt) {
    cout << prompt;
    cout << "\nSeparate capabilities using | : ";
    string input;
    getline(cin, input);
    vector<string> capabilities;
    stringstream stream(input);
    string capability;
    while (getline(stream, capability, '|')) {
        if (!capability.empty())
            capabilities.push_back(capability);
    }

    return capabilities;
}

// STATUS CONVERSIONS 

string incidentStatusToString(IncidentStatus status) {
    switch (status) {
        case IncidentStatus::Pending:
            return "Pending";
        case IncidentStatus::Assigned:
            return "Assigned";
        case IncidentStatus::Resolved:
            return "Resolved";
    }
    return "Unknown";
}

string assignmentStatusToString(AssignmentStatus status) {
    switch (status) {
        case AssignmentStatus::Active:
            return "Active";
        case AssignmentStatus::Completed:
            return "Completed";
        case AssignmentStatus::Cancelled:
            return "Cancelled";
    }

    return "Unknown";
}

// ROAD NETWORK

RoadNetwork buildRoadNetwork() {
    RoadNetwork network;
    for (int i = 1; i <= 5; i++)
        network.addLocation(i);
    network.addRoad(1, 2, 4);
    network.addRoad(1, 3, 2);
    network.addRoad(2, 3, 3);
    network.addRoad(2, 4, 6);
    network.addRoad(3, 4, 3);
    network.addRoad(4, 5, 2);

    return network;
}

void displayRoadNetwork(const RoadNetwork& network) {
    cout << "\nLocations available: 1, 2, 3, 4, 5\n";
    cout << "Road network is initialized with fixed roads.\n";
    cout << "Use these location IDs when registering incidents "
            "and units.\n";
}

// INCIDENT MANAGEMENT

void registerIncident(vector<Incident>& incidents) {
    int id = 1;
    for (const auto& incident : incidents)
        id = max(id, incident.getIncidentId() + 1);
    string type = readString("Incident type: ");
    int severity = readInt("Severity (1-5): ");
    int population = readInt("Affected population: ");
    int location = readInt("Location ID (1-5): ");
    auto mandatory =readCapabilities("Mandatory capabilities");
    auto optional =readCapabilities("Optional capabilities");
    incidents.emplace_back(id,type,severity,population,location,IncidentStatus::Pending,time(nullptr),
        mandatory,
        optional
    );
    cout << "\nIncident registered successfully.\n";
    cout << "Incident ID: " << id << '\n';
}

void displayIncidents(const vector<Incident>& incidents) {
    cout << "\nINCIDENTS\n";
    if (incidents.empty()) {
        cout << "No incidents registered.\n";
        return;
    }
    for (const auto& incident : incidents) {
        cout << "\nIncident ID: "<< incident.getIncidentId();
        cout << "\nType: " << incident.getType();
        cout << "\nSeverity: "<< incident.getSeverity();
        cout << "\nAffected population: "<< incident.getAffPop();
        cout << "\nLocation: "<< incident.getLocId();
        cout << "\nPriority score: "<< incident.getPriorityScore();
        cout << "\nStatus: "<< incidentStatusToString(incident.getStatus());
        cout << "\n-----------------------------\n";
    }
}

// RESPONSE UNIT MANAGEMENT 

void registerUnit(
    vector<unique_ptr<ResponseUnit>>& units) {
    int id = 1;
    for (const auto& unit : units)
        id = max(id, unit->getUnitID() + 1);
    cout << "\n1. Ambulance\n";
    cout << "2. FireTruck\n";
    cout << "3. RescueTeam\n";
    int choice = readInt("Select unit type: ");
    int location = readInt("Location ID (1-5): ");
    auto capabilities =readCapabilities("Unit capabilities");
    unique_ptr<ResponseUnit> unit;
    switch (choice) {
        case 1:
            unit = make_unique<Ambulance>(id, location, capabilities);
            break;
        case 2:unit = make_unique<FireTruck>(id, location, capabilities);
            break;
        case 3:unit = make_unique<RescueTeam>(id, location, capabilities);
            break;
        default:cout << "Invalid unit type.\n";
            return;
    }
    units.push_back(move(unit));
    cout << "\nResponse unit registered successfully.\n";
    cout << "Unit ID: " << id << '\n';
}

void displayUnits(
    const vector<unique_ptr<ResponseUnit>>& units) {
    cout << "\nRESPONSE UNITS \n";
    if (units.empty()) {
        cout << "No response units registered.\n";
        return;
    }
    for (const auto& unit : units) {
        cout << "\nUnit ID: " << unit->getUnitID();
        cout << "\nType: " << unit->getUnitType();
        cout << "\nLocation: " << unit->getLocationID();
        cout << "\nAvailability: "<< (unit->getAvailability() ==UnitAvailability::Available? "Available" : "Busy");
        cout << "\nCapabilities: ";
        for (const auto& capability : unit->getCapabilities())
            cout << capability << " ";
        cout << "\n-----------------------------\n";
    }
}
// RESOURCE ALLOCATION 
void allocateResources(
    vector<Incident>& incidents,
    vector<unique_ptr<ResponseUnit>>& units,
    vector<Assignment>& assignments,
    ResourceAllocator& allocator,
    const RoadNetwork& network) {
    vector<ResponseUnit*> unitPointers;
    for (const auto& unit : units)unitPointers.push_back(unit.get());
    auto newAssignments = allocator.allocateResources(incidents,unitPointers,network
    );
    if (newAssignments.empty()) {
        cout << "\nNo new assignments could be made.\n";
        return;
    }
    for (const auto& assignment : newAssignments) {
        cout << "\nAssignment created successfully.\n";
        cout << "Assignment ID: "<< assignment.getAssignmentID();
        cout << "\nIncident ID: "<< assignment.getIncidentID();
        cout << "\nUnit ID: "<< assignment.getUnitID() << '\n';
        // Display route for the selected unit.
        auto unit = find_if(units.begin(),units.end(),[&](const auto& ptr) {
                return ptr->getUnitID() ==
                       assignment.getUnitID();
            }
        );
        auto incident = find_if(
            incidents.begin(),
            incidents.end(),
            [&](const Incident& item) {
                return item.getIncidentId() ==
                       assignment.getIncidentID();
            }
        );
        if (unit != units.end() && incident != incidents.end()) {
            PathResult route = network.findShortestPath((*unit)->getLocationID(),incident->getLocId());
            cout << "Distance: " << route.distance << " km\n";
            cout << "Route: ";
            for (int location : route.path)
                cout << location << " ";cout << '\n';
        }
        assignments.push_back(assignment);
    }
}

// ASSIGNMENT HISTORY 

void displayAssignments(
    const vector<Assignment>& assignments) {
    cout << "\nASSIGNMENT HISTORY \n";
    if (assignments.empty()) {
        cout << "No assignment history available.\n";
        return;
    }
    for (const auto& assignment : assignments) {
        cout << "\nAssignment ID: "<< assignment.getAssignmentID();
        cout << "\nIncident ID: "<< assignment.getIncidentID();
        cout << "\nUnit ID: "<< assignment.getUnitID();
        cout << "\nStatus: "<< assignmentStatusToString(assignment.getStatus());
        cout << "\n-----------------------------\n";
    }
}

// COMPLETE ASSIGNMENT 

void completeAssignment(
    vector<Assignment>& assignments,
    vector<Incident>& incidents,
    vector<unique_ptr<ResponseUnit>>& units) {
    int id = readInt("Enter active assignment ID: ");

    auto assignment = find_if(assignments.begin(),assignments.end(),[&](const Assignment& item) {
            return item.getAssignmentID() == id;
        }
    );

    if (assignment == assignments.end() ||
        assignment->getStatus() != AssignmentStatus::Active) {
        cout << "Active assignment not found.\n";
        return;
    }

    auto incident = find_if(incidents.begin(),incidents.end(),[&](const Incident& item) {
            return item.getIncidentId() ==
                   assignment->getIncidentID();
        }
    );

    auto unit = find_if(
        units.begin(),
        units.end(),
        [&](const auto& ptr) {
            return ptr->getUnitID() ==
                   assignment->getUnitID();
        }
    );

    if (incident == incidents.end() ||
        unit == units.end()) {
        cout << "Assignment references missing data.\n";
        return;
    }
    assignment->complete(time(nullptr));
    incident->setStatus(IncidentStatus::Resolved);
    (*unit)->setAvailability(UnitAvailability::Available);
    (*unit)->setLocationID(incident->getLocId());
    cout << "Assignment completed successfully.\n";
}

//MAIN 

int main() {
    try {
        FileManager fileManager("data");
        vector<Incident> incidents;
        vector<unique_ptr<ResponseUnit>> units;
        vector<Assignment> assignments;
        // First-run initialization.
        bool hasIncidents =
    filesystem::exists("data/incidents.csv");

    bool hasUnits =filesystem::exists("data/response_units.csv");
    bool hasAssignments =filesystem::exists("data/assignments.csv");
    bool allMissing =!hasIncidents && !hasUnits && !hasAssignments;
    bool allEmpty =
        hasIncidents && hasUnits && hasAssignments &&
        filesystem::file_size("data/incidents.csv") == 0 &&
        filesystem::file_size("data/response_units.csv") == 0 &&
        filesystem::file_size("data/assignments.csv") == 0;

    if (allMissing || allEmpty) {fileManager.saveAll(incidents, units,assignments);
    }
        fileManager.loadAll(incidents,units,assignments);
        ResourceAllocator allocator;
        int nextAssignmentID = 1;
        for (const auto& assignment : assignments) {
            nextAssignmentID = max(nextAssignmentID,assignment.getAssignmentID() + 1);
        }
        allocator.setNextAssignmentID(nextAssignmentID);
        RoadNetwork network = buildRoadNetwork();
        int choice;
        do {
            cout << "\n DISASTER RESPONSE MANAGEMENT SYSTEM\n";
            cout << "= = = = = = =  = = = = = =  = = =\n";

            cout << "1. Report New Incident\n";
            cout << "2. Register Response Unit\n";
            cout << "3. View All Incidents\n";
            cout << "4. View All Response Units\n";
            cout << "5. Allocate Resources\n";
            cout << "6. View Assignment History\n";
            cout << "7. Complete Assignment\n";
            cout << "8. View Road Network\n";
            cout << "9. Exit\n";
            choice = readInt("\nEnter your choice: ");
            switch (choice) {
                case 1:registerIncident(incidents);
                    break;
                  case 2:registerUnit(units);
                    break;
                case 3:displayIncidents(incidents);
                    break;
                case 4:displayUnits(units);
                    break;
                case 5:allocateResources(incidents,units,assignments, allocator,network);
                    break;
                case 6:displayAssignments(assignments);
                    break;
                case 7:completeAssignment(assignments,incidents,units);
                    break;
                case 8:
                    displayRoadNetwork(network);
                    break;
                case 9: cout << "Exiting application...\n";
                    break;
                default:cout << "Invalid menu choice.\n";
            }

            // Persist changes after every operation.
            fileManager.saveAll(incidents,units,assignments);
        } while (choice != 9);
    }
    catch (const exception& error) {
        cerr << "\nApplication error: "
             << error.what() << '\n';
        return 1;
    }
    return 0;
}
