#ifndef VECTOR3I_H
#define VECTOR3I_H

#include <cmath>
#include <algorithm>
#include <iostream>

// ============================================================================
// [OOP CONCEPT: Classes, Encapsulation, Operator Overloading & Friend Function]
// Represents a 3D Integer Grid Coordinate for the battlefield.
// ============================================================================
class Vector3i {
private:
    int x;
    int y;
    int z;

public:
    // [OOP CONCEPT: Constructor with Default Arguments]
    Vector3i(int xVal = 0, int yVal = 0, int zVal = 0) : x(xVal), y(yVal), z(zVal) {}

    // Getters (Data Abstraction & Encapsulation)
    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }

    // [OOP CONCEPT: Operator Overloading (Compile-Time Polymorphism)]
    bool operator==(const Vector3i& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    bool operator!=(const Vector3i& other) const {
        return !(*this == other);
    }

    Vector3i operator+(const Vector3i& other) const {
        return Vector3i(x + other.x, y + other.y, z + other.z);
    }

    Vector3i operator-(const Vector3i& other) const {
        return Vector3i(x - other.x, y - other.y, z - other.z);
    }

    // Grid distance calculation (Chebyshev / 3D King's distance)
    int distanceTo(const Vector3i& other) const {
        int dx = std::abs(x - other.x);
        int dy = std::abs(y - other.y);
        int dz = std::abs(z - other.z);
        return std::max(dx, std::max(dy, dz));
    }

    // Check if directly adjacent (within 1 tile)
    bool isAdjacent(const Vector3i& other) const {
        if (*this == other) return false;
        return distanceTo(other) == 1;
    }

    // [OOP CONCEPT: Friend Function for Stream Insertion]
    friend std::ostream& operator<<(std::ostream& os, const Vector3i& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

#endif // VECTOR3I_H
