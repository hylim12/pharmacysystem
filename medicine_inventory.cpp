/*
TDS6213 Data Structures and Algorithms
Trimester October
Pharmacy Management System C++ Project

Lab 1C - Group 2
1. Eldeena Lim Huey Yinn    1211111904
2. Do Wai Lung              1211109457
3. Doris Heng               1221305730
4. Kong Yi Xuan             1211108301

Medicine Inventory ADT
*/

#ifndef MEDICINE_INVENTORY_CPP
#define MEDICINE_INVENTORY_CPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <regex>
using namespace std;

// Definition of a Medicine node
struct Medicine {
    string medicineID;
    string medName;
    string medManufacturer;
    int medQuantity;
    float medPrice;
    string expiryDate;
    Medicine* next;

    // Default constructor
    Medicine() : medicineID(""), medName(""), medManufacturer(""), medQuantity(0), medPrice(0.0), expiryDate(""), next(nullptr) {}

    // Parameterized constructor
    Medicine(string id, string name, string manufacturer, int quantity, float price, string expiry)
        : medicineID(id), medName(name), medManufacturer(manufacturer), medQuantity(quantity), medPrice(price), expiryDate(expiry), next(nullptr) {}
};

// Medicine Inventory ADT
class MedicineInventory {
private:
    Medicine* headPtr; // Pointer to the head of the linked list

    bool isUniqueID(string medicineID) const;
    bool isLoading; // True when loading data from file

public:
    MedicineInventory() : headPtr(nullptr), isLoading(false){}
    ~MedicineInventory();
    Medicine* GetHeadPtr() const;

    bool CreateMedicine(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate);
    bool EditMedicine(string medicineID, string newName, string newManufacturer, int newQuantity, float newPrice, string newExpiry);
    bool RemoveMedicine(string medicineID);
    void DisplayMedicines() const;
    void SortByName();
    void SortByQuantity();
    void Menu();

    void SaveToFile(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate);
    void LoadFromFile();

    Medicine* SearchMedicineByID(string medicineID) const;
    Medicine* SearchMedicineByName(string medName) const;
};

// Destructor to free memory
MedicineInventory::~MedicineInventory() {
    while (headPtr) {
        Medicine* temp = headPtr;
        headPtr = headPtr->next;
        delete temp;
    }
}

// Checks if the medicine ID is unique
bool MedicineInventory::isUniqueID(string medicineID) const {
    Medicine* curPtr = headPtr;
    while (curPtr) {
        if (curPtr->medicineID == medicineID) {
            return false;
        }
        curPtr = curPtr->next;
    }
    return true;
}

// Getter function to access the head pointer of the Medicine linked list since it is private.
Medicine* MedicineInventory::GetHeadPtr() const {
    return headPtr;
}

// Adds a new medicine to the inventory
bool MedicineInventory::CreateMedicine(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate) {
    if (!isUniqueID(medicineID)) {
        cout << "Error! Medicine ID already exists.\n";
        return false;
    }

    Medicine* newMedicine = new Medicine(medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate);
    newMedicine->next = headPtr;
    headPtr = newMedicine;

    // Save the new medicine to the file if not loading data
    if(!isLoading){
        SaveToFile(medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate);
    }

    return true;
}

// Saves a single medicine record to the file
void MedicineInventory::SaveToFile(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate) {
    ofstream outFile("inventory_data.txt", ios::app); // Open in append mode
    if (outFile) {
        outFile << medicineID << "," << medName << "," << medManufacturer << ","
                << medQuantity << "," << medPrice << "," << expiryDate << "\n";
        outFile.close();
    } else {
        cout << "Error! Unable to write to file.\n";
    }
}

// Load record from the file
void MedicineInventory::LoadFromFile() {
    ifstream inFile("inventory_data.txt");
    if (!inFile) {
        cout << "No previous data found. Starting fresh.\n";
        return;
    }

    string line;
    while (getline(inFile, line)) {
        // Parse the line (medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate)
        stringstream ss(line);
        string medicineID, medName, medManufacturer, expiryDate;
        int medQuantity;
        float medPrice;

        getline(ss, medicineID, ',');
        getline(ss, medName, ',');
        getline(ss, medManufacturer, ',');
        ss >> medQuantity;
        ss.ignore(1, ',');
        ss >> medPrice;
        ss.ignore(1, ',');
        getline(ss, expiryDate, ',');

        // Add to linked list
        CreateMedicine(medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate);
    }

    isLoading = false;  // Loading complete
    inFile.close();
}


// Edits an existing medicine
bool MedicineInventory::EditMedicine(string medicineID, string newName, string newManufacturer, int newQuantity, float newPrice, string newExpiry) {
    Medicine* medicine = SearchMedicineByID(medicineID);
    if (!medicine) {
        cout << "Error! Medicine not found.\n";
        return false;
    }

    medicine->medName = newName;
    medicine->medManufacturer = newManufacturer;
    medicine->medQuantity = newQuantity;
    medicine->medPrice = newPrice;
    medicine->expiryDate = newExpiry;

    return true;
}

