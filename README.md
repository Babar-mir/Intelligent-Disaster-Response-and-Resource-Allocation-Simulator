# Intelligent Disaster Response & Resource Allocation Simulator

> A C++-based object-oriented simulation system for managing emergency response resources during multiple disasters.

## Overview

The **Intelligent Disaster Response & Resource Allocation Simulator** models a disaster-response environment in which multiple emergencies may occur across different locations while emergency resources are limited.

The system simulates how ambulances, fire trucks, and rescue teams are selected and dispatched based on factors such as **disaster severity, affected population, resource availability, and route distance**.

The project is designed as an **OOP-focused C++ application**, with algorithms and data structures integrated into the object-oriented design.

## Objectives

- Model real-world disaster-response entities using C++ classes.
- Demonstrate core Object-Oriented Programming concepts.
- Allocate limited emergency resources efficiently.
- Prioritize multiple simultaneous emergencies.
- Find efficient routes between locations.
- Simulate dynamic emergency events.
- Generate response and resource-utilization reports.

## Key Features

### Disaster Management
- Create and manage different disaster events.
- Store disaster type, severity, location, and affected population.
- Track active, resolved, and pending emergencies.
- Support multiple simultaneous disasters.

### Emergency Response Units
The simulator supports different types of response units:

- Ambulance
- Fire Truck
- Rescue Team

Each unit maintains information such as:

- Unit ID
- Current location
- Availability
- Unit type
- Assignment status

### Intelligent Resource Allocation
Resources are allocated based on:

- Disaster priority
- Unit availability
- Unit compatibility
- Distance from the disaster
- Number of affected people

### Priority-Based Emergency Handling
When several disasters occur, the system prioritizes emergencies according to their severity and impact.

A priority queue can be used to ensure that critical emergencies are processed first.

### Route Optimization
Locations and roads are represented as a graph.

**Dijkstra's algorithm** can be used to determine the shortest route between a response unit and a disaster location.

### Dynamic Simulation
The simulation can model events such as:

- New disaster occurrence
- Emergency completion
- Response-unit recovery
- Resource shortage
- Road blockage
- Higher-priority emergency
- Resource reallocation

### Reporting
The system can generate statistics such as:

- Total disasters
- Resolved emergencies
- Pending emergencies
- Units dispatched
- Average response time
- Resource utilization

---

## OOP Concepts Demonstrated

The project is intentionally designed around C++ OOP principles.

| Concept | Application |
|---|---|
| **Classes & Objects** | Disaster, Location, ResponseUnit, Dispatcher, etc. |
| **Encapsulation** | Controlled access to object data |
| **Inheritance** | Ambulance, FireTruck, and RescueTeam derived from ResponseUnit |
| **Polymorphism** | Different response units implement common virtual operations |
| **Abstraction** | Allocation and simulation logic exposed through high-level interfaces |
| **Composition** | SimulationEngine contains and coordinates system components |
| **Constructors / Destructors** | Object lifecycle management |
| **Exception Handling** | Invalid input and invalid operations |
| **STL** | `vector`, `priority_queue`, `unordered_map`, etc. |

## System Architecture

```text
                         SimulationEngine
                                |
                ┌───────────────┴───────────────┐
                |                               |
           Dispatcher                    ResourceAllocator
                |                               |
                └───────────────┬───────────────┘
                                |
                         ResponseUnit
                    ┌───────────┼───────────┐
                    |           |           |
                Ambulance   FireTruck   RescueTeam

        Disaster          Location          RoadNetwork
           |                  |                  |
     ┌─────┼─────┐      ┌─────┼─────┐       Dijkstra
     |     |     |      |     |     |
   Fire  Flood  Earthquake  Hospital Shelter Zone
```

## Main Components

### `Disaster`
Represents an emergency event.

Stores information such as:

- Disaster ID
- Disaster type
- Location
- Severity
- Affected population
- Current status

### `Location`
Represents a physical location in the simulated environment.

### `ResponseUnit`
Abstract/base class for emergency response units.

Derived classes:

- `Ambulance`
- `FireTruck`
- `RescueTeam`

