/*
TDS6213 Data Structures and Algorithms
Trimester October
Pharmacy Management System C++ Project

Lab 1C - Group 2
1. Eldeena Lim Huey Yinn    1211111904
2. Do Wai Lung              1211109457
3. Doris Heng               1221305730
4. Kong Yi Xuan             1211108301

Stock Update and Alerts ADT
*/

#ifndef STOCKUPDATEALERTS_CPP
#define STOCKUPDATEALERTS_CPP

#include <iostream>
using namespace std;

// Definition of a Stock node, representing a medicine in the pharmacy stock list
struct StockNode{
    int medicineID;
    int stockLevel;
    StockNode* next; // Pointer to the next node in the linked list

    // Constructor to initialize a StockNode with given medicine ID an stock quantity
    StockNode(int medID, int stock) : medicineID(medID), stockLevel(stock), next(nullptr){}

};

// Manual Queue implementation for tracking stock updates and generating alerts
class StockUpdateAlerts {
private:

    StockNode* headPtr; // Pointer to the first node of the linked list

public:

    StockUpdateAlerts();
    ~StockUpdateAlerts();
    void AddMedicineStock();
    void UpdateStock();
    void LowStockAlert(int threshold);
};

// Constructor initializes an empty stock list
StockUpdateAlerts:: StockUpdateAlerts() : headPtr(nullptr)
{
}

// Destructor to free allocated memory
StockUpdateAlerts:: ~StockUpdateAlerts() {
        StockNode* curPtr = headPtr;
        while (curPtr) {
            StockNode* temp = curPtr;
            curPtr = curPtr->next;
            delete temp; // Deallocate memory for each node
        }
    }

// Function to add a new medicine to the stock
void StockUpdateAlerts::AddMedicineStock() {
    int medicineID, initialStock;

    // Prompt the user to input medicine details
    cout << "Enter Medicine ID: ";
    cin >> medicineID;
    cout << "Enter Initial Stock Level: ";
    cin >> initialStock;

    // Create a new StockNode and add it to the linked list
    StockNode* newNode = new StockNode(medicineID, initialStock);
    newNode->next = headPtr;
    headPtr = newNode;

    cout << "Medicine added successfully!" << endl;
}

// Function to update the stock quantity of a given medicine
void StockUpdateAlerts::UpdateStock() {
    int medicineID, quantity;

    // Prompt the user for medicine ID and quantity
    cout << "Enter Medicine ID to update: ";
    cin >> medicineID;
    cout << "Enter quantity to add: ";
    cin >> quantity;

    StockNode* curPtr = headPtr;

    // Traverse the linked list to find the medicine with the given ID
    while (curPtr) {
        if (curPtr->medicineID == medicineID) {
            curPtr->stockLevel += quantity; // Update the stock level with the quantity
            cout << "Stock updated successfully for Medicine ID: " << medicineID << endl;
            return; // Exit after updating
        }
        curPtr = curPtr->next; // Move to the next node
    }

    // If the medicine ID is not found, display a message
    cout << "Medicine with ID: " << medicineID << " not found in the stock list." << endl;
}

// Function to generate low stock alerts for medicines below a given threshold
void StockUpdateAlerts::LowStockAlert (int threshold){
    // Ensure that threshold is valid (more than 15)
    if (threshold <= 15){
        cout << "Stock quantities must be greater than 15 items." << endl;
        return;
    }
    bool alert = false; // Flag to track if alert is generated
    StockNode* curPtr = headPtr;
    while (curPtr){
        if (curPtr -> stockLevel < threshold){
            cout << "ALERT: Medicine ID" << curPtr -> medicineID << " is low on stock ( " << curPtr ->stockLevel << " units left)." << endl;
            alert = true; // Set alert flag to true
        }
        curPtr = curPtr -> next;
    }

    // If no medicines are low on stock, pop-up message informs the admin
    if (!alert){
        cout << "No medicines are in low stock quantities." << endl;
    }
}

int main() {
    StockUpdateAlerts stockAlerts; // Create an instance of StockUpdateAlerts

    int choice;

    // Display menu options
    do {
        cout << "\nPharmacy Stock Update and Alerts" << endl;
        cout << "1. Add Medicine" << endl;
        cout << "2. Update Stock" << endl;
        cout << "3. Low Stock Alerts" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Add a new medicine
                stockAlerts.AddMedicineStock();
                break;

            case 2:
                // Update stock for a specific medicine
                stockAlerts.UpdateStock();
                break;

            case 3: {
                // Display low stock alerts
                int threshold;
                cout << "Enter stock threshold: ";
                cin >> threshold;
                stockAlerts.LowStockAlert(threshold);
                break;
            }

            case 4:
                // Exit the program
                cout << "Exiting the system. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}

#endif
