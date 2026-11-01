#pragma once

#include<ctime>
#include<string>
#include <vector>


enum class IncidentStatus{
    Pending,
    Assigned,
    Resolved
};

class Incident{
    int incidentId;
    std::string type;
    int severity,affPop;
    int locID;
    IncidentStatus status;
    std::time_t reportTime;
    std::vector<std::string> mandatoryCapabilities;
    std::vector<std::string> optionalCapabilities;

    public:
    Incident(int id, const std::string& ty, int s, int a, int l , IncidentStatus st, std::time_t t,
        const std::vector<std::string>& mandatory = {},
        const std::vector<std::string>& optional = {});
    int getIncidentId() const;
    std::string getType() const;
    int getSeverity() const;
    int getAffPop() const;
    int getLocId() const;
    IncidentStatus getStatus() const;
    std::time_t getReportTime() const;

    void setStatus(IncidentStatus newStatus);

    double getNormalizedSeverity() const;
    double getNormalizedAffPop() const;
    double getPriorityScore() const;

    const std::vector<std::string>& getMandatoryCapabilities() const;

    const std::vector<std::string>& getOptionalCapabilities() const;






    
};