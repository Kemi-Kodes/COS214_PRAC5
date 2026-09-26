#ifndef CAMPUSEVENT_H
#define CAMPUSEVENT_H

#include <string>

enum class CampusEvent { UnitDispatched, MedicalEmergency, FireSpreading, AllClear };

inline std::string eventName(CampusEvent event)
{
    switch (event)
    {
    case CampusEvent::UnitDispatched: return "UNIT DISPATCHED";
    case CampusEvent::MedicalEmergency: return "MEDICAL EMERGENCY";
    case CampusEvent::FireSpreading: return "FIRE SPREADING";
    case CampusEvent::AllClear: return "ALL CLEAR";
    }
    return "UNKNOWN";
}

#endif
