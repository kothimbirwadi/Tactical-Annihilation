#ifndef COMBAT_LOGGER_H
#define COMBAT_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// ============================================================================
// [OOP CONCEPT: File Handling (Unit 5) & Generic Programming / Templates (Unit 6)]
// Logs battle actions to an external file using std::ofstream and std::ifstream.
// ============================================================================
class CombatLogger {
private:
    std::string filename;
    std::ofstream logFile;

public:
    // Constructor opens file in write mode
    explicit CombatLogger(const std::string& fname = "combat_replay_log.txt") 
        : filename(fname) {
        // [OOP CONCEPT: Opening File with File Mode]
        logFile.open(filename, std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            std::cerr << "[File Error] Could not open file: " << filename << " for writing!\n";
        } else {
            logFile << "========================================================\n";
            logFile << "   TACTICAL ANNIHILATION - MATCH COMBAT REPLAY LOG\n";
            logFile << "========================================================\n\n";
        }
    }

    // Destructor ensures file is cleanly closed
    ~CombatLogger() {
        if (logFile.is_open()) {
            logFile << "\n--- Match Session Concluded ---\n";
            logFile.close();
            std::cout << "[File Handling] Match log successfully saved to: " << filename << "\n";
        }
    }

    // Write a sequential combat log line
    void logEvent(const std::string& eventText) {
        if (logFile.is_open()) {
            logFile << "[EVENT] " << eventText << "\n";
            logFile.flush();
        }
    }

    // ========================================================================
    // [OOP CONCEPT: Generic Programming / Function Template (Unit 6)]
    // Logs any data type (int, float, string, Vector3i, etc.) generically
    // ========================================================================
    template <typename T>
    void logMetric(const std::string& label, const T& value) {
        if (logFile.is_open()) {
            logFile << "  [METRIC] " << label << ": " << value << "\n";
            logFile.flush();
        }
    }

    // [OOP CONCEPT: Reading from File using std::ifstream]
    void displayLogSummary() {
        std::ifstream readFile(filename);
        if (!readFile.is_open()) {
            std::cerr << "[File Error] Could not open file: " << filename << " for reading!\n";
            return;
        }

        std::cout << "\n--------------------------------------------------------\n";
        std::cout << " [FILE I/O] Reading Replay Log Back from: " << filename << "\n";
        std::cout << "--------------------------------------------------------\n";

        std::string line;
        int lineCount = 0;
        while (std::getline(readFile, line) && lineCount < 10) {
            std::cout << "  " << line << "\n";
            lineCount++;
        }
        std::cout << "  ... (log continues in file)\n";
        readFile.close();
    }
};

#endif // COMBAT_LOGGER_H
