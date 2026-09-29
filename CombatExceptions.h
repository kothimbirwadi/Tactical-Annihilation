#ifndef COMBAT_EXCEPTIONS_H
#define COMBAT_EXCEPTIONS_H

#include <exception>
#include <string>

// ============================================================================
// [OOP CONCEPT: Exception Handling & Inheritance]
// Custom exception hierarchy for combat error management.
// Demonstrates: Base Exception class, Derived Exception classes, overriding what()
// ============================================================================

class CombatException : public std::exception {
protected:
    std::string message;
public:
    explicit CombatException(const std::string& msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

// Thrown when an operative tries to spend more than 3 AP in a single turn
class OutOfAPException : public CombatException {
public:
    explicit OutOfAPException(const std::string& unitName, int attempted, int used, int maxAP)
        : CombatException("[AP Cap Error] " + unitName + " attempted to use " + 
                          std::to_string(attempted) + " AP, but already used " + 
                          std::to_string(used) + "/" + std::to_string(maxAP) + " AP this turn!") {}
};

// Thrown when a target is beyond an operative's attack range
class OutOfRangeException : public CombatException {
public:
    explicit OutOfRangeException(const std::string& unitName, const std::string& targetName, int distance, int maxRange)
        : CombatException("[Range Error] " + targetName + " is " + std::to_string(distance) + 
                          " tiles away from " + unitName + " (Max Range: " + std::to_string(maxRange) + ")!") {}
};

// Thrown when trying to attack an invalid target (e.g. already dead or friendly unit)
class InvalidTargetException : public CombatException {
public:
    explicit InvalidTargetException(const std::string& reason)
        : CombatException("[Target Error] " + reason) {}
};

#endif // COMBAT_EXCEPTIONS_H
