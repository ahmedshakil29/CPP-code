#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <algorithm>

using namespace std;

const int TABLE_COUNT = 15;  // Define the total number of tables in the restaurant

// Structure to represent a booking with necessary information
struct Booking {
    int bookingNum;
    string name;       // Customer's name
    int age;           // Customer's age
    string phoneNum;   // Customer's phone number
    int tableNum;      // Table number assigned to the booking
};

// Structure to represent a table's reservation status
struct Table {
    bool reserved;     // Whether the table is reserved
    string status;     // Status of the table (e.g., "Available" or "No available")
};

// Admin class to manage bookings, table statuses, and other related operations
class Admin {
private:
    string password = "admin123";  // Default password (in real applications, secure password management should be used)

    // Private methods for different functionalities
    void viewCustomerInfo();
    void bookTable();
    void viewTableStatus();
    void searchCustomer();
    void cancelBooking();
    void logout();
    void menu();

    // Helper method to load bookings from the file
    vector<Booking> loadBookings();

public:
    // Method to log in as admin
    void login();
};

// Loads bookings from the "Booking.txt" file into a vector of Booking structs
vector<Booking> Admin::loadBookings() {
    ifstream file("Booking.txt");
    vector<Booking> bookings;

    if (!file) {
        return bookings;  // Return an empty vector if file can't be opened
    }

    int bookingNum, age, tableNum;
    string name, phoneNum;

    // Read booking information from the file
    while (file >> bookingNum >> name >> age >> phoneNum >> tableNum) {
        bookings.push_back({bookingNum, name, age, phoneNum, tableNum});
    }

    file.close();  // Close the file after reading
    return bookings;  // Return the list of bookings
}

// Display customer information and booking details
void Admin::viewCustomerInfo() {
    vector<Booking> bookings = loadBookings();

    if (bookings.empty()) {
        cout << "No bookings found!" << endl;
        Sleep(1000);
        menu();
        return;
    }

    // Sort bookings by table number for easy viewing
    sort(bookings.begin(), bookings.end(), [](const Booking& a, const Booking& b) {
        return a.tableNum < b.tableNum;
    });

    // Display booking details in a table format
    cout << "\n\t\t\t\t   Booking Details" << endl;
    cout << "\n     Booking No        Table Number             Name                Age            Phone Number ";
    cout << "\n     *******************************************************************************************";

    for (const auto& booking : bookings) {
        cout << endl << setw(9) << booking.bookingNum
             << setw(20) << booking.tableNum
             << setw(24) << booking.name
             << setw(18) << booking.age
             << setw(24) << booking.phoneNum << endl;
    }

    menu();  // Go back to the main menu after displaying
}

// Function to handle table reservations and adding new bookings
void Admin::bookTable() {
    // Initialize tables
    unordered_map<int, Table> tables;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        tables[i] = {false, "Available"};
    }

    vector<Booking> bookings = loadBookings();
    int maxBookingNum = 0;  // Keep track of the highest booking number

    // Update table reservation status based on existing bookings
    for (const auto& booking : bookings) {
        if (booking.tableNum >= 1 && booking.tableNum <= TABLE_COUNT) {
            tables[booking.tableNum] = {true, "No available"};
        }
        maxBookingNum = max(maxBookingNum, booking.bookingNum);  // Find the largest booking number
    }

    // Display available and reserved tables
    cout << "------Select a table to book------" << endl;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        cout << "Table [" << i << "] : " << tables[i].status << endl;
    }

    int tableNum;
    cout << "\nEnter table number to reserve: ";
    cin >> tableNum;

    // Check if the selected table is valid and available
    if (tableNum < 1 || tableNum > TABLE_COUNT || tables[tableNum].reserved) {
        cout << "Table unavailable or invalid number. Please try again." << endl;
        Sleep(2000);
        bookTable();  // Retry if input is invalid
        return;
    }

    // Get customer details
    cout << "Enter customer name: ";
    string name;
    cin >> name;

    cout << "Enter age: ";
    int age;
    cin >> age;

    // Validate phone number
    string phoneNum;
    do {
        cout << "Please enter your phone number (11 digits): ";
        cin >> phoneNum;

        if (phoneNum.length() != 11) {
            cout << "Invalid input. Please enter exactly 11 digits." << endl;
        }
    } while (phoneNum.length() != 11);

    // Increment booking number for the new booking
    int newBookingNum = maxBookingNum + 1;

    // Append new booking to the file
    ofstream outfile("Booking.txt", ios::app);
    outfile << newBookingNum << " " << name << " " << age << " " << phoneNum << " " << tableNum << endl;
    outfile.close();

    cout << "Booking added successfully!" << endl;
    Sleep(1000);
    menu();  // Return to menu after booking
}

