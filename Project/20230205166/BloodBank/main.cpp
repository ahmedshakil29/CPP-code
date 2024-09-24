#include <iostream> // for cin,cout
#include <fstream> // for ofstream
#include <vector> // for vector array
#include <string>
#include <sstream>
#include <algorithm> // for length
#include <cstring> //for strncpy
#include <iomanip> // fot setw
#include <limits>
#define MAX_LINE_LENGTH 100

using namespace std;


// Struct to store donor details
struct Donor {
    char name[50], gender[10], blood[10], address[50], date[50],phone[50];
    int id, age;
};

// Function to display the main menu
void displayMenu() {
    cout<<"\n\t\t===========================================";
    cout<<"\n\t\t|   AUST Blood Bank Management System     |"<<endl;
    cout<<"\t\t===========================================\n\n";
    cout << "\t\t 1. Add a New Donor" << endl;
    cout << "\t\t 2. Show All Donor" << endl;
    cout << "\t\t 3. Search Donor By Blood Group" << endl;
    cout << "\t\t 4. Edit a Donor by ID Number" << endl;
    cout << "\t\t 5. Delete a Donor by ID Number" << endl;
    cout << "\t\t 6. Exit" << endl << endl;
}
// Function to validate Gender
bool isValidGender(const string& gender) {
    return (gender == "Male" || gender == "Female");
}
// Function to validate Age
bool isValidAge(int age) {
    return (age > 16 && age <= 50);
}
// Function to validate Blood Group
bool isValidBloodGroup(const string& blood) {
    string validBloodGroups[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (const string& validBlood : validBloodGroups) {
        if (blood == validBlood) {
            return true;
        }
    }
    return false;
}
// Function to validate date
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

// Function to validate phone number (11 digits and numeric)
bool isValidPhoneNumber(const string& phone) {
    return (phone.length() == 11 && all_of(phone.begin(), phone.end(), ::isdigit));
}

// Function to add a single Donor to the text file
void AddDonor(const Donor& donor) {
    ofstream file("donors.txt", ios::app); // Open the file in append mode
    if (!file) {
        cerr << "Failed to open the file.\n";
        return;
    }
cout << " donar phone number : " << donor.phone << donor.name;
    // Write donor's details to the file
    file << donor.id << " " << donor.name << " " << donor.gender << " "
         << donor.age << " " << donor.blood << " " << donor.address
         << " " << donor.date << " " << donor.phone << " "  << "\n";

    file.close(); // Close the file after writing
}

// Function to load and display donors with proper formatting using setw
void ShowDonor(vector<Donor>& donors) {
    ifstream file("donors.txt");
    if (!file) {
        cerr << "Failed to open the file.\n";
        return;
    }

    donors.clear(); // Clear existing donors in the vector

    Donor temp;
    // Read donor details line by line and store them in the vector
    while (file >> temp.id >> temp.name >> temp.gender >> temp.age >> temp.blood >> temp.address >> temp.date >> temp.phone) {
        donors.push_back(temp);
    }
    file.close(); // Close the file after reading

    // Display the list of donors with formatting
    if (donors.empty()) {
        cout << "No donors available.\n";
    } else {
        cout << left << setw(5) << "ID" << setw(15) << "Donor Name" << setw(10) << "Gender" << setw(5) << "Age"
             << setw(15) << "Blood Group" << setw(15) << "Address" << setw(20) << "Last Donation Date" << setw(15) << "Contact Number";
        cout << endl;

        // Display each donor's information with alignment using setw
        for (const auto& donor : donors) {
            cout << left << setw(5) << donor.id << setw(15) << donor.name << setw(10) << donor.gender << setw(5) << donor.age
                 << setw(15) << donor.blood << setw(15) << donor.address << setw(20) << donor.date << setw(15) << donor.phone << "\n";
        }
    }
}
// Function to show all Donors by reading from the text file
void searchDonor(ifstream& file, const string& bloodGroup) {
    string line;
    bool found = false;

    cout << "\nDonor Details:\n";
    cout << left << setw(5) << "ID" << setw(15) << "Donor Name" << setw(8) << "Gender"
         << setw(5) << "Age" << setw(12) << "Blood Group" << setw(20) << "Address"
         << setw(15) << "Last Donation" << setw(15) << "Contact Number" << "\n";
    cout << "---------------------------------------------------------------------------------------------\n";

    // Read the file line by line
    while (getline(file, line)) {
        istringstream iss(line);
        Donor donor;
        iss >> donor.id >> donor.name >> donor.gender >> donor.age >> donor.blood
            >> donor.address >> donor.date >> donor.phone;

        // If the blood group matches, display the donor details horizontally
        if (donor.blood == bloodGroup) {
            found = true;
            cout << left << setw(5) << donor.id << setw(15) << donor.name << setw(8) << donor.gender
                 << setw(5) << donor.age << setw(12) << donor.blood << setw(20) << donor.address
                 << setw(15) << donor.date << setw(15) << donor.phone << "\n";
        }
    }

    // If no matching blood group is found, display a message
    if (!found) {
        cout << "\nNo donor found with blood group: " << bloodGroup << "\n";
    }
}
// Function to edit for a specific Donors
void EditDonor(int editID) {
    ifstream file("donors.txt");
    if (!file) {
        cerr << "Failed to open the file.\n";
        return;
    }

    vector<Donor> donors;
    Donor temp;
    bool found = false;

    // Read all donor records from the file
    while (file >> temp.id >> temp.name >> temp.gender >> temp.age >> temp.blood >> temp.address >> temp.date >> temp.phone) {
        donors.push_back(temp);
    }
    file.close();

    // Search for the donor by ID
    for (auto& donor : donors) {
        if (donor.id == editID) {
            found = true;
            cout << "\nDonor found:\n";
            cout << "ID: " << donor.id << "\n";
            cout << "Donor Name: " << donor.name << "\n";
            cout << "Gender: " << donor.gender << "\n";
            cout << "Age: " << donor.age << "\n";
            cout << "Blood Group: " << donor.blood << "\n";
            cout << "Address: " << donor.address << "\n";
            cout << "Last Donation Date: " << donor.date << "\n";
            cout << "Contact Number: " << donor.phone << "\n";

            // Clear the newline character from input buffer after reading editID
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Prompt for new last donation date with validation
            string date;
            while (true) {
                cout << "\nEnter the new Last Donation Date (YYYY-MM-DD): ";
                getline(cin, date);
                if (isValidDate(date)) {
                    strncpy(donor.date, date.c_str(), sizeof(donor.date));
                    donor.date[sizeof(donor.date) - 1] = '\0'; // Ensure null-termination
                    break;
                } else {
                    cout << "Invalid date format. Please enter a valid date in YYYY-MM-DD format.\n";
                }
            }
            break;
        }
    }

    if (!found) {
        cout << "No donor found with ID: " << editID << "\n";
        return;
    }

    // Rewrite the updated donor records to the file
    ofstream outFile("donors.txt");
    if (!outFile) {
        cerr << "Failed to open the file for writing.\n";
        return;
    }

    for (const auto& donor : donors) {
        outFile << donor.id << " " << donor.name << " " << donor.gender << " " << donor.age << " "
                << donor.blood << " " << donor.address << " " << donor.date << " " << donor.phone << "\n";
    }

    outFile.close();
    cout << "Last Donation Date updated successfully.\n";
}
// Function to delete an donor by ID
void DeleteDonor(int id) {
    ifstream file("donors.txt"); // Open the file for reading
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
    if (remove("donors.txt") != 0) {
        cout << "Error deleting file." << endl;
        return;
    }

    if (rename("temp.txt", "donors.txt") != 0) {
        cout << "Error renaming file." << endl;
        return;
    }
// Display success/failure message
    if (found) {
        cout << "Donor with ID " << id << " deleted successfully." << endl;
    } else {
        cout << "Donor with ID " << id << " not found." << endl;
    }
}
// Main function
int main() {
    vector<Donor> donors;
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
    Donor donor;

    // Input and validations for ID, name, gender, age, blood, date, and time
    cout << "ID: ";
    cin >> donor.id;

    cin.ignore();
    cout << "Donor Name: ";
    cin.getline(donor.name, 50);

    // Gender Validation
    string gender;
    while (true) {
        cout << "Donor Gender (Male/Female): ";
        getline(cin, gender);
        if (isValidGender(gender)) {
            strncpy(donor.gender, gender.c_str(), sizeof(donor.gender));
            donor.gender[sizeof(donor.gender) - 1] = '\0'; // Ensure null-termination
            break;
        } else {
            cout << "Invalid gender. Please enter 'Male', 'Female'.\n";
        }
    }

    // Age Validation
    while (true) {
        cout << "Donor Age: ";
        cin >> donor.age;
        if (isValidAge(donor.age)) {
            break;
        } else {
            cout << "Invalid age. Please enter a valid age between 16 and 45.\n";
        }
    }

    cin.ignore();
    cout << "Donor Blood Group: ";
    string bloodGroup;
    while (true) {
        cin >> bloodGroup;
        if (isValidBloodGroup(bloodGroup)) {
            strncpy(donor.blood, bloodGroup.c_str(), sizeof(donor.blood));
            donor.blood[sizeof(donor.blood) - 1] = '\0'; // Ensure null-termination
            break;
        } else {
            cout << "Invalid blood group. Please enter a valid blood group (e.g., A+, A-, B+, O+, etc.): ";
        }
    }

    cin.ignore();
    cout << "Address: ";
    cin.getline(donor.address, 50);

    // Date Validation
    string date;
    while (true) {
        cout << "Last Blood Donation Date (YYYY-MM-DD): ";
        getline(cin, date);
        if (isValidDate(date)) {
            strncpy(donor.date, date.c_str(), sizeof(donor.date));
            donor.date[sizeof(donor.date) - 1] = '\0'; // Ensure null-termination
            break;
        } else {
            cout << "Invalid date format. Please enter a valid date in YYYY-MM-DD format.\n";
        }
    }

    // Phone Number Validation
    string phone;
    while (true) {
        cout << "Contact Number (11 digits): ";
        cin >> phone;

        if (isValidPhoneNumber(phone)) {
            strncpy(donor.phone, phone.c_str(), sizeof(donor.phone));
            donor.phone[sizeof(donor.phone) - 1] = '\0'; // Ensure null-termination
            break; // Break the loop if phone number is valid
        } else {
            cout << "Invalid phone number. Please enter exactly 11 digits.\n";
        }
    }

    cout << "Phone number validated: " << phone << endl;

    AddDonor(donor); // Save the single donor to the file
    break;
}


            case 2: {
// Show all donors
                ShowDonor(donors);
                break;
            }
            case 3: {
// Search appointment by Blood Group
                ifstream file("donors.txt");
                if (!file) {
                    cerr << "Failed to open the file.\n";
                    return 1;
                }

                string bloodGroup;
                cout << "Enter Blood group for Search: ";
                cin >> bloodGroup;
                searchDonor(file, bloodGroup);// Search and display
                file.close();
                break;
            }
            case 4: {
// Edit an appointment by ID
                int editID;
                cout << "Enter Donor ID to Edit: ";
                cin >> editID;
                EditDonor(editID); // Perform delete operation
                break;
            }
            case 5: {
// Delete an appointment by ID
                int searchID;
                cout << "Enter Donor ID to Delete: ";
                cin >> searchID;
                DeleteDonor(searchID); // Perform delete operation
                break;
            }
            case 6:
 // Exit the program
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
