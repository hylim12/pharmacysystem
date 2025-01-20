/*
TDS6213 Data Structures and Algorithms
Trimester October
Pharmacy Management System C++ Project

Lab 1C - Group 2
1. Eldeena Lim Huey Yinn    1211111904
2. Do Wai Lung              1211109457
3. Doris Heng               1221305730
4. Kong Yi Xuan             1211108301

Medicine Inventory Management ADT
*/

#ifndef MEDICINE_INVENTORY_CPP
#define MEDICINE_INVENTORY_CPP

#include <iostream> // Provide input and output functionality.
#include <fstream>  // Provide file handling operations.
#include <sstream>  // Allow string stream manipulation.
#include <limits>   // Provide numeric limits for data types.
#include <regex>    // Provide pattern matching and validation.
using namespace std;

// Definition of a Medicine node.
struct Medicine {
    string medicineID;
    string medName;
    string medManufacturer;
    int medQuantity;
    float medPrice;
    string expiryDate;
    Medicine* next;

    // Default constructor.
    Medicine() : medicineID(""), medName(""), medManufacturer(""), medQuantity(0), medPrice(0.0), expiryDate(""), next(nullptr) {}

    // Parameterized constructor.
    Medicine(string id, string name, string manufacturer, int quantity, float price, string expiry)
        : medicineID(id), medName(name), medManufacturer(manufacturer), medQuantity(quantity), medPrice(price), expiryDate(expiry), next(nullptr) {}
};

// Medicine Inventory ADT.
class MedicineInventory {
// Private methods for internal used by class to ensure encapsulation.
private:
    Medicine* headPtr; // Pointer to the head of the linked list storing medicine records.

    bool isUniqueID(string medicineID) const;   // Checks if a given medicine ID is unique in the inventory.
    bool isLoading; // Flag to indicate if data is being loaded from the file to prevent unnecessary file writes.
                    // True when loading data from file.

// Public methods for external used by class.
public:
    MedicineInventory() : headPtr(nullptr), isLoading(false){}  // Constructor to initialize the inventory.
    ~MedicineInventory();   // Destructor to free allocated memory.

    // Operations for managing medicines inventory.
    bool CreateMedicine(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate);
    bool EditMedicine(string medicineID, string newName, string newManufacturer, int newQuantity, float newPrice, string newExpiry);
    bool RemoveMedicine(string medicineID);
    void DisplayMedicines() const;

    // Sorting functions.
    void SortByName();
    void SortByQuantity();

    // Menu displayed for user interaction.
    void Menu();

    // File handling functions.
    void SaveToFile(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate);
    void LoadFromFile();

    // Search functions.
    Medicine* SearchMedicineByID(string medicineID) const;
    Medicine* SearchMedicineByName(string medName) const;
};

// Destructor to free memory.
// Using while loop to iterates through the linked list and deletes each node to prevent memory leaks.
MedicineInventory::~MedicineInventory() {
    while (headPtr) {
        Medicine* temp = headPtr;   // Store the current node.
        headPtr = headPtr->next;    // Move to the next node.
        delete temp;                // Delete the current node.
    }
}

// Checks if the medicine ID is unique.
bool MedicineInventory::isUniqueID(string medicineID) const {
    Medicine* curPtr = headPtr; // curPtr points to the head of the linked list to start traversal.

    // Using while loop to iterates through the linked list.
    while (curPtr) {
        // Compare the current node's medicineID with the input ID
        if (curPtr->medicineID == medicineID) {
            return false;   // Return false if a match is found, indicating input ID is not unique.
        }
        curPtr = curPtr->next;  // Move to the next node in the linked list.
    }
    return true;    // Return true id no match is found until the end of linked list,
                    // indicating input ID is unique.
}

// Adds a new medicine to the inventory.
bool MedicineInventory::CreateMedicine(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate) {
    // Check if the medicine ID is unique in the inventory.
    // If the ID is not unique, print an error message and return false.
    // If the ID is unique, skip the if statement and proceed with the following code.
    if (!isUniqueID(medicineID)) {
        cout << "Error! Medicine ID already exists.\n";
        return false;
    }

    // Create a new Medicine object with the provided details.
    Medicine* newMedicine = new Medicine(medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate);
    newMedicine->next = headPtr;    // Link the new Medicine pointer to the head of linked list.
    headPtr = newMedicine;          // Update the head pointer to point to new Medicine node.

    // Save the new medicine to the file only if not loading data.
    if(!isLoading){
        SaveToFile(medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate);
    }

    return true; // Return true to indicate the new medicine is successfully added to the inventory.
}

