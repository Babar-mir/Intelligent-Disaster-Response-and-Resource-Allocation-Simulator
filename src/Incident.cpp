#include "Incident.h"
#include<stdexcept>
#include<algorithm>


Incident::Incident(int id, const std::string& ty, int s, int a, int l , IncidentStatus st, std::time_t t,
    const std::vector<std::string>& mandatory,
    const std::vector<std::string>& optional):
    incidentId(id),type(ty),severity(s),affPop(a),locID(l),status(st),reportTime(t),
    mandatoryCapabilities(mandatory),
    optionalCapabilities(optional) {
    if (incidentId <= 0) {
        throw std::invalid_argument("Incident ID must be positive.");
    }

    if (severity < 1 || severity > 5) {
        throw std::invalid_argument("Severity must be between 1 and 5.");
    }

    if (affPop < 0) {
        throw std::invalid_argument(
            "Affected population cannot be negative."
        );
    }
}

int Incident::getIncidentId()const{
    return incidentId;
}

std::string Incident::getType() const {
    return type;
}

int Incident::getSeverity() const {
    return severity;
}

int Incident::getAffPop() const {
    return affPop;
}

int Incident::getLocId() const {
    return locID;
}

IncidentStatus Incident::getStatus() const {
    return status;
}

std::time_t Incident::getReportTime() const {
    return reportTime;
}


double Incident::getNormalizedSeverity() const{
    return ((severity-1)/4.0)*100.0;
}
void Incident::setStatus(IncidentStatus newStatus){
    status=newStatus;
}

double Incident::getNormalizedAffPop() const {
    constexpr double MAX_REFERENCE_POPULATION = 1000.0;

    return std::min(static_cast<double>(affPop) / MAX_REFERENCE_POPULATION * 100.0,
        100.0);   
}

double Incident::getPriorityScore() const {
    return 0.5 * getNormalizedSeverity()
         + 0.5 * getNormalizedAffPop();
}


const std::vector<std::string>&
Incident::getMandatoryCapabilities() const {
    return mandatoryCapabilities;
}

const std::vector<std::string>&
Incident::getOptionalCapabilities() const {
    return optionalCapabilities;
}