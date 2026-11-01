# Intelligent Disaster Response and Resource Allocation Simulator (IDRRAS)

A C++17 console-based disaster management simulator that allocates emergency response units to incidents based on severity, affected population, unit availability, capability compatibility, and shortest travel distance.

## 1. Problem Statement

During disasters such as floods, fires, and medical emergencies, response resources must be allocated efficiently. Assigning an inappropriate or distant response unit can delay emergency assistance.

IDRRAS simulates a disaster response management system that prioritizes incidents and assigns suitable response units using capability matching and Dijkstra's shortest-path algorithm.

## 2. Features

- Register and manage disaster incidents.
- Register different response unit types:
  - Ambulance
  - FireTruck
  - RescueTeam
- Assign mandatory and optional capabilities to incidents and units.
- Calculate incident priority using severity and affected population.
- Allocate the nearest available and compatible response unit.
- Calculate shortest routes using Dijkstra's algorithm.
- Track assignment history and unit availability.
- Complete assignments and update incident and unit states.
- Store and retrieve application data using CSV files.

## 3. Technology Stack

- Language: C++17
- Standard Library: STL
- Data Structures: Vectors, unordered maps, priority queues
- Algorithm: Dijkstra's shortest-path algorithm
- Persistence: CSV file handling
- Build: GCC / G++

## 4. Project Structure

```text
IDRRAS/
│
├── data/
│   ├── incidents.csv
│   ├── response_units.csv
│   └── assignments.csv
│
├── include/
│   ├── Incident.h
│   ├── ResponseUnit.h
│   ├── Ambulance.h
│   ├── FireTruck.h
│   ├── RescueTeam.h
│   ├── Assignment.h
│   ├── RoadNetwork.h
│   ├── ResourceAllocator.h
│   └── FileManager.h
│
├── src/
│   ├── main.cpp
│   ├── Incident.cpp
│   ├── ResponseUnit.cpp
│   ├── Ambulance.cpp
│   ├── FireTruck.cpp
│   ├── RescueTeam.cpp
│   ├── Assignment.cpp
│   ├── RoadNetwork.cpp
│   ├── ResourceAllocator.cpp
│   └── FileManager.cpp
│
├── tests/
│   ├── test_incident.cpp
│   ├── test_road_network.cpp
│   ├── test_resource_allocator.cpp
│   ├── test_persistence.cpp
│   └── test_integration.cpp
│
├── .gitignore
└── README.md
```

## 5. Object-Oriented Programming Concepts

### Encapsulation

Classes maintain their data members privately and expose controlled access through public methods.

Examples: `Incident`, `Assignment`, and `ResponseUnit`.

### Abstraction

`ResponseUnit` is an abstract base class that defines a common interface for all response units.

### Inheritance

`Ambulance`, `FireTruck`, and `RescueTeam` inherit from `ResponseUnit`.

### Polymorphism

Virtual functions such as `getUnitType()` and `canHandle()` allow different response unit types to provide their own implementations.

The allocator works with base-class pointers instead of requiring separate allocation logic for each derived class.

## 6. Core Algorithms

### Incident Priority Calculation

The priority score combines normalized severity and affected population:

\[
S = \frac{Severity-1}{4}\times100
\]

\[
A = \min\left(\frac{Population}{1000}\times100,100\right)
\]

\[
Priority = 0.5S + 0.5A
\]

Incidents are processed in descending priority order. Report time is used as a tie-breaker.

### Capability Matching

A response unit must satisfy all mandatory capabilities.

For optional capabilities, at least 75% must match.

### Resource Allocation

For each pending incident:

1. Identify available response units.
2. Filter units using capability compatibility.
3. Calculate shortest travel distances.
4. Select the nearest suitable unit.
5. Create an assignment.
6. Mark the incident as assigned and the unit as busy.

### Dijkstra's Algorithm

The road network is represented using an adjacency list.

Dijkstra's algorithm calculates the shortest distance between two locations using a minimum-priority queue.

Time complexity with an adjacency list and binary heap:

\[
O((V+E)\log V)
\]

where \(V\) is the number of locations and \(E\) is the number of roads.

## 7. Unit Lifecycle

The response unit follows this lifecycle:

```text
Available
    |
    | Assignment
    v
Busy
    |
    | Assignment Completed
    v
Available
```

After completing an assignment, the unit becomes available again and its location is updated to the incident location.

## 8. Data Persistence

The application uses three CSV files:

| File               | Stored information                          |
| ------------------ | ------------------------------------------- |
| incidents.csv      | Incident details and status                 |
| response_units.csv | Unit details, capabilities and availability |
| assignments.csv    | Assignment history and timestamps           |

The `FileManager` class handles saving and loading application data.

## 9. Compilation and Execution

Requirements:

- GCC or G++ compiler supporting C++17.
- Windows, Linux, or another compatible environment.

Compile from the project root:

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/Incident.cpp src/ResponseUnit.cpp src/Ambulance.cpp src/FireTruck.cpp src/RescueTeam.cpp src/Assignment.cpp src/RoadNetwork.cpp src/ResourceAllocator.cpp src/FileManager.cpp -o simulator
```

On Windows PowerShell:

```powershell
.\simulator.exe
```

## 10. Testing

The project includes five test programs:

| Test file                   | Purpose                            |
| --------------------------- | ---------------------------------- |
| test_incident.cpp           | Incident validation and priority   |
| test_road_network.cpp       | Shortest paths and invalid roads   |
| test_resource_allocator.cpp | Allocation logic and compatibility |
| test_persistence.cpp        | CSV saving and loading             |
| test_integration.cpp        | End-to-end system workflow         |

Example compilation:

```bash
g++ -std=c++17 -Iinclude tests/test_incident.cpp src/Incident.cpp -o test_incident
```

Each test program can be compiled and executed independently.

## 11. Current Limitations

- The road network is initialized with a fixed demonstration topology.
- CSV parsing has limited support for complex quoted fields.
- Saving multiple CSV files is not transactional.
- Resource allocation uses a greedy nearest-compatible-unit strategy rather than a globally optimal assignment algorithm.
- The application is console-based and does not provide a graphical interface.
- The simulator does not incorporate live GPS, traffic conditions, or real-time disaster data.

## 12. Future Improvements

- Dynamic road network management.
- Real-time traffic and location integration.
- More advanced multi-resource allocation algorithms.
- Database integration.
- Graphical dashboard for monitoring incidents and response units.
- More comprehensive automated testing and error recovery.

---

**Project:** Intelligent Disaster Response and Resource Allocation Simulator  
**Language:** C++17  
**Category:** Object-Oriented Programming / Disaster Management Simulation
