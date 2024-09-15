#include <iostream>
#include <fstream> // for file open ifstream file("Booking.txt");
#include <windows.h> // for  Sleep(1000);
#include <vector> // for  vector<Node*> bookings;
#include <string>
#include <iomanip> // for setw
#include <unordered_map>
#include <algorithm> // for std::sort

using namespace std;

const int TABLE_COUNT = 15;

struct Table {
    bool reserved;
    string status;
};

// Linked list node for booking information
class Node {
public:
    int bookingNum;
    string name;
    int age;
    string phoneNum;
    int tableNum;
    Node* next;
};

class Admin {
private:
    string password = "admin123"; // Default password, should be securely handled in practice

    void viewCustomerInfo();
    void bookTable();
    void viewTableStatus();
    void searchCustomer();
    void editBooking();
    void cancelBooking();
    void logout();
    void menu();

public:
    void login();
};

void Admin::viewCustomerInfo() {
    ifstream file("Booking.txt");
    if (!file) {
        cout << "Error: file could not be opened!" << endl;
        Sleep(1000);
        return;
    }

    vector<Node*> bookings;
    int bookingNum, age, tableNum;
    string name, phoneNum;

    while (file >> bookingNum >> name >> age >>  phoneNum >> tableNum) {
        Node* newNode = new Node{bookingNum, name, age, phoneNum, tableNum, nullptr};
        bookings.push_back(newNode);
    }
    file.close();

    // Bubble sort by table number
    sort(bookings.begin(), bookings.end(), [](Node* a, Node* b) {
        return a->tableNum < b->tableNum;
    });

    cout << "\n\t\t\t\t   Booking Details" << endl;
    cout << "\n     Booking No        Table Number             Name                Age            Phone Number ";
    cout << "\n     *******************************************************************************************";
    for (const auto& booking : bookings) {
        cout << endl << setw(9) << booking->bookingNum
             << setw(20) << booking->tableNum
             << setw(24) << booking->name
             << setw(18) << booking->age
             << setw(24) << booking->phoneNum << endl;
        delete booking;
    }
    menu();
}

void Admin::bookTable() {
    unordered_map<int, Table> tables;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        tables[i] = {false, "Available"};
    }

    ifstream file("Booking.txt");
    int maxBookingNum = 0;  // This will store the highest booking number
    if (file) {
        int bookingNum, age, tableNum;
        string name, phoneNum;
        while (file >> bookingNum >> name >> age >> phoneNum >> tableNum) {
            if (tableNum >= 1 && tableNum <= TABLE_COUNT) {
                tables[tableNum] = {true, "No available"};
            }
            // Update maxBookingNum to hold the highest booking number
            if (bookingNum > maxBookingNum) {
                maxBookingNum = bookingNum;
            }
        }
        file.close();
    }

    cout << "------Select a table to book------" << endl;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        cout << "Table [" << i << "] : " << tables[i].status << endl;
    }

    int tableNum;
    cout << "\nEnter table number to reserve: ";
    cin >> tableNum;

    if (tableNum < 1 || tableNum > TABLE_COUNT || tables[tableNum].reserved) {
        cout << "Table unavailable or invalid number. Please try again." << endl;
        Sleep(2000);
        bookTable();  // Recurse to reattempt booking
        return;
    }

    cout << "Enter customer name: ";
    string name;
    cin >> name;
    cout << "Enter age: ";
    int age;
    cin >> age;

    // Validate phone number (should be 11 digits)
    string phoneNum = "1";
    while (phoneNum.length() > 11 || phoneNum.length() <= 10) {
        cout << "Please enter your phone number: ";
        cin >> phoneNum;

        if (phoneNum.length() > 11 || phoneNum.length() <= 10) {
            cout << endl << "Invalid input. Please input an 11-digit number" << endl;
        }
    }

    // Increment booking number based on the highest booking number found in the file
    int newBookingNum = maxBookingNum + 1;

    // Append to file
    ofstream outfile("Booking.txt", ios::app);
    outfile << newBookingNum << " " << name << " " << age << " " << phoneNum << " " << tableNum << endl;
    outfile.close();

    cout << "Booking added successfully!" << endl;
    Sleep(1000);
    menu();  // Assuming you have a menu() function
}

