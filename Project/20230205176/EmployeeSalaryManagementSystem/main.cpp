#include <iostream>
#include <vector>
#include <fstream>
//#include <sstream>
#include <iomanip> // for setw

using namespace std;

// Struct to store Employee information
struct Employee {
    int id;
    string name;
    string department;
    double basicSalary;
    // Percentage of salary allocated to bills and allowances
    double houseBillPercentage = 40.0;
    double medicalBillPercentage = 20.0;
    double transportBillPercentage = 10.0;
    double otherAllowancePercentage = 10.0;
    double deductions = 0.0; // Deductions from salary
        // Calculated bill amounts
    double houseBill = 0.0;
    double medicalBill = 0.0;
    double transportBill = 0.0;
    double otherAllowance = 0.0;
};

// Function declarations
void displayMenu();
bool validateStringInput(const string& str);
bool validateDoubleInput(double num);
void addEmployee(vector<Employee>& employees);
void displayEmployees(const vector<Employee>& employees);
void generatePaySlip(const vector<Employee>& employees);
double calculateNetSalary(const Employee& emp);
double calculateTotalSalary(const Employee& emp);

void updateDeductions(vector<Employee>& employees);
void updateEmployeeAllowances(vector<Employee>& employees);
void updateEmployeeSalary(vector<Employee>& employees);
void saveEmployeesToFile(const vector<Employee>& employees);

// Function to display the menu of available actions
void displayMenu() {
    cout <<endl << endl<< "1. Add Employee" << endl;
    cout << "2. Display Employees" << endl;
    cout << "3. Generate Pay Slip" << endl;
    cout << "4. Display Employees By ID " << endl;
    cout << "5. Update Deductions" << endl;
    cout << "6. Update Employee Allowances" << endl;
    cout << "7. Update Employee Salary" << endl;
    cout << "8. Exit" << endl;
}

// Function to validate string input (non-empty)
bool validateStringInput(const string& str) {
    return !str.empty();
}
// Function to validate if the double input is non-negative
bool validateDoubleInput(double num) {
    return num >= 0;
}

// Function to add a new employee to the system
void addEmployee(vector<Employee>& employees) {
    Employee emp;

    // Input Employee details
    cout << "Enter Employee ID: ";
    cin >> emp.id;
    cin.ignore();

    cout << "Enter Employee Name: ";
    getline(cin, emp.name);
    if (!validateStringInput(emp.name)) {
        cerr << "Invalid Name. Please enter a valid name." << endl;
        return;
    }

    cout << "Enter Employee Department: ";
    getline(cin, emp.department);
    if (!validateStringInput(emp.department)) {
        cerr << "Invalid Department. Please enter a valid department." << endl;
        return;
    }

    cout << "Enter Basic Salary: ";
    cin >> emp.basicSalary;
    if (!validateDoubleInput(emp.basicSalary)) {
        cerr << "Invalid Salary. Please enter a valid salary." << endl;
        return;
    }
    // Calculate house, medical, transport bills, and other allowances
    emp.houseBill = emp.basicSalary * (emp.houseBillPercentage / 100);
    emp.medicalBill = emp.basicSalary * (emp.medicalBillPercentage / 100);
    emp.transportBill = emp.basicSalary * (emp.transportBillPercentage / 100);
    emp.otherAllowance = emp.basicSalary * (emp.otherAllowancePercentage / 100);
   // Add employee to the vector
    employees.push_back(emp);

    // Save to file in a single line per employee
    saveEmployeesToFile(employees);
}