// Removes a medicine from the inventory
bool MedicineInventory::RemoveMedicine(string medicineID) {
    Medicine* curPtr = headPtr;
    Medicine* prevPtr = nullptr;

    while (curPtr) {
        if (curPtr->medicineID == medicineID) {
            if (prevPtr) {
                prevPtr->next = curPtr->next;
            } else {
                headPtr = curPtr->next;
            }
            delete curPtr;
            return true;
        }
        prevPtr = curPtr;
        curPtr = curPtr->next;
    }

    cout << "Error! Medicine ID not found.\n";
    return false;
}

// Displays all medicines in the inventory
void MedicineInventory::DisplayMedicines() const {
    if (!headPtr) {
        cout << "The inventory is empty.\n";
        return;
    }

    Medicine* curPtr = headPtr;
    cout << "Medicine Inventory:\n";
    while (curPtr) {
        cout << "ID: " << curPtr->medicineID << ", Name: " << curPtr->medName
             << ", Manufacturer: " << curPtr->medManufacturer
             << ", Quantity: " << curPtr->medQuantity
             << ", Price: RM" << curPtr->medPrice
             << ", Expiry: " << curPtr->expiryDate << endl;
        curPtr = curPtr->next;
    }
}

// Sorts medicines by name in ascending order, then display the sorted inventory.
void MedicineInventory::SortByName() {
    if (!headPtr || !headPtr->next) {
        return; // No sorting needed for empty or single-node list
    }

    bool swapped;
    Medicine* curPtr;
    Medicine* prevPtr = nullptr;

    do {
        swapped = false;
        curPtr = headPtr;

        while (curPtr->next != prevPtr) {
            if (curPtr->medName > curPtr->next->medName){
                // Swap medicine data
                swap(curPtr->medicineID, curPtr->next->medicineID);
                swap(curPtr->medName, curPtr->next->medName);
                swap(curPtr->medManufacturer, curPtr->next->medManufacturer);
                swap(curPtr->medQuantity, curPtr->next->medQuantity);
                swap(curPtr->medPrice, curPtr->next->medPrice);
                swap(curPtr->expiryDate, curPtr->next->expiryDate);
                swapped = true;
            }
            curPtr = curPtr->next;
        }
        prevPtr = curPtr;
    } while (swapped);
}

// Sorts medicines by quantity in ascending order, then display the sorted inventory.
void MedicineInventory::SortByQuantity() {
    if (!headPtr || !headPtr->next) {
        return; // No sorting needed for empty or single-node list
    }

    bool swapped;
    Medicine* curPtr;
    Medicine* prevPtr = nullptr;

    do {
        swapped = false;
        curPtr = headPtr;

        while (curPtr->next != prevPtr) {
            if (curPtr->medQuantity > curPtr->next->medQuantity) {
                // Swap medicine data
                swap(curPtr->medicineID, curPtr->next->medicineID);
                swap(curPtr->medName, curPtr->next->medName);
                swap(curPtr->medManufacturer, curPtr->next->medManufacturer);
                swap(curPtr->medQuantity, curPtr->next->medQuantity);
                swap(curPtr->medPrice, curPtr->next->medPrice);
                swap(curPtr->expiryDate, curPtr->next->expiryDate);
                swapped = true;
            }
            curPtr = curPtr->next;
        }
        prevPtr = curPtr;
    } while (swapped);
}

// Search a medicine by its ID
Medicine* MedicineInventory::SearchMedicineByID(string medicineID) const {
    Medicine* curPtr = headPtr;
    while (curPtr) {
        if (curPtr->medicineID == medicineID) {
            return curPtr;
        }
        curPtr = curPtr->next;
    }
    return nullptr;
}

// Search a medicine by its medical name
Medicine* MedicineInventory::SearchMedicineByName(string medName) const {
    Medicine* curPtr = headPtr;
    while (curPtr) {
        if (curPtr->medName == medName) {
            return curPtr;
        }
        curPtr = curPtr->next;
    }
    return nullptr;
}

