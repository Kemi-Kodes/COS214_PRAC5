#ifndef CAMPUSRESPONSECOORDINATOR_H
#define CAMPUSRESPONSECOORDINATOR_H

#include "ResponseCoordinator.h"
#include "SecurityTeam.h"
#include "MedicalTeam.h"
#include "FacilitiesTeam.h"
#include "CommsService.h"

class CampusResponseCoordinator : public ResponseCoordinator
{
public:
    explicit CampusResponseCoordinator(AlertService* alerts);
    ~CampusResponseCoordinator() override;
    void notify(ResponseComponent* sender, CampusEvent event, const std::string& location) override;
    void printStatus() const;

    SecurityTeam* getSecurity() const;
    MedicalTeam* getMedical() const;
    FacilitiesTeam* getFacilities() const;
    CommsService* getComms() const;

    CampusResponseCoordinator(const CampusResponseCoordinator&) = delete;
    CampusResponseCoordinator& operator=(const CampusResponseCoordinator&) = delete;

private:
    void onUnitDispatched(ResponseComponent* sender, const std::string& location);
    void onMedicalEmergency(const std::string& location);
    void onFireSpreading(const std::string& location);
    void onAllClear(const std::string& location);

    SecurityTeam* security;
    MedicalTeam* medical;
    FacilitiesTeam* facilities;
    CommsService* comms;
};

#endif