// Saves a single medicine record to an autogenerated txt. file.
// Example function call: SaveToFile("M001", "Paracetamol", "PharmaCorp", 50, 1.5, "2025-12-15");
// Example record saved: M001,Paracetamol,PharmaCorp,50,1.5,2025-12-15
void MedicineInventory::SaveToFile(string medicineID, string medName, string medManufacturer, int medQuantity, float medPrice, string expiryDate) {
    // Open the file "inventory_data.txt" in append mode
    ofstream outFile("inventory_data.txt", ios::app);

    // Check if the file was successfully opened.
    if (outFile) {
        // Write the medicine details to the file in CSV format.
        outFile << medicineID << "," << medName << "," << medManufacturer << ","
                << medQuantity << "," << medPrice << "," << expiryDate << "\n";
        outFile.close();    // Close the file after writing.
    } else {
        // Print an error message if the file is not opened, record will not be written.
        cout << "Error! Unable to write to file.\n";
    }
}

// Load data from the file storing medicine records.
void MedicineInventory::LoadFromFile() {
    // Open the file "inventory_data.txt" in read mode.
    ifstream inFile("inventory_data.txt");

    // Check if the file exists or can be opened.
    // If not, print a message to indicate no data file is found.
    if (!inFile) {
        cout << "No previous data found. Starting fresh.\n";
        return;
    }

    string line; // Variable to store each line from the file

    // Using while loop to read though the file line by line.
    while (getline(inFile, line)) {
        // Parse the line (medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate).
        stringstream ss(line);
        string medicineID, medName, medManufacturer, expiryDate;
        int medQuantity;
        float medPrice;

        // getline() extract every data fields from the line using ',' as the delimiter.
        getline(ss, medicineID, ',');
        getline(ss, medName, ',');
        getline(ss, medManufacturer, ',');
        // Reads the integer quantity value directly from the stream.
        // ss.ignore() is used to skips over the next delimiter (,) in the stream.
        ss >> medQuantity;
        ss.ignore(1, ',');
        ss >> medPrice;
        ss.ignore(1, ',');
        getline(ss, expiryDate, ',');

        // Calls CreateMedicine to add the parsed record to the linked list.
        CreateMedicine(medicineID, medName, medManufacturer, medQuantity, medPrice, expiryDate);
    }

    isLoading = false;  // Loading complete.
    inFile.close();     // Close the file after reading.
}


// Edits an existing medicine in the inventory.
bool MedicineInventory::EditMedicine(string medicineID, string newName, string newManufacturer, int newQuantity, float newPrice, string newExpiry) {
    // Search for the medicine using ID.
    Medicine* medicine = SearchMedicineByID(medicineID);

    // Print an error message if the medicine is not found.
    // Return false and exits the function.
    if (!medicine) {
        cout << "Error! Medicine not found.\n";
        return false;
    }

    // Update the previous medicine record with new details.
    medicine->medName = newName;
    medicine->medManufacturer = newManufacturer;
    medicine->medQuantity = newQuantity;
    medicine->medPrice = newPrice;
    medicine->expiryDate = newExpiry;

    return true; // Return true indicating the medicine is successfully updated.
}

// Removes a medicine from the inventory.
bool MedicineInventory::RemoveMedicine(string medicineID) {
    // Initialize two pointers: one for traversing the list and one for tracking the previous node.
    Medicine* curPtr = headPtr;     // curPtr points to the current node in the linked list.
                                    // Traverse the linked list from the head node.
    Medicine* prevPtr = nullptr;    // Tracks the previous node in the linked list.

    // Using while loop to iterates through the linked list until the end.
    while (curPtr) {
        // Check if the current node's medicine ID matches the input ID.
        if (curPtr->medicineID == medicineID) {
            // Skip the current node if it is not the head node.
            if (prevPtr) {
                prevPtr->next = curPtr->next;
            } else {
            // Update the head pointer to the next node if the current node is head node.
                headPtr = curPtr->next;
            }
            delete curPtr;  // Delete the current node.
            return true;    // Return true indicating medicine is successfully removed.
        }
        prevPtr = curPtr;       // Set the current node as the previous node.
        curPtr = curPtr->next;  // Move the current node to the next node.
    }

    // If no matching ID found in inventory, print an error message.
    cout << "Error! Medicine ID not found.\n";
    return false;   // Return false indicating the medicine is not removed.
}

