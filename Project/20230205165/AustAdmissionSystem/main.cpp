#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>  // For setfill and setw



using namespace std;

struct Student {
    string phone;
    string Password;
    string name;
    string gender;
    int sscRoll;
    int sscYear;
    float sscGPA;
    int hscRoll;
    int hscYear;
    float hscGPA;
    string examStatus = "Pending"; // will be pass or fail
    int position = 0;
    string selectDepartment = "pending";
//    string studentFinalStatus = "Pending";
};

struct Admin {
    string userName;
    string Password;
};

// Function to check if the phone number is exactly 11 digits
bool isValidPhoneNumber(const string& phone) {
    return (phone.length() == 11 && all_of(phone.begin(), phone.end(), ::isdigit));
}

// Validate if a string is not empty
bool validateStringInput(const string& str) {
    return !str.empty();
}

// Validate gender
bool isValidGender(const string& gender) {
    return (gender == "M" || gender == "F");
}

// Validate year range for SSC/HSC
bool isValidYear(int year) {
    return (year >= 2020 && year <= 2024);
}

// Function to check if a roll number is exactly 6 digits
bool isValidRollNumber(const int& roll) {
    return (to_string(roll).length() == 6);
}

// Validate SSC GPA (must be 5.0)
bool isValidSSCGPA(float sscGPA) {
    return (sscGPA == 5.0);
}

// Validate HSC GPA (must be between 4.50 and 5.00)
bool isValidHSCGPA(float hscGPA) {
    return (hscGPA >= 4.50 && hscGPA <= 5.00);
}

