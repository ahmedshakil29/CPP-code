#include <iostream> // for cout,cin
#include <fstream> // for file open
#include <vector> // for vector
//#include <string>
//#include <sstream>
#include <cstring> // for strncpy
//#include <cctype>
#include <algorithm> // for size,length
#include <iomanip> // Include this for std::setw
//#define MAX_LINE_LENGTH 100
using namespace std;

//Struct to store patient details
struct Patient {
    char name[50], gender[10], blood[10], phone[15], doctorName[50], date[50];
    int id, age, serial;
};

// Function to display the main menu
void displayMenu() {
    cout << "Doctor Appointment Management System" << endl;
    cout << "1. Add a New Appointment" << endl;
    cout << "2. Show All Appointments" << endl;
    cout << "3. Show Appointment By ID" << endl;
    cout << "4. Delete an Appointment by ID Number" << endl;
    cout << "5. Exit" << endl;
}

// Function to validate gender
bool isValidGender(const string& gender) {
    return (gender == "Male" || gender == "Female" );
}

// Function to validate age
bool isValidAge(int age) {
    return (age > 0 && age <= 120);
}

// Function to validate blood group
bool isValidBloodGroup(const string& blood) {
    string validBloodGroups[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (const string& validBlood : validBloodGroups) {
        if (blood == validBlood) {
            return true;
        }
    }
    return false;
}

// Function to validate phone number (11 digits and numeric)
bool isValidPhoneNumber(const string& phone) {
    return (phone.length() == 11 && all_of(phone.begin(), phone.end(), ::isdigit));
}

// Function to validate date format
bool isValidDate(const string& date) {
    // Basic format check for YYYY-MM-DD
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    string year = date.substr(0, 4);
    string month = date.substr(5, 2);
    string day = date.substr(8, 2);
    return (stoi(year) > 1900 && stoi(month) >= 1 && stoi(month) <= 12 && stoi(day) >= 1 && stoi(day) <= 31);
}

// Function to add a single patient to the text file
void AddPatient(const Patient& patient) {
    ofstream file("patient.txt", ios::app); // Open the file in append mode
    if (!file) {
        cerr << "Failed to open the file.\n";
        return;
    }

    // Write patient's details to the file
    file << patient.id << " " << patient.name << " " << patient.gender << " "
         << patient.age << " " << patient.blood << " " << patient.phone << " "
         << patient.doctorName << " " << patient.date << " " << patient.serial << "\n";

    file.close(); // Close the file after writing
}

// Function to show all patients by reading from the text file

void ShowPatient(vector<Patient>& patients) {
    ifstream file("patient.txt"); // Open the file in append mode
    if (!file) {
        cerr << "Failed to open the file.\n";
        return;
    }
    patients.clear(); // Clear existing patients in the vector
    Patient temp;

    // Read patient details line by line and store them in the vector
    while (file >> temp.id >> temp.name >> temp.gender >> temp.age >> temp.blood >> temp.phone >> temp.doctorName >> temp.date >> temp.serial) {
        patients.push_back(temp);
    }
    file.close(); // Close the file after reading

    // Display all patient details
    cout << "Appointment List:\n\n";
    cout << left; // Align output to the left
    cout << setw(5) << "ID"
         << setw(20) << "Patient Name"
         << setw(10) << "Gender"
         << setw(5) << "Age"
         << setw(15) << "Blood Group"
         << setw(15) << "Phone"
         << setw(20) << "Doctor Name"
         << setw(15) << "Appointment Date"
         << setw(10) << "Serial NO" << endl;

    cout << string(110, '-') << endl; // Divider line

    for (const auto& patient : patients) {
        cout << setw(5) << patient.id
             << setw(20) << patient.name
             << setw(10) << patient.gender
             << setw(5) << patient.age
             << setw(15) << patient.blood
             << setw(15) << patient.phone
             << setw(20) << patient.doctorName
             << setw(15) << patient.date
             << setw(10) << patient.serial << endl;
    }
}


// Function to search for a specific appointment by ID
void searchAppointment(ifstream& file, int id) {
    string line;
    bool found = false;

    // Read the file line by line
    while (getline(file, line)) {
        istringstream iss(line);
        Patient patient;
        iss >> patient.id >> patient.name >> patient.gender >> patient.age >> patient.blood >> patient.phone >> patient.doctorName >> patient.date >> patient.serial;

        // If the patient ID matches, display the details
        if (patient.id == id) {
            found = true;
            cout << "\nAppointment Details:\n\n";
            cout << "ID: " << patient.id << "\n";
            cout << "Patient Name: " << patient.name << "\n";
            cout << "Gender: " << patient.gender << "\n";
            cout << "Age: " << patient.age << "\n";
            cout << "Blood: " << patient.blood << "\n";
            cout << "Phone: " << patient.phone << "\n"; // Display phone
            cout << "Doctor Name: " << patient.doctorName << "\n";
            cout << "Appointment Date: " << patient.date << "\n";
            cout << "Appointment Serial: " << patient.serial << "\n";
            break;
        }
    }

    // If no matching ID is found, display a message
    if (!found) {
        cout << "Appointment not found with ID: " << id << "\n";
    }
}

// Function to delete an appointment by ID
void DeleteAppointment(int id) {
    ifstream file("patient.txt"); // Open the file for reading
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    ofstream tempFile("temp.txt"); // Create a temporary file for filtered data
    if (!tempFile.is_open()) {
        cout << "Error creating temporary file." << endl;
        file.close();
        return;
    }

    string line;
    bool found = false;

    // Read the file line by line
    while (getline(file, line)) {
        istringstream iss(line);
        int currentID;
        iss >> currentID;

        // Skip the line if the ID matches the one to be deleted
        if (currentID == id) {
            found = true;
            continue;
        }

        // Write non-matching lines to the temporary file
        tempFile << line << endl;
    }

    file.close(); // Close the original file
    tempFile.close(); // Close the temporary file

    // Replace the original file with the updated file
    if (remove("patient.txt") != 0) {
        cout << "Error deleting file." << endl;
        return;
    }

    if (rename("temp.txt", "patient.txt") != 0) {
        cout << "Error renaming file." << endl;
        return;
    }

    // Display success/failure message
    if (found) {
        cout << "Patient with ID " << id << " deleted successfully." << endl;
    } else {
        cout << "Patient with ID " << id << " not found." << endl;
    }
}

// Main function
int main() {
    vector<Patient> patients;
    int choice;

    // Main loop for the appointment system
    while (true) {
        cout << endl;
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                // Adding a new appointment
                Patient patient;

                // Input and validations for ID, name, gender, age, blood, phone, date, and time
                cout << "ID: ";
                cin >> patient.id;

                cin.ignore();
                cout << "Patient Name: ";
                cin.getline(patient.name, 50);

                // Gender Validation
                string gender;
                while (true) {
                    cout << "Patient Gender (Male/Female): ";
                    getline(cin, gender);
                    if (isValidGender(gender)) {
                        strncpy(patient.gender, gender.c_str(), sizeof(patient.gender));
                        patient.gender[sizeof(patient.gender) - 1] = '\0'; // Ensure null-termination
                        break;
                    } else {
                        cout << "Invalid gender. Please enter 'Male' or 'Female'.\n";
                    }
                }

                // Age Validation
                while (true) {
                    cout << "Patient Age: ";
                    cin >> patient.age;
                    if (isValidAge(patient.age)) {
                        break;
                    } else {
                        cout << "Invalid age. Please enter a valid age between 1 and 120.\n";
                    }
                }

                cin.ignore();
                cout << "Patient Blood Group: ";
                string bloodGroup;
                while (true) {
                    cin >> bloodGroup;
                    if (isValidBloodGroup(bloodGroup)) {
                        strncpy(patient.blood, bloodGroup.c_str(), sizeof(patient.blood));
                        patient.blood[sizeof(patient.blood) - 1] = '\0'; // Ensure null-termination
                        break;
                    } else {
                        cout << "Invalid blood group. Please enter a valid blood group (e.g., A+, A-, B+, O+, etc.): ";
                    }
                }

                // Phone Number Validation
                cin.ignore();
                string phone;
                while (true) {
                    cout << "Patient Phone Number (11 digits): ";
                    getline(cin, phone);
                    if (isValidPhoneNumber(phone)) {
                        strncpy(patient.phone, phone.c_str(), sizeof(patient.phone));
                        patient.phone[sizeof(patient.phone) - 1] = '\0'; // Ensure null-termination
                        break;
                    } else {
                        cout << "Invalid phone number. Please enter an 11-digit phone number.\n";
                    }
                }

                cout << "Doctor Name: ";
                cin.getline(patient.doctorName, 50);

                // Date Validation
                string date;
                while (true) {
                    cout << "Appointment Date (YYYY-MM-DD): ";
                    getline(cin, date);
                    if (isValidDate(date)) {
                        strncpy(patient.date, date.c_str(), sizeof(patient.date));
                        patient.date[sizeof(patient.date) - 1] = '\0'; // Ensure null-termination
                        break;
                    } else {
                        cout << "Invalid date format. Please enter a valid date in YYYY-MM-DD format.\n";
                    }
                }

                cout << "Serial Number: ";
                cin >> patient.serial;

                AddPatient(patient); // Save the single patient to the file
                break;
            }
            case 2: {
                // Show all appointments
                ShowPatient(patients);
                break;
            }
            case 3: {
                // Search appointment by ID
                ifstream file("patient.txt");
                if (!file) {
                    cerr << "Failed to open the file.\n";
                    return 1;
                }

                int searchID;
                cout << "Enter Appointment ID for Search: ";
                cin >> searchID;
                searchAppointment(file, searchID); // Search and display
                file.close();
                break;
            }
            case 4: {
                // Delete an appointment by ID
                int searchID;
                cout << "Enter Appointment ID to Delete: ";
                cin >> searchID;
                DeleteAppointment(searchID); // Perform delete operation
                break;
            }
            case 5:
                // Exit the program
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