// Displays all medicines in the inventory.
void MedicineInventory::DisplayMedicines() const {
    // Check if the inventory is empty.
    // If headPtr is null, print and message and exit the function.
    if (!headPtr) {
        cout << "The inventory is empty.\n";
        return;
    }

    // Start traversing the linked list from the head.
    Medicine* curPtr = headPtr;

    // Print header for medicine inventory.
    cout << "Medicine Inventory:\n";

    // Using while loop to iterate through the linked list.
    // Print the details of current medicine record.
    while (curPtr) {
        cout << "ID: " << curPtr->medicineID << ", Name: " << curPtr->medName
             << ", Manufacturer: " << curPtr->medManufacturer
             << ", Quantity: " << curPtr->medQuantity
             << ", Price: RM" << curPtr->medPrice
             << ", Expiry: " << curPtr->expiryDate << endl;
        curPtr = curPtr->next; // Move to the next medicine record in list.
    }
}

// Sorts medicines by name in ascending order.
void MedicineInventory::SortByName() {
    // Check if the linked list is empty or contains only one node.
    // If headPtr is null or next node is null, no sorting needed.
    // Exit the function.
    if (!headPtr || !headPtr->next) {
        return;
    }

    bool swapped;       // Flag to track whether any swaps were made in the current pass
    Medicine* curPtr;   // curPtr used to traverse the linked list.
    Medicine* prevPtr = nullptr;    //prevPtr used to track the last sorted node.

    // Using do-while loop for bubble sort implementation.
    // During a pass, two adjacent node is compared and swapped if needed until the last node.
    // Continue with next pass until all node is sorted.
    // The last pass is to confirm no swapping occur in the pass,
    // indicating the list is fully sorted.
    do {
        swapped = false;    // Reset the swapped flag for each pass.
        curPtr = headPtr;   // Start from the head of the list.

        // Traverses the list up to the last sorted node prevPtr.
        while (curPtr->next != prevPtr) {
            // Perform swap when current node is large than the next node.
            if (curPtr->medName > curPtr->next->medName){
                swap(curPtr->medicineID, curPtr->next->medicineID);
                swap(curPtr->medName, curPtr->next->medName);
                swap(curPtr->medManufacturer, curPtr->next->medManufacturer);
                swap(curPtr->medQuantity, curPtr->next->medQuantity);
                swap(curPtr->medPrice, curPtr->next->medPrice);
                swap(curPtr->expiryDate, curPtr->next->expiryDate);
                swapped = true;
            }
            curPtr = curPtr->next;  // Move to the next node.
        }
        prevPtr = curPtr;   // Update the last sorted node.
    } while (swapped);      // Repeat the loop until no swaps are made in a pass.
                            // The list is sorted at that time.
}

// Sorts medicines by quantity in ascending order.
void MedicineInventory::SortByQuantity() {
    // Check if the linked list is empty or contains only one node.
    // If headPtr is null or next node is null, no sorting needed.
    // Exit the function.
    if (!headPtr || !headPtr->next) {
        return;
    }

    bool swapped;       // Flag to track whether any swaps were made in the current pass
    Medicine* curPtr;   // curPtr used to traverse the linked list.
    Medicine* prevPtr = nullptr;    //prevPtr used to track the last sorted node.

    // Using do-while loop for bubble sort implementation.
    // During a pass, two adjacent node is compared and swapped if needed until the last node.
    // Continue with next pass until all node is sorted.
    // The last pass is to confirm no swapping occur in the pass,
    // indicating the list is fully sorted.
    do {
        swapped = false;    // Reset the swapped flag for each pass.
        curPtr = headPtr;   // Start from the head of the list.

        // Traverses the list up to the last sorted node prevPtr.
        while (curPtr->next != prevPtr) {
            // Perform swap when current node is large than the next node.
            if (curPtr->medName > curPtr->next->medName){
                swap(curPtr->medicineID, curPtr->next->medicineID);
                swap(curPtr->medName, curPtr->next->medName);
                swap(curPtr->medManufacturer, curPtr->next->medManufacturer);
                swap(curPtr->medQuantity, curPtr->next->medQuantity);
                swap(curPtr->medPrice, curPtr->next->medPrice);
                swap(curPtr->expiryDate, curPtr->next->expiryDate);
                swapped = true;
            }
            curPtr = curPtr->next;  // Move to the next node.
        }
        prevPtr = curPtr;   // Update the last sorted node.
    } while (swapped);      // Repeat the loop until no swaps are made in a pass.
                            // The list is sorted at that time.
}