// Menu to allow users to choose their actions when program is executed.
void MedicineInventory::Menu(){
    int choice;
    do {
        cout << "\nMedicine Inventory Menu:\n";
        cout << "1. Create Medicine\n";
        cout << "2. Edit Medicine\n";
        cout << "3. Remove Medicine\n";
        cout << "4. Display Medicines\n";
        cout << "5. Sort Medicines by Name\n";
        cout << "6. Sort Medicines by Quantity\n";
        cout << "7. Search Medicine by ID\n";
        cout << "8. Search Medicine by Name\n";
        cout << "9. Exit\n";

        // Prompt users to input their choice
        cout << "Enter your choice: ";
        while (!(cin >> choice)) { // Input validation
            cout << "Invalid input. Please enter a number between 1 and 9: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Switch case to run the specific operation.
        switch (choice) {
            case 1: {
                string id, name, manufacturer, expiryDate;
                int quantity;
                float price;

                cout << "Enter Medicine ID: ";
                cin >> id;
                cin.ignore();

                cout << "Enter Medicine Name: ";
                getline(cin, name);

                cout << "Enter Manufacturer: ";
                getline(cin, manufacturer);

                cout << "Enter Quantity: ";
                cin >> quantity;
                while (!(cin >> quantity) || quantity < 0) {
                    cout << "Invalid input. Please enter a non-negative integer for quantity: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cout << "Enter Price: ";
                while (!(cin >> price) || price < 0) {
                    cout << "Invalid input. Please enter a non-negative value for price: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore();

                cout << "Enter Expiry Date (YYYY-MM-DD): ";
                while (true) {
                    getline(cin, expiryDate);
                    // Validate format using regex
                    regex dateFormat("^\\d{4}-\\d{2}-\\d{2}$");
                    if (regex_match(expiryDate, dateFormat)) {
                        // Additional validation for valid month and day
                        int year, month, day;
                        sscanf(expiryDate.c_str(), "%d-%d-%d", &year, &month, &day);
                        if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                            break; // Valid date format and reasonable values
                        }
                    }
                    cout << "Invalid format. Please enter the date in YYYY-MM-DD format: ";
                }

                if (CreateMedicine(id, name, manufacturer, quantity, price, expiryDate)) {
                    cout << "Medicine added successfully!\n";
                }
                break;
            }
            case 2: {
                string id, name, manufacturer, expiryDate;
                int quantity;
                float price;

                cout << "Enter Medicine ID to edit: ";
                cin >> id;
                cin.ignore();

                cout << "Enter New Medicine Name: ";
                getline(cin, name);

                cout << "Enter New Manufacturer: ";
                getline(cin, manufacturer);

                cout << "Enter New Quantity: ";
                cin >> quantity;
                while (!(cin >> quantity) || quantity < 0) {
                    cout << "Invalid input. Please enter a non-negative integer for quantity: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cout << "Enter New Price: ";
                cin >> price;
                while (!(cin >> price) || price < 0) {
                    cout << "Invalid input. Please enter a non-negative value for price: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore();

                cout << "Enter New Expiry Date (YYYY-MM-DD): ";
                while (true) {
                    getline(cin, expiryDate);
                    // Validate format using regex
                    regex dateFormat("^\\d{4}-\\d{2}-\\d{2}$");
                    if (regex_match(expiryDate, dateFormat)) {
                        // Additional validation for valid month and day
                        int year, month, day;
                        sscanf(expiryDate.c_str(), "%d-%d-%d", &year, &month, &day);
                        if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                            break; // Valid date format and reasonable values
                        }
                    }
                    cout << "Invalid format. Please enter the date in YYYY-MM-DD format: ";
                }

                if (EditMedicine(id, name, manufacturer, quantity, price, expiryDate)) {
                    cout << "Medicine updated successfully!\n";
                } else {
                    cout << "Error! Medicine not found.\n";
                }
                break;
            }
            case 3: {
                string id;
                cout << "Enter Medicine ID to remove: ";
                cin >> id;

                if (RemoveMedicine(id)) {
                    cout << "Medicine removed successfully!\n";
                } else {
                    cout << "Error! Medicine not found.\n";
                }
                break;
            }
            case 4:
                DisplayMedicines();
                break;

            case 5:
                SortByName();
                cout << "Medicines sorted by name.\n";
                DisplayMedicines();
                break;

            case 6:
                SortByQuantity();
                cout << "Medicines sorted by quantity.\n";
                DisplayMedicines();
                break;

            case 7: {
                string id;
                cout << "Enter Medicine ID to search: ";
                cin >> id;

                Medicine* result = SearchMedicineByID(id);
                if (result) {
                    cout << "Medicine Found:\n"
                         << "ID: " << result->medicineID << "\n"
                         << "Name: " << result->medName << "\n"
                         << "Manufacturer: " << result->medManufacturer << "\n"
                         << "Quantity: " << result->medQuantity << "\n"
                         << "Price: RM" << result->medPrice << "\n"
                         << "Expiry Date: " << result->expiryDate << "\n";
                } else {
                    cout << "Medicine not found.\n";
                }
                break;
            }
            case 8: {
                string name;
                cin.ignore();
                cout << "Enter Medicine Name to search: ";
                getline(cin, name);

                Medicine* result = SearchMedicineByName(name);
                if (result) {
                    cout << "Medicine Found:\n"
                         << "ID: " << result->medicineID << "\n"
                         << "Name: " << result->medName << "\n"
                         << "Manufacturer: " << result->medManufacturer << "\n"
                         << "Quantity: " << result->medQuantity << "\n"
                         << "Price: RM" << result->medPrice << "\n"
                         << "Expiry Date: " << result->expiryDate << "\n";
                } else {
                    cout << "Medicine not found.\n";
                }
                break;
            }
            case 9:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);
}

// Main function of medicine inventory management ADT
int main() {
    MedicineInventory inventory;
    inventory.LoadFromFile(); // Load data from file on startup
    inventory.Menu();         // Call the menu operation
    return 0;
}

#endif