// Display the current status of all tables (available or reserved)
void Admin::viewTableStatus() {
    system("cls");

    unordered_map<int, Table> tables;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        tables[i] = {false, "Available"};
    }

    vector<Booking> bookings = loadBookings();

    // Update table status based on existing bookings
    for (const auto& booking : bookings) {
        if (booking.tableNum >= 1 && booking.tableNum <= TABLE_COUNT) {
            tables[booking.tableNum] = {true, "No available"};
        }
    }

    // Display status of all tables
    cout << "\n\n\t------Tables Status------" << endl;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        cout << "\tTable [" << i << "] : " << tables[i].status << endl;
    }

    menu();
}

// Search for customer details by name
void Admin::searchCustomer() {
    system("cls");
    vector<Booking> bookings = loadBookings();

    if (bookings.empty()) {
        cout << "No bookings found!" << endl;
        Sleep(1000);
        menu();
        return;
    }

    string searchName;
    cout << "\n\n\t\tEnter the customer's name to search: ";
    cin >> searchName;

    bool found = false;  // Flag to check if a match is found
    cout << "\n\t\t\t\t   Booking Details" << endl;
    cout << "\n     Booking No          Name              Age           Phone Number          Table Number      ";
    cout << "\n**************************************************************************************************";

    // Search and display matching bookings
    for (const auto& booking : bookings) {
        if (booking.name == searchName) {
            cout << endl << setw(10) << booking.bookingNum
                 << setw(20) << booking.name
                 << setw(15) << booking.age
                 << setw(23) << booking.phoneNum
                 << setw(16) << booking.tableNum << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo record found!" << endl;
    }

    menu();  // Return to menu after search
}

// Function to cancel or delete a booking
void Admin::cancelBooking() {
    system("cls");

    vector<Booking> bookings = loadBookings();
    if (bookings.empty()) {
        cout << "No bookings found!" << endl;
        Sleep(1000);
        menu();
        return;
    }

    int bookingNum2;
    cout << "\n\t\t\t\t   Booking Details" << endl;
    cout << "\n       Booking No            Name           Age          Phone Number            Table Number ";
    cout << "\n       *******************************************************************************************";

    // Display all bookings for cancellation selection
    for (const auto& booking : bookings) {
        cout << endl << setw(9) << booking.bookingNum
             << setw(22) << booking.name
             << setw(12) << booking.age
             << setw(22) << booking.phoneNum
             << setw(19) << booking.tableNum << endl;
    }

    cout << "\n\tEnter the booking number to delete the reservation: ";
    cin >> bookingNum2;

    // Create a temporary file to store bookings except the one to be deleted
    ofstream tempFile("tmp.txt", ios::out);
    bool found = false;

    for (const auto& booking : bookings) {
        if (booking.bookingNum != bookingNum2) {
            tempFile << booking.bookingNum << " " << booking.name << " " << booking.age
                     << " " << booking.phoneNum << " " << booking.tableNum << endl;
        } else {
            found = true;
        }
    }

    tempFile.close();
    remove("Booking.txt");       // Delete original booking file
    rename("tmp.txt", "Booking.txt");  // Rename temporary file to original file

    if (found) {
        cout << "\nReservation cancelled successfully!" << endl;
    } else {
        cout << "\nBooking number not found!" << endl;
    }

    Sleep(1000);
    menu();
}

// Logs out the admin and returns to login screen
void Admin::logout() {
    system("cls");
    cout << "\nLogging out..." << endl;
    Sleep(1000);
    login();  // Return to login screen
}

// Main menu for admin functionalities
void Admin::menu() {

    // Display menu options
    cout << "\n-------Restaurant Management-------\n";
    cout << "1. View Customer Info\n";
    cout << "2. Book Table\n";
    cout << "3. View Table Status\n";
    cout << "4. Search Customer\n";
    cout << "5. Cancel Booking\n";
    cout << "6. Logout\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    // Handle user selection
    switch (choice) {
        case 1: viewCustomerInfo(); break;
        case 2: bookTable(); break;
        case 3: viewTableStatus(); break;
        case 4: searchCustomer(); break;
        case 5: cancelBooking(); break;
        case 6: logout(); break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            Sleep(1000);
            menu();  // Retry on invalid input
    }
}

// Admin login function with password verification
void Admin::login() {
    string enteredPassword;
    cout << "Enter Admin Password: ";
    cin >> enteredPassword;

    // Check if the entered password matches
    if (enteredPassword == password) {
        menu();  // Grant access to the menu
    } else {
        cout << "Incorrect password. Try again." << endl;
        Sleep(1000);
        login();  // Retry login on incorrect password
    }
}

// Main function to initiate admin login
int main() {
    Admin admin;
    admin.login();
    return 0;
}