// Search a medicine from inventory record by its ID.
Medicine* MedicineInventory::SearchMedicineByID(string medicineID) const {
    // curPtr point to head node to traverse the list from start.
    Medicine* curPtr = headPtr;

    // Using while loop to iterate through the linked list until the end.
    while (curPtr) {
        // Check if the current node's medicine ID matches the input ID.
        // If yes, print a message indicating record is found.
        // Return the current node as result.
        if (curPtr->medicineID == medicineID) {
            cout << "The medicine is found!\n";
            return curPtr;
        }
        // Move to the next node until the medicine is found or the end is reached.
        curPtr = curPtr->next;
    }
    return nullptr;
}

// Search a medicine by its medical name.
Medicine* MedicineInventory::SearchMedicineByName(string medName) const {
    // curPtr point to head node to traverse the list from start.
    Medicine* curPtr = headPtr;

    // Using while loop to iterate through the linked list until the end.
    while (curPtr) {
        // Check if the current node's medicine name matches the input name.
        // If yes, print a message indicating record is found.
        // Return the current node as result.
        if (curPtr->medName == medName) {
            cout << "The medicine is found!\n";
            return curPtr;
        }
        // Move to the next node until the medicine is found or the end is reached.
        curPtr = curPtr->next;
    }
    // Move to the next node until the medicine is found or the end is reached.
    return nullptr;
}