// Function to save employee data to a file
void saveEmployeesToFile(const vector<Employee>& employees) {
    ofstream outFile("employees.txt");
    if (!outFile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
        // Write each employee's data in a single line in the file
    for (const auto& emp : employees) {
        outFile << emp.id << ',' << emp.name << ',' << emp.department << ','
                << emp.basicSalary << ',' << emp.houseBillPercentage << ','
                << emp.medicalBillPercentage << ',' << emp.transportBillPercentage << ','
                << emp.otherAllowancePercentage << ',' << emp.deductions << '\n';
    }
    outFile.close();
}
// Function to display employee details
void displayEmployees(const vector<Employee>& employees) {
    if (employees.empty()) {
        cout << "No employees found." << endl;
        return;
    }
       // Display a formatted table with employee information
    cout << endl<< endl << "*****************************************   Employees Details   *****************************************" << endl << endl;
    cout  << setw(5) << "ID" << setw(10) << "Name" << setw(20) << "Department"
         << setw(15) << "Basic Salary" << setw(18) << "House Bill(%)"
         << setw(18) << "Medical Bill(%)" << setw(21) << "Transport Bill(%)"
         << setw(21) << "Other Allowance(%)" << setw(18) << "Deductions (taka)" << endl;
 // Loop through employees and display their details
    for (const auto& emp : employees) {
        cout  << setw(5) << emp.id << setw(12) << emp.name << setw(14) << emp.department
             << setw(15) << fixed << setprecision(1) << emp.basicSalary << setw(15) << emp.houseBillPercentage
             << setw(18) << emp.medicalBillPercentage << setw(22) << emp.transportBillPercentage
             << setw(18) << emp.otherAllowancePercentage << setw(22) << emp.deductions << endl;
    }
}
// Function to generate pay slips for all employees
void generatePaySlip(const vector<Employee>& employees) {
    if (employees.empty()) {
        cout << "No employees to generate payslip for." << endl;
        return;
    }

    string month;
    cout << "Enter month for the Pay Slip: ";
    cin >> month;
    // Display a formatted table with pay slip details
    cout << "\nPay Slips for " << month << ":\n";
    cout << left << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(20) << "Department"
         << setw(15) << "Basic Salary"
         << setw(15) << "House Bill"
         << setw(15) << "Medical Bill"
         << setw(15) << "Transport Bill"
         << setw(15) << "Other Allowance"
         << setw(15) << "Deductions"
         << setw(25) << "Total Salary (inc. bills)"
         << endl;
    // Loop through employees and generate the pay slip
    for (const auto& emp : employees) {
        double totalSalary = calculateTotalSalary(emp);

        cout << left << setw(10) << emp.id
             << setw(20) << emp.name
             << setw(20) << emp.department
             << setw(15) << fixed << setprecision(2) << emp.basicSalary
             << setw(15) << emp.houseBill
             << setw(15) << emp.medicalBill
             << setw(15) << emp.transportBill
             << setw(15) << emp.otherAllowance
             << setw(15) << emp.deductions
             << setw(25) << totalSalary
             << endl;
    }
}
// Function to calculate the net salary (excluding bills)
double calculateNetSalary(const Employee& emp) {
    return emp.basicSalary - emp.deductions;
}
// Function to calculate the total salary including bills and allowances
double calculateTotalSalary(const Employee& emp) {
    return calculateNetSalary(emp) + emp.houseBill + emp.medicalBill + emp.transportBill + emp.otherAllowance;
}
// Function to display details of a specific employee by ID
void displayEmployeeByID(const vector<Employee>& employees) {
    int id;
    cout << "Enter Employee ID to display details: ";
    cin >> id;
   // Loop through employees and find the matching ID
    for (const auto& emp : employees) {
        if (emp.id == id) {
            cout << endl << "Employee Details:" << endl;
            cout << "ID: " << emp.id << endl;
            cout << "Name: " << emp.name << endl;
            cout << "Department: " << emp.department << endl;
            cout << "Basic Salary: " << fixed << setprecision(2) << emp.basicSalary << endl;
            cout << "House Bill: " << emp.houseBill << " (Percentage: " << emp.houseBillPercentage << "%)" << endl;
            cout << "Medical Bill: " << emp.medicalBill << " (Percentage: " << emp.medicalBillPercentage << "%)" << endl;
            cout << "Transport Bill: " << emp.transportBill << " (Percentage: " << emp.transportBillPercentage << "%)" << endl;
            cout << "Other Allowance: " << emp.otherAllowance << " (Percentage: " << emp.otherAllowancePercentage << "%)" << endl;
            cout << "Deductions: " << emp.deductions << endl;
            double totalSalary = calculateTotalSalary(emp);
            cout << "Total Salary (with allowances): " << totalSalary << endl;
            return;
        }
    }
    cout << "Employee with ID " << id << " not found." << endl;
}

// Function to update deductions for a specific employee
void updateDeductions(vector<Employee>& employees) {
    int id;
    double newDeductions;
    cout << "Enter Employee ID to update deductions: ";
    cin >> id;
    // Loop through employees and find the matching ID

    for (auto& emp : employees) {
        if (emp.id == id) {
            cout << "Enter new Deductions for " << emp.name << ": ";
            cin >> newDeductions;
            emp.deductions = newDeductions;
            saveEmployeesToFile(employees);
            cout << "Deductions updated successfully for " << emp.name << "." << endl;
            return;
        }
    }
    cout << "Employee with ID " << id << " not found." << endl;
}
// Function to update the salary structure for all employees
void updateEmployeeAllowances(vector<Employee>& employees) {
    int id;
    cout << "Enter Employee ID to update allowances: ";
    cin >> id;
    // Update salary structure for all employees
    for (auto& emp : employees) {
        if (emp.id == id) {
            double newHouseBillPercentage, newMedicalBillPercentage, newTransportBillPercentage, newOtherAllowancePercentage;

            cout << "Enter new House Bill percentage for " << emp.name << " (current: " << emp.houseBillPercentage << "%): ";
            cin >> newHouseBillPercentage;
            cout << "Enter new Medical Bill percentage for " << emp.name << " (current: " << emp.medicalBillPercentage << "%): ";
            cin >> newMedicalBillPercentage;
            cout << "Enter new Transport Bill percentage for " << emp.name << " (current: " << emp.transportBillPercentage << "%): ";
            cin >> newTransportBillPercentage;
            cout << "Enter new Other Allowance percentage for " << emp.name << " (current: " << emp.otherAllowancePercentage << "%): ";
            cin >> newOtherAllowancePercentage;

            // Update percentages for the selected employee
            emp.houseBillPercentage = newHouseBillPercentage;
            emp.medicalBillPercentage = newMedicalBillPercentage;
            emp.transportBillPercentage = newTransportBillPercentage;
            emp.otherAllowancePercentage = newOtherAllowancePercentage;

            // Recalculate bills and allowances based on new percentages
            emp.houseBill = emp.basicSalary * (emp.houseBillPercentage / 100);
            emp.medicalBill = emp.basicSalary * (emp.medicalBillPercentage / 100);
            emp.transportBill = emp.basicSalary * (emp.transportBillPercentage / 100);
            emp.otherAllowance = emp.basicSalary * (emp.otherAllowancePercentage / 100);

            // Save changes to file
            saveEmployeesToFile(employees);

            cout << "Allowances updated successfully for " << emp.name << "." << endl;
            return;
        }
    }
    cout << "Employee with ID " << id << " not found." << endl;
}

// Function to update the salary of a specific employee
void updateEmployeeSalary(vector<Employee>& employees) {
    int id;
    cout << "Enter Employee ID to update salary: ";
    cin >> id;
  // Find employee by ID
    for (auto& emp : employees) {
        if (emp.id == id) {
            cout << "Enter new Basic Salary for " << emp.name << ": ";
            cin >> emp.basicSalary;

            // Recalculate bills and allowances based on new salary
            emp.houseBill = emp.basicSalary * (emp.houseBillPercentage / 100);
            emp.medicalBill = emp.basicSalary * (emp.medicalBillPercentage / 100);
            emp.transportBill = emp.basicSalary * (emp.transportBillPercentage / 100);
            emp.otherAllowance = emp.basicSalary * (emp.otherAllowancePercentage / 100);

            saveEmployeesToFile(employees);

            cout << "Basic Salary updated successfully for " << emp.name << "." << endl;
            return;
        }
    }
    cout << "Employee with ID " << id << " not found." << endl;
}

int main() {
    vector<Employee> employees;

    ifstream inFile("employees.txt");
    if (inFile) {
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            Employee emp;
            string temp;
            getline(ss, temp, ','); emp.id = stoi(temp);
            getline(ss, emp.name, ',');
            getline(ss, emp.department, ',');
            getline(ss, temp, ','); emp.basicSalary = stod(temp);
            getline(ss, temp, ','); emp.houseBillPercentage = stod(temp);
            getline(ss, temp, ','); emp.medicalBillPercentage = stod(temp);
            getline(ss, temp, ','); emp.transportBillPercentage = stod(temp);
            getline(ss, temp, ','); emp.otherAllowancePercentage = stod(temp);
            getline(ss, temp, ','); emp.deductions = stod(temp);

            // Recalculate bills and allowances based on loaded percentages
            emp.houseBill = emp.basicSalary * (emp.houseBillPercentage / 100);
            emp.medicalBill = emp.basicSalary * (emp.medicalBillPercentage / 100);
            emp.transportBill = emp.basicSalary * (emp.transportBillPercentage / 100);
            emp.otherAllowance = emp.basicSalary * (emp.otherAllowancePercentage / 100);

            employees.push_back(emp);
        }
        inFile.close();
    }

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addEmployee(employees);
                break;
            case 2:
                displayEmployees(employees);
                break;
            case 3:
                generatePaySlip(employees);
                break;
            case 4:
                displayEmployeeByID(employees);
                break;
            case 5:
                updateDeductions(employees);
                break;
            case 6:
                updateEmployeeAllowances(employees);
                break;
            case 7:
                updateEmployeeSalary(employees);
                break;
            case 8:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
