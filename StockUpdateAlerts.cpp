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
    bool UpdateStock(int medicineID, int quantity);
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

// Function to update stock quantity of a given medicine
bool StockUpdateAlerts::UpdateStock (int medicineID, int quantity){
    StockNode* curPtr = headPtr;

    // Traverse the linked list to find the medicine with given ID
    while (curPtr){
        if (curPtr -> medicineID == medicineID){
            curPtr -> stockLevel += quantity; // Update the stock level with the quantity
            return true; // Medicine is found and updated
        }
        curPtr = curPtr -> next; // Move to the next node
    }
    return false; // Medicine does not exist in the list
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

#endif