// Menu of Medicine Inventory Management ADT.
// Provide an interface for user interaction.
void MedicineInventory::Menu(){
    // Declare users' choice as integer.
    int choice;

    // Using do-while loop to display the list of options for user's action.
    // Repeats the menu until the user chooses to exit.
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

        // Prompt users to input their choice.
        cout << "Enter your choice: ";
        // Input validation to limit user input between 1 to 9.
        // If invalid input is detected, an invalid message is printed.
        // The input errors is cleared using cin validation.
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 9: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Using switch case to run the specific operation.
        switch (choice) {
            /*
            Case 1:
            Calls the CreateMedicine function to add the medicine to the inventory.
            User is prompted to enter the medicine details.
            cin.ignore() is used to clear the input buffer for the next input.
            getline is used to handle multi-word input.
            Input validation is used for integer input to ensure valid value.
            */
            case 1: {
                // Declare variable with appropriate data type.
                // To store the medicine details.
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
                // Check if the input is a non-negative integer.
                // If not, print an invalid message and prompt user to enter the input again.
                while (!(cin >> quantity) || quantity < 0) {
                    cout << "Invalid input. Please enter a non-negative integer for quantity: ";
                    // Clear the error flag on the input stream.
                    cin.clear();
                    // Clear the invalid input in the input buffer.
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cout << "Enter Price: ";
                // Input validation same as the quantity.
                while (!(cin >> price) || price < 0) {
                    cout << "Invalid input. Please enter a non-negative value for price: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore();

                cout << "Enter Expiry Date (YYYY-MM-DD): ";
                // The user input must be in the formate YYYY-MM-DD.
                // Repeatedly loop until a valid input is entered.
                while (true) {
                    getline(cin, expiryDate);
                    // Validate format using a regular expression regex.
                    regex dateFormat("^\\d{4}-\\d{2}-\\d{2}$");
                    // Check if the month and day values are within valid ranges.
                    if (regex_match(expiryDate, dateFormat)) {
                        int year, month, day;
                        sscanf(expiryDate.c_str(), "%d-%d-%d", &year, &month, &day);
                        if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                            break; // Exit the loop if the date is valid.
                        }
                    }
                    // If the input is invalid, prompt the user to enter a valid date.
                    cout << "Invalid format. Please enter the date in YYYY-MM-DD format: ";
                }

                // Attempt to create a new medicine record using the collected data.
                // Print a message if the data is successfully added to the inventory record.
                if (CreateMedicine(id, name, manufacturer, quantity, price, expiryDate)) {
                    cout << "Medicine added successfully!\n";
                }
                break; // Exit the switch case.
            }

            /*
            Case 2:
            Calls the EditMedicine function to modify the medicine's data.
            User is prompted to enter the new medicine details for update purpose.
            cin.ignore() is used to clear the input buffer for the next input.
            getline is used to handle multi-word input.
            Input validation is used for integer input to ensure valid value.
            */
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
                    regex dateFormat("^\\d{4}-\\d{2}-\\d{2}$");
                    if (regex_match(expiryDate, dateFormat)) {
                        int year, month, day;
                        sscanf(expiryDate.c_str(), "%d-%d-%d", &year, &month, &day);
                        if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                            break;
                        }
                    }
                    cout << "Invalid format. Please enter the date in YYYY-MM-DD format: ";
                }

                if (EditMedicine(id, name, manufacturer, quantity, price, expiryDate)) {
                    cout << "Medicine updated successfully!\n";
                } else {
                    cout << "Error! Medicine not found.\n";
                }
                break; // Exit the switch case.
            }

            /*
            Case 3:
            Calls the RemoveMedicine function to remove specific medicine in the inventory record.
            Success or error messages will be printed.
            */
            case 3: {
                string id;
                cout << "Enter Medicine ID to remove: ";
                cin >> id;

                if (RemoveMedicine(id)) {
                    cout << "Medicine removed successfully!\n";
                } else {
                    cout << "Error! Medicine not found.\n";
                }
                break;  // Exit the switch case.
            }

            /*
            Case 4:
            Calls the DisplayMedicines function to show all medicines in the inventory.
            */
            case 4:
                DisplayMedicines();
                break;  // Exit the switch case.

            /* Case 5:
            Calls the SortByName function to sort the record by name in ascending order.
            Calls the DisplayMedicines function to show all sorted medicines in the inventory.
            */
            case 5:
                SortByName();
                cout << "Medicines sorted by name.\n";
                DisplayMedicines();
                break;  // Exit the switch case.

            /* Case 6:
            Calls the SortByQuantity function to sort the record by quantity in ascending order.
            Calls the DisplayMedicines function to show all sorted medicines in the inventory.
            */
            case 6:
                SortByQuantity();
                cout << "Medicines sorted by quantity.\n";
                DisplayMedicines();
                break;  // Exit the switch case.

            /*
            Case 7:
            Calls SearchMedicineByID function to search for specific medicine using its ID.
            If the medicine is found, print all details.
            If the medicine is not found, print a message indicating medicine not found.
            */
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
                break;  // Exit the switch case.
            }

            /*
            Case 8:
            Calls SearchMedicineByName function to search for specific medicine using its name.
            If the medicine is found, print all details.
            If the medicine is not found, print a message indicating medicine not found.
            */
            case 8: {
                string name;
                cin.ignore();
                cout << "Enter Medicine Name to search: ";
                getline(cin, name); // getline is used to handle multi-word input.

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
                break;  // Exit the switch case.
            }

            /*
            Case 9:
            Exit the program with an exiting message.
            */
            case 9:
                cout << "Exiting...\n";
                break;  // Exit the switch case.

            /*
            Default Case:
            Run when the value of choice does not match any explicitly defined cases (case 1-9).
            Print a message indicate invalid choice and prompt user to input again.
            */
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);  // Repeats the menu until the user chooses to exit.
}

// Main function of medicine inventory management ADT
int main() {
    MedicineInventory inventory;    // Create an object of MedicineInventory class to manage the inventory.
    inventory.LoadFromFile();       // Load inventory data from a file during program startup.
    inventory.Menu();               // Call the menu operation for user interaction.

    return 0;   // Return 0 indicating program successfully executed.
}

#endif