### `ResourceAllocator`
Determines which available resources should be assigned to a disaster.

### `Dispatcher`
Handles the actual assignment and status changes of response units.

### `RoadNetwork`
Represents roads and connections between locations and provides route calculation.

### `SimulationEngine`
Coordinates disasters, resources, allocation, dispatching, and simulation events.

### `ReportGenerator`
Produces system statistics and performance reports.

---

## Algorithms & Data Structures

The project can integrate the following:

- **Priority Queue** — emergency prioritization
- **Graph** — road-network representation
- **Dijkstra's Algorithm** — shortest-path calculation
- **Vectors / Maps / Hash Maps** — object and resource storage
- **Sorting and Searching** — resource selection and reporting

## Example Workflow

```text
Disaster Detected
       ↓
Calculate Emergency Priority
       ↓
Determine Required Resources
       ↓
Find Available Compatible Units
       ↓
Calculate Efficient Routes
       ↓
Allocate Resources
       ↓
Dispatch Units
       ↓
Update Unit Status
       ↓
Resolve Emergency
       ↓
Generate Response Statistics
```

## Example Simulation

```text
========================================
     DISASTER RESPONSE SIMULATOR
========================================

New Emergency
Type              : Earthquake
Location          : Zone C
Severity          : 9/10
Affected People   : 1200

Required Resources:
Ambulances        : 4
Rescue Teams      : 3
Fire Trucks       : 1

Searching for available units...
Calculating routes...
Allocating resources...

A04  -> Zone C
A07  -> Zone C
A11  -> Zone C
R02  -> Zone C
R05  -> Zone C
R08  -> Zone C
F03  -> Zone C

Resources dispatched successfully.
```

## Project Structure

```text
disaster-response-simulator/
│
├── include/
│   ├── Disaster.h
│   ├── Location.h
│   ├── ResponseUnit.h
│   ├── Ambulance.h
│   ├── FireTruck.h
│   ├── RescueTeam.h
│   ├── ResourceAllocator.h
│   ├── Dispatcher.h
│   ├── RoadNetwork.h
│   ├── SimulationEngine.h
│   └── ReportGenerator.h
│
├── src/
│   ├── Disaster.cpp
│   ├── Location.cpp
│   ├── ResponseUnit.cpp
│   ├── Ambulance.cpp
│   ├── FireTruck.cpp
│   ├── RescueTeam.cpp
│   ├── ResourceAllocator.cpp
│   ├── Dispatcher.cpp
│   ├── RoadNetwork.cpp
│   ├── SimulationEngine.cpp
│   └── ReportGenerator.cpp
│
├── data/
│   └── sample_data.txt
│
├── tests/
│   └── test_cases.cpp
│
├── main.cpp
├── CMakeLists.txt
└── README.md
```

## Technology Stack

- **Language:** C++
- **Standard:** C++17 / C++20
- **Standard Library:** STL
- **Build System:** CMake
- **Version Control:** Git & GitHub

No external database or Python dependency is required for the core project.

## Getting Started

### Prerequisites

Install:

- A C++17/C++20 compatible compiler
- CMake
- Git

### Clone the Repository

```bash
git clone <your-repository-url>
cd disaster-response-simulator
```

### Build with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

On Linux/macOS:

```bash
./disaster_response_simulator
```

On Windows:

```bash
disaster_response_simulator.exe
```

## Future Enhancements

Possible extensions include:

- Graphical user interface using Qt
- Interactive map visualization
- More sophisticated resource-allocation strategies
- Weather and environmental events
- Real-world geographic data
- Multi-city simulation
- Machine-learning-assisted resource allocation
- Real-time emergency simulation

## Project Scope

This project is primarily intended to demonstrate:

**Object-Oriented Programming + Data Structures + Algorithms + Simulation**

The core implementation remains in C++ so that the OOP architecture and design decisions are clearly visible.

## Author

**Babar Mohiudin Mir**

B.Tech Computer Science & Engineering  
IIIT Bhubaneswar

## License

This project is developed as an academic project for educational purposes.
