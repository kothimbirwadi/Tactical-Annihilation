#ifndef VECTOR3I_H
#define VECTOR3I_H

#include <cmath>
#include <algorithm>
#include <iostream>

/**
 * @brief Simple 3D Integer Vector for Grid-based calculations (Godot Vector3i equivalent).
 */
struct Vector3i {
    int x;
    int y;
    int z;

    Vector3i(int px = 0, int py = 0, int pz = 0) : x(px), y(py), z(pz) {}

    bool operator==(const Vector3i& other) const {
        return x == other.x && y == other.y && z == other.z;
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

    // Grid distance (Chebyshev / 3D King's distance: maximum coordinate difference)
    int distanceTo(const Vector3i& other) const {
        int dx = std::abs(x - other.x);
        int dy = std::abs(y - other.y);
        int dz = std::abs(z - other.z);
        return std::max(dx, std::max(dy, dz));
    }

    // Check if another tile is directly adjacent (distance == 1)
    bool isAdjacent(const Vector3i& other) const {
        if (*this == other) return false;
        return distanceTo(other) == 1;
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

#endif // VECTOR3I_H
