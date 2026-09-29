#ifndef FIELDTEAM_H
#define FIELDTEAM_H

#include "ResponseComponent.h"

class FieldTeam : public ResponseComponent {
    public:
    explicit FieldTeam(const std::string &name);
    ~FieldTeam() override;
    bool dispatchTo(const std::string &location);
    bool declareAllClear(const std::string &location);
    void standDown();
    bool isBusy() const;
    std::string getLocation() const;
    std::string describeStatus() const override;

    private:
    bool busy;
    std::string location;
};

#endif /* FIELDTEAM_H */