void Admin::viewTableStatus() {
    system("cls");
    unordered_map<int, Table> tables;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        tables[i] = {false, "Available"};
    }

    ifstream file("Booking.txt");
    if (file) {
        int bookingNum, age, tableNum;
        string name, gender, phoneNum;
        while (file >> bookingNum >> name >> age >> phoneNum >> tableNum) {
            if (tableNum >= 1 && tableNum <= TABLE_COUNT) {
                tables[tableNum] = {true, "No available"};
            }
        }
        file.close();
    }

    cout << "\n\n\t------Tables Status------" << endl;
    for (int i = 1; i <= TABLE_COUNT; ++i) {
        cout << "\tTable [" << i << "] : " << tables[i].status << endl;
    }
    menu();
}

void Admin::searchCustomer() {
    system("cls");
    ifstream file("Booking.txt");
    if (!file) {
        cout << "Error: file could not be opened!" << endl;
        Sleep(1000);
        return;
    }

    string searchName;
    cout << "\n\n\t\tEnter the customer's name to search: ";
    cin >> searchName;

    bool found = false;
    int bookingNum, age, tableNum;
    string name,  phoneNum;

    cout << "\n\t\t\t\t   Booking Details" << endl << endl;
    cout << "\n     Booking No          Name              Age           Phone Number          Table Number      ";
    cout << "\n**************************************************************************************************";
    while (file >> bookingNum >> name >> age  >> phoneNum >> tableNum) {
        if (searchName == name) {
            cout << endl << setw(10) << bookingNum
                 << setw(20) << name
                 << setw(15) << age
                 << setw(23) << phoneNum
                 << setw(16) << tableNum << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\nNo record found!" << endl;
    }
    file.close();
 menu();

}

void Admin::editBooking() {
    system("cls");

    unordered_map<int, Table> tables;
    int bookingNum2, tableNum2, tableNum3;
    bool found = false;

    fstream file, tempFile;

    // Open the booking file
    file.open("Booking.txt", ios::in);
    if (!file) {
        cout << "Error: file could not be opened!";
        Sleep(1000);
        return;
    }

    // Display current bookings
    cout << "\n\t\t\t\t\t    Booking Details " << endl;
    cout << "\n\t\t\t        Booking No         Name           Table Number    ||";
    cout << "\n\t\t\t        ******************************************************";

    string line, name,  phoneNum;
    int bookingNum, age, tableNum;
    int num = 1;

    // Read and display booking details
    while (getline(file, line)) {
        istringstream iss(line);
        iss >> bookingNum >> name >> age >> phoneNum >> tableNum;

        cout << endl << "\t\t\t" << num << ". " << setw(9) << bookingNum << setw(19) << name << setw(16) << tableNum << endl;
        num++;
    }
    file.close();

    // Ask for the booking number to edit
    cout << "\n\t\t\tEnter booking number you want to edit: ";
    cin >> bookingNum2;

    file.open("Booking.txt", ios::in);
    if (!file) {
        cout << "Error: file could not be opened!";
        Sleep(1000);
        return;
    }

    // Find the booking number in the file
    while (getline(file, line)) {
        istringstream iss(line);
        iss >> bookingNum >> name >> age >> phoneNum >> tableNum;

        if (bookingNum2 == bookingNum) {
            found = true;
            tableNum2 = tableNum;  // Store the current table number
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "\t\t\t Invalid booking number. Please try again.";
        Sleep(1000);
        return;
    }

    // Mark reserved tables
    file.open("Booking.txt", ios::in);
    while (getline(file, line)) {
        istringstream iss(line);
        iss >> bookingNum >> name >> age >> phoneNum >> tableNum;

        for (int i = 1; i <= TABLE_COUNT; i++) {
            if (i == tableNum) {
                tables[i].reserved = true;
                tables[i].status = "Not available";
            }
        }
    }
    file.close();

    // Display table status
    system("cls");
    cout << "\n\n\t\t------Table Status------" << endl;
    for (int i = 1; i <= TABLE_COUNT; i++) {
        cout << "\t\tTable [" << i << "] : ";
        if (tables[i].reserved) {
            cout << tables[i].status << endl;
        } else {
            cout << "Available" << endl;
        }
    }

    // Ask for the new table number
    cout << "\n\t\tEnter the new table number: ";
    cin >> tableNum3;

    // Check if the new table number is valid
    if (tableNum3 > TABLE_COUNT || tableNum3 <= 0) {
        cout << "\t\tInvalid table number. Please try again.";
        Sleep(1000);
        //return;
    }

    if (tables[tableNum3].reserved) {
        cout << "\t\tThe table is unavailable. Please try again.";
        Sleep(1000);
        return;
    }

    // Edit booking details in the file
    file.open("Booking.txt", ios::in);
    tempFile.open("tmp.txt", ios::app | ios::out);

    while (getline(file, line)) {
        istringstream iss(line);
        iss >> bookingNum >> name >> age >>  phoneNum >> tableNum;

        if (bookingNum2 != bookingNum) {
            tempFile << bookingNum << " " << name << " " << age << " "
                   << phoneNum << " " << tableNum << endl;
        } else {
            tempFile << bookingNum << " " << name << " " << age << " "
                     << phoneNum << " " << tableNum3 << endl;
        }
    }

    cout << "\n\t\tTable Number " << tableNum2 << " changed to Table Number " << tableNum3 << endl;
    cout << "\n\t\tEdit successful...";

    file.close();
    tempFile.close();

    // Replace old file with updated file
    remove("Booking.txt");
    rename("tmp.txt", "Booking.txt");

    cout << "\n\t\tBack to Menu...";
    Sleep(2000);

    menu();  // Assuming you have a Menu() function to return to the main menu
}

void Admin::cancelBooking() {
    system("cls");
    int bookingNum2;
    bool found = false;

    fstream file, tempFile;
    file.open("Booking.txt", ios::in);

    if (!file) {
        cout << "Error: file could not be opened!";
        Sleep(1000);
        return;
    }

    cout << "\n\t\t\t\t   Booking Details" << endl;
    cout << "\n       Booking No            Name           Age          Phone Number            Table Number ";
    cout << "\n       *******************************************************************************************";

    string line;
    int bookingNum, age, tableNum;
    string name,  phoneNum;
    int num = 1;

    // Display all bookings
    while (getline(file, line)) {
        istringstream iss(line);
        iss >> bookingNum >> name >> age >> phoneNum >> tableNum;

        cout << endl << num << ". "
             << setw(9) << bookingNum
             << setw(22) << name
             << setw(12) << age
             << setw(22) << phoneNum
             << setw(19) << tableNum << endl;
        num++;
    }
    file.close();

    cout << "\n\t Enter the booking number to delete the reservation: ";
    cin >> bookingNum2;

    file.open("Booking.txt", ios::in);
    tempFile.open("tmp.txt", ios::app | ios::out);

    if (!file || !tempFile) {
        cout << "Error: could not open the file!";
        Sleep(1000);
        return;
    }

    // Read bookings again to check the entered booking number and write to a temp file
    while (getline(file, line)) {
        istringstream iss(line);
        iss >> bookingNum >> name >> age >>  phoneNum >> tableNum;

        if (bookingNum2 == bookingNum) {
            found = true;
        } else {
            tempFile << bookingNum << " " << name << " " << age << " "
                     << phoneNum << " " << tableNum << endl;
        }
    }

    file.close();
    tempFile.close();

    if (!found) {
        cout << "\n\t Invalid booking number! Please try again.";
        Sleep(1000);
    } else {
        char confirm;
        cout << "\n\t Are you sure you want to delete booking number " << bookingNum2 << "? (y/n): ";
        cin >> confirm;

        if (tolower(confirm) == 'y') {
            remove("Booking.txt");
            rename("tmp.txt", "Booking.txt");
            cout << "\n\t Cancelled successfully...";
        } else {
            cout << "\n\t Back to Menu...";
        }
        Sleep(1000);
    }
    menu();
}

void Admin::logout() {
    system("cls");
    cout << "You have been logged out." << endl;
    Sleep(1000);
    exit(0);
}

void Admin::menu() {
    cout << endl << endl ;
    cout << "\t\t\t\t\tRestaurant Table Reservation System\n\n";
    cout << "1. View Customer Information\n";
    cout << "2. Book a Table\n";
    cout << "3. View Table Status\n";
    cout << "4. Search for a Customer\n";
    cout << "5. Edit a Booking\n";
    cout << "6. Cancel/Complete a Booking\n";
    cout << "7. Logout\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: viewCustomerInfo(); break;
        case 2: bookTable(); break;
        case 3: viewTableStatus(); break;
        case 4: searchCustomer(); break;
        case 5: editBooking(); break;
        case 6: cancelBooking(); break;
        case 7: logout(); break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            Sleep(1000);
            menu();
    }
}

void Admin::login() {
     menu();
    /*string enteredPassword;
    cout << "Enter Admin Password: ";
    cin >> enteredPassword;

    if (enteredPassword == password) {
        menu();
    } else {
        cout << "Incorrect password. Try again." << endl;
        Sleep(1000);
        login();
    }*/
}

int main() {
    Admin admin;
    admin.login();
    return 0;
}
