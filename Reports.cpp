/*
TDS6213 Data Structures and Algorithms
Trimester October
Pharmacy Management System C++ Project

Lab 1C - Group 2
1. Eldeena Lim Huey Yinn    1211111904
2. Do Wai Lung              1211109457
3. Doris Heng               1221305730
4. Kong Yi Xuan             1211108301

Reports ADT
*/

#ifndef REPORTS_CPP
#define REPORTS_CPP

#include <iostream>
using namespace std;

// Structure to represent a medicine in the inventory
struct Medicine {
    int medicineID;
    char medName[50];
    int medQuantity;
    float medPrice;
    Medicine* next; // Pointer to the next medicine in the linked list

    // Constructor to initialize a Medicine node
    Medicine(int medID, const char* name, int quantity, float price) : medicineID(medID), medQuantity(quantity), medPrice(price), next(nullptr) {
        int i = 0;
        while (name[i] != '\0') {
            medName[i] = name[i];
            i++;
        }
        medName[i] = '\0'; // Null-terminate the string
    }
};

// Structure to represent a sale record
struct Sale {
    char date[11]; // Format: DD-MM-YYYY
    int medicineID;
    int quantitySold;
    float totalPrice;
    Sale* next; // Pointer to the next sale in the linked list

    // Constructor to initialize a Sale node
    Sale(const char* saleDate, int id, int quantity, float price) : medicineID(id), quantitySold(quantity), totalPrice(price), next(nullptr) {
        int i = 0;
        while (saleDate[i] != '\0') {
            date[i] = saleDate[i];
            i++;
        }
        date[i] = '\0'; // Null-terminate the string
    }
};

// Reports ADT class
class Reports {
private:
    Medicine* inventoryHead; // Pointer to the head of the inventory linked list
    Sale* salesHead;         // Pointer to the head of the sales linked list

public:
    Reports();
    ~Reports();
    void AddMedicineToInventory();
    void AddSaleRecord();
    void InventoryReport();
    void SalesReport(const char* startDate, const char* endDate);
};

// Constructor to initialize the inventory and sales linked lists
Reports::Reports() : inventoryHead(nullptr), salesHead(nullptr) {}

// Destructor to deallocate memory for inventory and sales lists
Reports::~Reports() {
    // Free memory for inventory
    Medicine* current = inventoryHead;
    while (current) {
        Medicine* temp = current;
        current = current->next;
        delete temp;
    }

    // Free memory for sales
    Sale* currentSale = salesHead;
    while (currentSale) {
        Sale* temp = currentSale;
        currentSale = currentSale->next;
        delete temp;
    }
}

// Function to add a new medicine to the inventory
void Reports::AddMedicineToInventory() {
    int medicineID, quantity;
    float price;
    char name[50];

    // Get the medicine details from the user
    cout << "Enter Medicine ID: ";
    cin >> medicineID;
    cout << "Enter Medicine Name: ";
    cin.ignore(); // To discard the newline character left by the previous input
    cin.getline(name, 50);
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Price: ";
    cin >> price;

    // Create a new Medicine node and add it to the inventory
    Medicine* newMedicine = new Medicine(medicineID, name, quantity, price);
    newMedicine->next = inventoryHead;
    inventoryHead = newMedicine;

    cout << "Medicine added to inventory successfully!" << endl;
}

// Function to add a sale record
void Reports::AddSaleRecord() {
    char date[11];
    int medicineID, quantitySold;
    float totalPrice;

    // Get sale details from the user
    cout << "Enter Sale Date (DD-MM-YYYY): ";
    cin >> date;
    cout << "Enter Medicine ID: ";
    cin >> medicineID;
    cout << "Enter Quantity Sold: ";
    cin >> quantitySold;
    cout << "Enter Total Price: ";
    cin >> totalPrice;

    // Create a new Sale node and add it to the sales list
    Sale* newSale = new Sale(date, medicineID, quantitySold, totalPrice);
    newSale->next = salesHead;
    salesHead = newSale;

    cout << "Sale record added successfully!" << endl;
}

// Function to generate an inventory report
void Reports::InventoryReport() {
    if (!inventoryHead) {
        cout << "No inventory available to display." << endl;
        return;
    }

    // Print the inventory details
    cout << "Inventory Report:" << endl;
    cout << "---------------------------------" << endl;
    cout << "ID\tName\t\tQuantity\tPrice" << endl;
    cout << "---------------------------------" << endl;

    Medicine* current = inventoryHead;
    while (current) {
        cout << current->medicineID << "\t" << current->medName << "\t\t"
             << current->medQuantity << "\t\t" << current->medPrice << endl;
        current = current->next;
    }
}

// Function to compare two date strings in the format DD-MM-YYYY
int customStringCompare(const char* str1, const char* str2) {
    for (int i = 0; i < 10; i++) {
        if (str1[i] < str2[i]) {
            return -1; // str1 is lexicographically less than str2
        }
        if (str1[i] > str2[i]) {
            return 1; // str1 is lexicographically greater than str2
        }
    }
    return 0; // str1 is equal to str2
}

// Function to generate a sales report for a specific date range
void Reports::SalesReport(const char* startDate, const char* endDate) {
    if (!salesHead) {
        cout << "No sales records available to display." << endl;
        return;
    }

    // Print the sales report for the given date range
    cout << "Sales Report from " << startDate << " to " << endDate << ":" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Date\t\tMedicine ID\tQuantity\tTotal Price" << endl;
    cout << "---------------------------------------------" << endl;

    Sale* current = salesHead;
    while (current) {
        // Check if the sale date is within the given date range
        if ((customStringCompare(current->date, startDate) >= 0) && (customStringCompare(current->date, endDate) <= 0)) {
            cout << current->date << "\t" << current->medicineID << "\t\t"
                 << current->quantitySold << "\t\t" << current->totalPrice << endl;
        }
        current = current->next;
    }
}


int main() {
    Reports reports; // Create an instance of the Reports class

    int choice;

    // Display menu options
    do {
        cout << "\nPharmacy Reports System" << endl;
        cout << "1. Add Medicine to Inventory" << endl;
        cout << "2. Add Sale Record" << endl;
        cout << "3. Inventory Report" << endl;
        cout << "4. Sales Report" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Add a new medicine to the inventory
                reports.AddMedicineToInventory();
                break;

            case 2:
                // Add a sale record
                reports.AddSaleRecord();
                break;

            case 3:
                // Display inventory report
                reports.InventoryReport();
                break;

            case 4: {
                // Display sales report for a given period
                char startDate[11], endDate[11];
                cout << "Enter Start Date (DD-MM-YYYY): ";
                cin >> startDate;
                cout << "Enter End Date (DD-MM-YYYY): ";
                cin >> endDate;
                reports.SalesReport(startDate, endDate);
                break;
            }

            case 5:
                // Exit the system
                cout << "Exiting the system. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}

#endif
