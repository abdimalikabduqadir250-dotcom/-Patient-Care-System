#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include "Patient.h"
#include "TriageHeap.h"

using namespace std;


unordered_map<int, Patient> patientRecords;
TriageHeap triage;


void saveToFile() {
    ofstream out("patients.txt");
    for (auto const& pair : patientRecords) {
        out << pair.second.id << " " << pair.second.name << " " << pair.second.severity << "\n";
    }
    out.close();
    cout << "Data saved.\n";
}

void loadFromFile() {
    ifstream in("patients.txt");
    if (!in) return; 

    int id, severity;
    string name;
    while (in >> id >> name >> severity) {
        Patient p = {id, name, severity};
        triage.insert(p);
        patientRecords[id] = p;
    }
    in.close();
}


int getValidInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input! Please enter a number.\n";
        } else {
            return value;
        }
    }
}


bool compareByName(const Patient& a, const Patient& b) {
    return a.name < b.name;
}

int main() {
    loadFromFile();

    int choice = 0;
    while (choice != 6) {
        cout << "\n===== Patient Care System =====\n";
        cout << "1. Register New Patient \n";
        cout << "2. Treat Most Severe Patient\n";
        cout << "3. Search Patient by ID \n";
        cout << "4. Update Patient Severity\n";
        cout << "5. Generate Alphabetical Report\n";
        cout << "6. Exit & Save\n";
        choice = getValidInt("Enter choice: ");

        if (choice == 1) {
            int id = getValidInt("Enter Patient ID: ");
            if (patientRecords.find(id) != patientRecords.end()) {
                cout << "Error: ID already exists!\n";
                continue;
            }
            cout << "Enter Patient Name: ";
            string name;
            cin >> name;
            
            int severity = getValidInt("Enter Severity (1-10): ");
            while (severity < 1 || severity > 10) {
                cout << "Severity must be 1-10.\n";
                severity = getValidInt("Enter Severity (1-10): ");
            }

            Patient p = {id, name, severity};
            triage.insert(p);
            patientRecords[id] = p;
            cout << "Patient registered successfully!\n";
        } 
        else if (choice == 2) {
            try {
                Patient p = triage.extractMax();
                patientRecords.erase(p.id);
                cout << "NOW TREATING: " << p.name << " (Severity: " << p.severity << ")\n";
            } catch (runtime_error& e) {
                cout << e.what() << "\n";
            }
        }
        else if (choice == 3) {
            int id = getValidInt("Enter Patient ID: ");
            if (patientRecords.find(id) != patientRecords.end()) {
                Patient p = patientRecords[id];
                cout << "Found: " << p.name << " | Severity: " << p.severity << "\n";
            } else {
                cout << "Patient not found.\n";
            }
        }
        else if (choice == 4) {
            int id = getValidInt("Enter Patient ID to update: ");
            if (patientRecords.find(id) != patientRecords.end()) {
                int newSeverity = getValidInt("Enter new Severity (1-10): ");
              
                patientRecords[id].severity = newSeverity;
                
               
                triage = TriageHeap();
                for (auto const& pair : patientRecords) {
                    triage.insert(pair.second);
                }
                cout << "Severity updated.\n";
            } else {
                cout << "Patient not found.\n";
            }
        }
        else if (choice == 5) {
            
            vector<Patient> allPatients = triage.getAllPatients();
            sort(allPatients.begin(), allPatients.end(), compareByName);
            
            cout << "--- Alphabetical Patient Report ---\n";
            for (const auto& p : allPatients) {
                cout << p.name << " | ID: " << p.id << " | Severity: " << p.severity << "\n";
            }
        }
        else if (choice == 6) {
            saveToFile();
            cout << "Exiting safely. Goodbye!\n";
        }
        else {
            cout << "Invalid menu choice.\n";
        }
    }
    return 0;
}