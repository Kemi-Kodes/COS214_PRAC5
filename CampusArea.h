#ifndef CAMPUSAREA_H
#define CAMPUSAREA_H

#include <string>


class CampusArea {
public:
    virtual ~CampusArea() = default;

  
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void restrict() = 0; 

    virtual std::string getName() const = 0;

  
    virtual void display(int depth = 0) const = 0;
};

#endif 