// Function to register student with validation
void RegisterStudent() {
    Student student;

    cout << "\n----- Student Registration -----\n";

    // Validate Phone Number (11 digits)
    bool validPhone = false;
    while (!validPhone) {
        cout << "Enter Phone (11 digits): ";
        cin >> student.phone;

        // Check if the phone number already exists
        ifstream file("students.txt");
        string existingPhone;
        bool phoneExists = false;

        while (file >> existingPhone) {
            if (existingPhone == student.phone) {
                phoneExists = true;
                break;
            }
            // Skip other fields in the file
            file.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        file.close();

        if (phoneExists) {
            cout << "This phone number is already registered. Please enter a different number.\n";
        } else if (!isValidPhoneNumber(student.phone)) {
            cout << "Invalid phone number. Please enter exactly 11 digits.\n";
        } else {
            validPhone = true; // Exit loop if phone is valid and unique
        }
    }

    // Validate Password
    do {
        cout << "Enter Password: ";
        cin >> student.Password;
        if (!validateStringInput(student.Password)) {
            cout << "Invalid input. Please try again.\n";
        }
    } while (!validateStringInput(student.Password));

    cin.ignore();  // Ignore newline for getline

    // Student Name
    do {
        cout << "Enter Name: ";
        getline(cin, student.name);
        if (!validateStringInput(student.name)) {
            cout << "Invalid input. Please try again.\n";
        }
    } while (!validateStringInput(student.name));

    // Validate Gender
    do {
        cout << "Enter Gender (M/F): ";
        cin >> student.gender;
        transform(student.gender.begin(), student.gender.end(), student.gender.begin(), ::toupper);
        if (!isValidGender(student.gender)) {
            cout << "Invalid gender. Please enter 'M' or 'F'.\n";
        }
    } while (!isValidGender(student.gender));

    // Validate SSC Roll Number (6 digits)
    do {
        cout << "Enter SSC Roll : ";
        cin >> student.sscRoll;
        if (!isValidRollNumber(student.sscRoll)) {
            cout << "Invalid SSC Roll. Please enter correct Roll Number.\n";
        }
    } while (!isValidRollNumber(student.sscRoll));

    // Validate SSC Year
    do {
        cout << "Enter SSC Year: ";
        cin >> student.sscYear;
        if (!isValidYear(student.sscYear)) {
            cout << "Invalid SSC year. Please enter a year between 2020 and 2024.\n";
        }
    } while (!isValidYear(student.sscYear));

    // Validate SSC GPA
    do {
        cout << "Enter SSC GPA : ";
        cin >> student.sscGPA;
        if (!isValidSSCGPA(student.sscGPA)) {
            cout << "Not eligible. Only 5.0 is accepted.\n";
        }
    } while (!isValidSSCGPA(student.sscGPA));

    // Validate HSC Roll Number (6 digits)
    do {
        cout << "Enter HSC Roll (6 digits): ";
        cin >> student.hscRoll;
        if (!isValidRollNumber(student.hscRoll)) {
            cout << "Invalid HSC Roll. Please enter correct Roll Number.\n";
        }
    } while (!isValidRollNumber(student.hscRoll));

    // Validate HSC Year
    do {
        cout << "Enter HSC Year: ";
        cin >> student.hscYear;
        if (!isValidYear(student.hscYear)) {
            cout << "Invalid HSC year. Please enter a year between 2020 and 2024.\n";
        }
    } while (!isValidYear(student.hscYear));

    // Validate HSC GPA
    do {
        cout << "Enter HSC GPA (4.50 - 5.00): ";
        cin >> student.hscGPA;
        if (!isValidHSCGPA(student.hscGPA)) {
            cout << "Not Eligible. We accepted 4.50 to 5.00.\n";
        }
    } while (!isValidHSCGPA(student.hscGPA));

    // Save student to file
    ofstream file("students.txt", ios::app);
    if (file.is_open()) {
        student.examStatus = "pending";  // Set default value
        student.position = 0;  // Set default value
        student.selectDepartment = "pending";  // Set default value
        file << student.phone << " " << student.Password << " " << student.name << " " << student.gender
             << " " << student.sscRoll << " " << student.sscYear << " " << student.sscGPA << " "
             << student.hscRoll << " " << student.hscYear << " " << student.hscGPA << " "
             << student.examStatus << " " << student.position << " "
             << student.selectDepartment << "\n";
        file.close();
    }

    cout << "Registration successful!\n";
}

// Function for student login
bool StudentLogin(string phone, string password, Student &loggedInStudent) {
    ifstream file("students.txt");
    if (file.is_open()) {
        string line;
        while (file >> loggedInStudent.phone >> loggedInStudent.Password >> loggedInStudent.name >> loggedInStudent.gender
                    >> loggedInStudent.sscRoll >> loggedInStudent.sscYear >> loggedInStudent.sscGPA
                    >> loggedInStudent.hscRoll >> loggedInStudent.hscYear >> loggedInStudent.hscGPA
                    >> loggedInStudent.examStatus >> loggedInStudent.position >> loggedInStudent.selectDepartment) {

            if (loggedInStudent.phone == phone && loggedInStudent.Password == password) {
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}

// Function to view exam day (from admin.txt)
void ViewExamDay() {
    ifstream file("admin.txt");
    if (!file.is_open()) {
        cout << "Error opening admin file.\n";
        return;
    }

    string line;
    cout << "\n--- Exam Date ---\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

// Function to view exam result
void ViewExamResult(const Student& student) {
    cout << "\n--- Exam Result ---\n";
    cout << "Name: " << student.name << "\n";

    if (student.examStatus == "Pending") {
        cout << "Your result is still pending.\n";
    } else {
        cout << "You have been " << student.examStatus << ".\n";
        if (student.examStatus == "pass") {
            cout << "Your position is " << student.position << " and you have been selected for the " << student.selectDepartment << " department.\n";
        }
    }
}

// Function to view personal info
void ViewPersonalInfo(const Student& student) {
    cout << "\n--- Personal Information ---\n";
    cout << "Phone: " << student.phone << "\n";
    cout << "Name: " << student.name << "\n";
    cout << "Gender: " << student.gender << "\n";
    cout << "SSC Roll: " << student.sscRoll << "\n";
    cout << "SSC Year: " << student.sscYear << "\n";
    cout << "SSC GPA: " << student.sscGPA << "\n";
    cout << "HSC Roll: " << student.hscRoll << "\n";
    cout << "HSC Year: " << student.hscYear << "\n";
    cout << "HSC GPA: " << student.hscGPA << "\n";
}
// Student Menu
void StudentMenu(const Student& student) {
    int choice;
    do {
        cout << endl << "--- Ahsanullah University of Science and Technology ---" << endl;
        cout << "1. View Exam Day" << endl;
        cout << "2. View Exam Result" << endl;
        cout << "3. View Personal Info" << endl;
        cout << "4. Logout" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                ViewExamDay();  // View the exam day
                break;
            case 2:
                ViewExamResult(student);  // View the exam result
                break;
            case 3:
                ViewPersonalInfo(student);  // View personal info
                break;
            case 4:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 4);
}

// Function to display all students
void DisplayAllStudents() {
    ifstream file("students.txt");
    if (file.is_open()) {
        // Display column headers
        cout << left << setw(15) << "Phone"
             << setw(15) << "Name"
             << setw(8) << "Gender" << setw(10) << "SSC Roll" << setw(10) << "SSC Year"
             << setw(8) << "SSC GPA" << setw(10) << "HSC Roll" << setw(10) << "HSC Year"
             << setw(8) << "HSC GPA" << setw(12) << "Exam Status" << setw(10) << "Position"
             << setw(18) << "Selected Dept." << endl;

        cout << string(145, '-') << endl; // Separator line for better readability

        // Display student data
        string phone, password, name, gender, examStatus, selectDepartment;
        int sscRoll, sscYear, hscRoll, hscYear, position;
        float sscGPA, hscGPA;

        while (file >> phone >> password >> name >> gender >> sscRoll >> sscYear >> sscGPA
                    >> hscRoll >> hscYear >> hscGPA >> examStatus >> position
                    >> selectDepartment) {

            // Display each student's data in aligned format
            cout << left << setw(15) << phone // Display phone number
                 << setw(15) << name
                 << setw(8) << gender << setw(10) << sscRoll << setw(10) << sscYear
                 << setw(8) << sscGPA << setw(10) << hscRoll << setw(10) << hscYear
                 << setw(8) << hscGPA << setw(12) << examStatus << setw(10) << position
                 << setw(18) << selectDepartment << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file.\n";
    }
}

// Helper function to get the day name based on a given date
string getDayName(int day, int month, int year) {
    tm time_in = { 0, 0, 0, day, month - 1, year - 1900 };
    time_t time_temp = mktime(&time_in);
    const tm *time_out = localtime(&time_temp);
    int weekday = time_out->tm_wday;
    const char *days[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    return days[weekday];
}

// Function to check if an exam date already exists
bool checkExistingExamDate() {
    ifstream inFile("admin.txt");
    string line;
    bool exists = false;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.find("Exam Date") != string::npos) {
                exists = true;
                break;
            }
        }
        inFile.close();
    }
    return exists;
}

// Function to validate the date
bool isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1 || year < 1900) {
        return false;
    }

    int daysInMonth[] = { 31, (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    return day <= daysInMonth[month - 1];
}

// Function to set the exam date and store it in the admin.txt file
void SetExamSDate() {
    string inputDate;

    // Check if there's already an exam date set
    if (checkExistingExamDate()) {
        char overwriteChoice;
        cout << "An exam date already exists. Do you want to set a new exam date? (y/n): ";
        cin >> overwriteChoice;
        if (tolower(overwriteChoice) != 'y') {
            cout << "Exam date not changed.\n";
            return;
        }
    }

    // Prompt admin for new date input
    cout << "Enter the exam date (YYYY/MM/DD): ";
    cin >> inputDate;

    // Parse the input date
    int year, month, day;
    char delimiter1, delimiter2;
    stringstream ss(inputDate);
    ss >> year >> delimiter1 >> month >> delimiter2 >> day;

    // Validate the date
    if (ss.fail() || delimiter1 != '/' || delimiter2 != '/' || !isValidDate(day, month, year)) {
        cout << "Invalid date format or values. Please enter a valid date in the format YYYY/MM/DD.\n";
        return;
    }

    // Get the day name from the input date
    string dayName = getDayName(day, month, year);

    // Print the exam date and day name for confirmation
    cout << "New Exam Date: " << setfill('0') << setw(2) << day << "/"
         << setfill('0') << setw(2) << month << "/" << year
         << " (" << dayName << ")\n";

    // Save the new exam date and day name to admin.txt
    ofstream outFile("admin.txt", ios::trunc);  // Overwrite any existing data
    if (!outFile.is_open()) {
        cout << "Error opening admin file for writing.\n";
        return;
    }

    outFile << "Exam Date: " << setfill('0') << setw(2) << day << "/"
            << setfill('0') << setw(2) << month << "/" << year
            << "\nDay: " << dayName << "\n";

    outFile.close();

    cout << "Exam date has been set and saved to admin.txt successfully.\n";
}

// Function for admin to update a student's information
void AdminUpdate() {
    string phone;
    cout << "Enter the phone number of the student to update: ";
    cin >> phone;

    vector<Student> students;
    ifstream file("students.txt");

    // Check if file opened successfully
    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

    // Load all student data from the file
    Student student;
    bool studentFound = false;
    while (file >> student.phone >> student.Password >> student.name >> student.gender
                >> student.sscRoll >> student.sscYear >> student.sscGPA
                >> student.hscRoll >> student.hscYear >> student.hscGPA
                >> student.examStatus >> student.position
                >> student.selectDepartment) {

        if (student.phone == phone) {
            studentFound = true;

            // Display current information of the student
            cout << "\n--- Current Student Information ---\n";
            cout << "Name: " << student.name << "\n";
            cout << "Exam Status: " << student.examStatus << "\n";
            cout << "Position: " << student.position << "\n";
            cout << "Selected Department: " << student.selectDepartment << "\n";

            // Input and verify exam status
            string newExamStatus;
            do {
                cout << "Enter new Exam Status (pass or fail): ";
                cin >> newExamStatus;
                if (newExamStatus != "pass" && newExamStatus != "fail") {
                    cout << "Invalid input. Please enter 'pass' or 'fail'.\n";
                }
            } while (newExamStatus != "pass" && newExamStatus != "fail");
            student.examStatus = newExamStatus;

            // Update position and department only if the student passes
            if (newExamStatus == "pass") {
                // Input and verify position
                int newPosition;
                while (true) {
                    cout << "Enter new Position (positive integer): ";
                    cin >> newPosition;
                    if (cin.fail() || newPosition <= 0) {
                        cout << "Invalid input. Please enter a valid positive integer.\n";
                        cin.clear(); // Clear error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
                    } else {
                        student.position = newPosition;
                        break;
                    }
                }

                // Input and verify selected department
                string newDepartment;
                const vector<string> validDepartments = {"CSE", "EEE", "ME", "CE", "BBA","Waiting"};
                bool validDepartment = false;
                while (!validDepartment) {
                    cout << "Enter Selected Department (e.g., CSE, EEE, ME, CE, BBA,Waiting): ";
                    cin >> newDepartment;
                    for (const string &dept : validDepartments) {
                        if (newDepartment == dept) {
                            validDepartment = true;
                            student.selectDepartment = newDepartment;
                            break;
                        }
                    }
                    if (!validDepartment) {
                        cout << "Invalid department. Please enter one of the valid options: CSE, EEE, ME, CE, BBA.\n";
                    }
                }
            } else {
                cout << "Student has failed. Position and department will not be updated.\n";
            }
            cout << "Student information updated successfully.\n";
        }
        // Add each student (including the updated one) to the vector
        students.push_back(student);
    }
    file.close();

    // If the student was found and updated, write back all students to the file
    if (studentFound) {
        ofstream outFile("students.txt", ios::trunc);  // Truncate the file to overwrite
        if (!outFile.is_open()) {
            cout << "Error opening file for writing.\n";
            return;
        }

        // Write the updated student list back to the file
        for (const auto &s : students) {
            outFile << s.phone << " " << s.Password << " " << s.name << " " << s.gender
                    << " " << s.sscRoll << " " << s.sscYear << " " << s.sscGPA
                    << " " << s.hscRoll << " " << s.hscYear << " " << s.hscGPA
                    << " " << s.examStatus << " " << s.position
                    << " " << s.selectDepartment << "\n";
        }
        outFile.close();
    } else {
        cout << "Student with phone number " << phone << " not found.\n";
    }
}

// Admin Menu
void AdminMenu() {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "[1] Display All Student Applications\n";
        cout << "[2] Set Exam Date \n";
        cout << "[3] Update Student Info\n";
        cout << "[4] Logout\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            DisplayAllStudents(); // Display all students
        } else if (choice == 2) {
            SetExamSDate(); // Set exam status for top students
        } else if (choice == 3) {
            AdminUpdate(); // Update student data
        } else if (choice != 4) {
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 4);
}

// Admin login function with password verification
void Adminlogin() {
    string enteredPassword;
    cout << "Enter Admin Password: ";
    cin >> enteredPassword;

    // Check if the entered password matches
    if (enteredPassword == "admin123") {
        AdminMenu();  // Grant access to the menu
    } else {
        cout << "Incorrect password. Try again." << endl;
}
}


// Main function
int main() {
    int choice;
    do {
        cout << endl << "--- Ahsanullah University of Science and Technology ---" << endl << endl;
        cout << "1. Register " << endl;
        cout << "2. Student Login" << endl;
        cout << "3. Admin Login" << endl;
        cout << "4. Exit" << endl << endl;
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            RegisterStudent();
        }
        else if (choice == 2) {
            string phone, password;
            Student student;
            cout << "Enter Phone: ";
            cin >> phone;
            cout << "Enter Password: ";
            cin >> password;

            if (StudentLogin(phone, password, student)) {
                cout << "Login successful!\n";
                StudentMenu(student); // Redirect to Student Menu
            } else {
                cout << "Invalid phone or password.\n";
            }
        }
        else if (choice == 3) {
            Adminlogin(); // Redirect to Admin Menu
        } else if (choice == 4) {
            cout << "Exiting...\n";
        } else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 4);

    return 0;
}
