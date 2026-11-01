
#include <iostream>
#include <cassert>
#include <stdexcept>
#include "Incident.h"

int main() {

    Incident incident(1,"Flood",4,500,2,IncidentStatus::Pending,1000
    );

    assert(incident.getIncidentId() == 1);
    assert(incident.getLocId() == 2);
    assert(incident.getSeverity() == 4);
    assert(incident.getAffPop() == 500);

    double priority = incident.getPriorityScore();
    assert(priority >= 0.0 && priority <= 100.0);


    bool exceptionThrown = false;

    try {
        Incident invalid(2, "Fire", 6, 100, 1,IncidentStatus::Pending, 1000
        );
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    exceptionThrown = false;

    try {
        Incident invalid(3, "Flood", 3, -100, 1,IncidentStatus::Pending, 1000);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Incident tests passed.\n";

    return 0;
}
