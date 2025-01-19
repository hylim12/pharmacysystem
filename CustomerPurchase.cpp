/*
TDS6213 Data Structures and Algorithms
Trimester October
Pharmacy Management System C++ Project

Lab 1C - Group 2
1. Eldeena Lim Huey Yinn    1211111904
2. Do Wai Lung              1211109457
3. Doris Heng               1221305730
4. Kong Yi Xuan             1211108301

Customer Purchase ADT
*/


#ifndef CUSTOMER_PURCHASE_CPP
#define CUSTOMER_PURCHASE_CPP

#include <iostream>
#include <fstream>
using namespace std;

// Definition of a purchaseItem node
struct purchaseItem{
    string name;
    int quantity;
    float price;
    purchaseItem* next;

// Default constructor
purchaseItem() : name(""), quantity(0), price(0.0), next(nullptr) {}

// Parameterized constructor
purchaseItem(string name, int quantity, float price): name(name), quantity(quantity), price(price),next(nullptr){}
};

// Manual Queue implementation of purchaseItem
class purchaseQueue {
private:

    purchaseItem* frontPtr;
    purchaseItem* backPtr;
    purchaseItem* curPtr;

public:
    purchaseQueue();
    purchaseQueue(const purchaseQueue& pQ);
    ~purchaseQueue();

    bool isEmpty()const;
    void copy(purchaseQueue& targetQueue) const;
    void enqueue(string newName, int newQuantity, float newPrice);
    void dequeue();
    void dequeue(string& frontName, int& frontQuantity, float& frontPrice);
    void display();
};

// Constructor to initialize the queue
purchaseQueue::purchaseQueue():backPtr(NULL),frontPtr(NULL)
{
}

purchaseQueue::purchaseQueue(const purchaseQueue& pQ)
{
    frontPtr = backPtr = nullptr;

    if (pQ.frontPtr == nullptr){
        return;
    }
}

// Destructor to remove all items in the queue.
purchaseQueue::~purchaseQueue()
{
    while(!isEmpty())
        dequeue();
}

// Checks if the queue is empty
bool purchaseQueue::isEmpty()const
{
    return bool(backPtr == NULL);
}

// Copies the content of the queue into a new queue (pass with reference - without modifiying)
void purchaseQueue::copy(purchaseQueue& targetQueue) const {
    targetQueue = purchaseQueue(); // Clear target queue
    for (purchaseItem* curPtr = frontPtr; curPtr; curPtr = curPtr->next) {
        targetQueue.enqueue(curPtr->name, curPtr->quantity, curPtr->price);
    }
}

// Adds a purchaseItem to the back of the queue
void purchaseQueue::enqueue(string newName, int newQuantity, float newPrice)
{
    if (isEmpty()) // If no purchase items in the queue, the item added will be the front of the queue and back of the queue.
    {
        frontPtr = new purchaseItem;
        frontPtr -> name = newName;
        frontPtr -> quantity = newQuantity;
        frontPtr -> price = newPrice;
        frontPtr -> next = NULL;
        backPtr = frontPtr;
    }
    else // If there are purchase items in the queue, add the new item at the back of the queue;
    {
        purchaseItem* newPtr = new purchaseItem;
        newPtr -> name = newName;
        newPtr -> quantity = newQuantity;
        newPtr -> price = newPrice;
        newPtr -> next = NULL;
        backPtr -> next = newPtr;
        backPtr = newPtr;
    }
}

// Removes a purchase item from the front of the queue
void purchaseQueue::dequeue()
{
    if(isEmpty()) // Cannot dequeue if there are no existing entries
        cout << "Error! There are no purchases so far! Delete purchase entry denied.";
    else
    {
        purchaseItem* tempPtr = frontPtr; // Create a temporary pointer for the first node in the queue
        if (frontPtr == backPtr)
        {
            frontPtr = NULL;
            backPtr = NULL;
        }
        else
        {
            tempPtr = frontPtr;
            frontPtr = frontPtr -> next;
            tempPtr -> next = NULL; // Remove the node using temporary pointer from the queue.
            delete tempPtr;
        }
    }
}

// Removes and returns the items at the front of the queue
void purchaseQueue:: dequeue(string& frontName, int& frontQuantity, float& frontPrice)
{
    if (isEmpty()) // Cannot dequeue if there are no existing entries
        cout << "Error! There are no purchases so far! Delete purchase entry denied.";
    else
    {
        // Retrieve the front node of the queue
        frontName = frontPtr -> name;
        frontQuantity = frontPtr -> quantity;
        frontPrice = frontPtr -> price;
        dequeue(); // Remove the front node of the queue
        // Display the removed first node from the queue
        cout << "The first purchase entry in the system is: "
             << frontName << ", "
             << frontQuantity << ", "
             << frontPrice << endl;

    }
}

// Prints all the purchase entries in the system
void purchaseQueue:: display()
{
    curPtr = frontPtr;
    cout << "Purchase entries in queue: [";
    while (curPtr != NULL)
    {
        cout << curPtr -> name << endl;
        cout << curPtr -> quantity << endl;
        cout << curPtr -> price << endl;
        curPtr = curPtr -> next;
    }
    cout << "]\n";
}

// Definition of PurchaseNode representing a purchase in a linked list
struct PurchaseNode{
    string purchaseID; // Unique identifier for the purchase
    string customerID;
    float totalAmount;
    purchaseQueue items; // Custom queue containing items in the purchase
    PurchaseNode* next; // Pointer to the next node in the linked list

    // Constructor to initialize the purchase node
    PurchaseNode (string pID, string cID, purchaseQueue item, float amount):
        purchaseID(pID), customerID(cID), items(item), totalAmount(amount), next(nullptr) {}
};

class CustomerPurchase{
private:
    PurchaseNode* headPtr; // Pointer to the head of the linked list
    bool newPurchaseID(string purchaseID);
    PurchaseNode* findPurchase(string purchaseID) const;

public:
    CustomerPurchase() : headPtr(nullptr){}
    ~CustomerPurchase();

    void AddPurchase();
    void EditPurchase();
    void DeletePurchase();
    void DisplayPurchase() const;

    void SortPurchases(bool ascending);
    void saveToFile();
    void loadFromFile();
    void Menu();
};

// Function to check if a purchase ID is unique in the list
bool CustomerPurchase::newPurchaseID(string purchaseID){
    PurchaseNode* curPtr = headPtr;
    while (curPtr){
        if (curPtr -> purchaseID == purchaseID){
            return false; // Purchase ID already exists
        }
        curPtr = curPtr -> next;
    }
    return true; // Purchase ID is unique
}

//  Finds a purchase node by its ID
PurchaseNode* CustomerPurchase::findPurchase(string purchaseID)const {
    PurchaseNode* curPtr = headPtr;
    while (curPtr){
        if (curPtr -> purchaseID == purchaseID){
            return curPtr; // Found the purchase node
        }
        curPtr = curPtr -> next;
    } return nullptr; // Purchase ID is not found
}

// Deletes all nodes in the linked list to free memory
CustomerPurchase::~CustomerPurchase(){
    while (headPtr){
        PurchaseNode* temp = headPtr;
        headPtr = headPtr -> next;
        delete temp; // Deletes the current node
    }
}

// Function to save customer purchase data to a file
void CustomerPurchase::saveToFile() {
    ofstream file("purchases.txt");
    if (!file) {
        cout << "Error! Unable to save purchase entry.\n";
        return;
    }

    PurchaseNode* curPtr = headPtr;
    // Save customer purchase data and items purchased
    while (curPtr) {
        file << curPtr->purchaseID << "\n"
             << curPtr->customerID << "\n";

        // Save all items in the purchase
        purchaseQueue tempQueue;
        curPtr->items.copy(tempQueue); // Copy the purchaseQueue to preserve original data
        int count = 0;

        // Save item details (name, quantity, price)
        while (!tempQueue.isEmpty()) {
            string itemName;
            int itemQuantity;
            float itemPrice;

            tempQueue.dequeue(itemName, itemQuantity, itemPrice);
            file << itemName << " " << itemQuantity << " " << itemPrice << "\n";
            count++;
        }

        file << count << "\n"; // Save the number of items in the purchase
        file << curPtr->totalAmount << "\n"; // Save total cost of the purchase

        curPtr = curPtr->next;
    }

    file.close();
    cout << "Purchases saved successfully.\n";
}

// Function to load customer purchase data from a file
void CustomerPurchase::loadFromFile() {
    ifstream file("purchases.txt");
    if (!file) {
        return;
    }

    while (!file.eof()) {
        string pID, cID;
        int itemCount;
        purchaseQueue item;
        float amount;

        // Read purchase ID, customer ID, total amount and item count
        file >> pID >> cID >> amount >> itemCount;
        if (file.fail()) break; // Stop reading if file format is incorrect

        cout << "Purchase ID: " << pID << "\n";
        cout << "Customer ID: " << cID << "\n";
        cout << "Total Amount: " << amount << "\n";
        cout << "Items:\n";

        // Load all items into the purchaseQueue
        for (int i = 0; i < itemCount; ++i) {
            string itemName;
            int itemQuantity;
            float itemPrice;

            file.ignore(); // Ignore leftover newline from the previous read
            file >> itemName >> itemQuantity >> itemPrice;
            if (file.fail()) break; // Stop if file format is invalid

            // Enqueue item to the items queue
            item.enqueue(itemName, itemQuantity, itemPrice);

            // Display the item details
            cout << "  Item Name: " << itemName << ", Quantity: " << itemQuantity
                 << ", Price: " << itemPrice << "\n";
        }

        // Display the items in the purchase queue
        cout << "Total Items: " << itemCount << "\n\n";

        // Create a new purchase node with the loaded data
        PurchaseNode* newPurchase = new PurchaseNode(pID, cID, item, amount);
        newPurchase->next = headPtr;  // Make list circular
        headPtr = newPurchase;
    }

    file.close();
    cout << "Data loaded successfully from file.\n";
}

// Adds a new purchase record to the linked list
void CustomerPurchase::AddPurchase() {
    string pID, cID;
    purchaseQueue item;
    char choice = 'y';
    float amount;

    cout << "Enter Purchase ID: ";
    getline(cin, pID);

    if (!newPurchaseID(pID)) {
        cout << "Error! This is an existing purchase. Insert a new purchase ID\n";
        return; // Purchase ID already exists
    }

    cout << "Enter Customer ID: ";
    getline(cin, cID);

    // Loop to add items to the purchase
    while (tolower(choice) == 'y') {
        string itemName;
        int itemQuantity;
        float itemPrice;

        cout << "\nItem Details\n ";
        cout << "\tEnter Item Name: ";
        getline(cin, itemName);

        cout << "\tEnter Item Quantity: ";
        cin >> itemQuantity;

        cout << "\tEnter Item Price: ";
        cin >> itemPrice;
        cin.ignore(); // Clear input buffer

        // Add the item to the purchaseQueue
        item.enqueue(itemName, itemQuantity, itemPrice);

        // Update total amount of the purchase
        amount += itemPrice * itemQuantity;

        cout << "Add another item? (Y/N): ";
        cin >> choice;
        cin.ignore(); // Clear input buffer
    }

    // Create a new purchase node and add to the front of the list
    PurchaseNode* newPurchase = new PurchaseNode(pID, cID, item, amount);
    newPurchase->next = headPtr;
    headPtr = newPurchase;

    saveToFile();
    cout << "Purchase entry added successfully\n";
}

// Modify an existing purchase record's details
void CustomerPurchase::EditPurchase(){
    string pID;
    purchaseQueue newItems;
    float amount = 0.0f;

    cout << "Enter Purchase ID to modify: ";
    cin >> pID;

    PurchaseNode* purchase = findPurchase(pID);
    if(!purchase){
        cout << "Error! The has been no such purchase.\n";
        return; // Purchase ID does not exist
    }

    cin.ignore(); // Clear input buffer
    cout << "Modifying Purchase ID: " << pID << "\n";
    cout << "Current items in this purchase: \n";

    purchaseQueue tempQueue = purchase->items;
    string itemName;
    int itemQuantity;
    float itemPrice;

    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(itemName, itemQuantity, itemPrice);
        cout << "Item Name: " << itemName << ", Quantity: " << itemQuantity
             << ", Price: " << itemPrice << "\n";
    }

   cout << "Enter Item Name to edit (leave blank to skip): ";
    getline(cin, itemName);

    if (!itemName.empty()) {
        cout << "Enter new Item Quantity: ";
        cin >> itemQuantity;

        cout << "Enter new Item Price: ";
        cin >> itemPrice;
        cin.ignore(); // Clear input buffer

        // Add or update the item in the purchaseQueue
        purchase->items.enqueue(itemName, itemQuantity, itemPrice);

        // Update total amount of the purchase
        amount += itemPrice * itemQuantity;
    }

    purchase -> totalAmount = amount; // Update the total amount to the updated total cost
    saveToFile();
    cout << "Purchase entry updated successfully\n";
}

// Deletes a purchase record by its ID
void CustomerPurchase::DeletePurchase(){
    string pID;
    cout << "Enter Purchase ID to remove: ";
    getline(cin, pID);

    PurchaseNode* curPtr = headPtr;
    PurchaseNode* prevPtr = nullptr;

    while (curPtr && curPtr -> purchaseID != pID){
            prevPtr = curPtr;
            curPtr = curPtr -> next;
    }

    if (!curPtr){
        cout << "Error! Purchase entry does not exist in the system. \n";
        return;
    }

    if (prevPtr)
    {
        prevPtr -> next = curPtr -> next;
    }
    else {
        headPtr = curPtr -> next;
    }

    delete curPtr;
    saveToFile();
    cout << "Purchase entry deleted successfully\n";
}

// Displays the details of a specific purchase record
void CustomerPurchase::DisplayPurchase () const{
    if (!headPtr){
        cout << "No purchase entries in the system!";
        return;
    }

    string pID;
    cout << "Enter purchase ID to display: ";
    cin >> pID;

    PurchaseNode* curPtr = headPtr;
    bool found = false;
     do {
        if (curPtr->purchaseID == pID) {
            // Display the purchase details
            cout << "\nPurchase ID: " << curPtr->purchaseID << endl;
            cout << "Customer ID: " << curPtr->customerID << endl;
            cout << "Items Purchased:\n";

            // Display all items in the current purchase
            purchaseQueue tempItem = curPtr->items;
            string itemName;
            int itemQuantity;
            float itemPrice;

            while (!tempItem.isEmpty()) {
                tempItem.dequeue(itemName, itemQuantity, itemPrice);
                cout << "Item Name: " << itemName << ", Quantity: " << itemQuantity
                     << ", Price: " << itemPrice << endl;
            }

            // Display the total amount of the purchase
            cout << "Total Amount: " << curPtr->totalAmount << endl;

            found = true;
            break; // Exit loop once the matching purchase is found
        }
        curPtr = curPtr->next;
    } while (curPtr != headPtr); // Stop when we complete the circular loop

    if (!found) {
        cout << "No purchase found with ID: " << pID << endl;
    }
}
// Bubble sort to sort purchases by totalAmount in ascending or descending order
void CustomerPurchase::SortPurchases(bool ascending) {
    if (headPtr == nullptr || headPtr->next == nullptr) {
        // If the list is empty or has only one purchase, no sorting is needed
        return;
    }

    bool swapped;
    PurchaseNode* curPtr;
    PurchaseNode* prevPtr = nullptr;

    do {
        swapped = false;
        curPtr = headPtr;

        while (curPtr->next != prevPtr) {
            bool Swap = ascending ? (curPtr->totalAmount > curPtr->next->totalAmount)
                                        : (curPtr->totalAmount < curPtr->next->totalAmount);

            if (Swap) {
                // Swap the purchases if they are in the wrong order
                string tempPurchaseID = curPtr->purchaseID;
                string tempCustomerID = curPtr->customerID;
                float tempTotalAmount = curPtr->totalAmount;
                purchaseQueue tempItems = curPtr->items;

                curPtr->purchaseID = curPtr->next->purchaseID;
                curPtr->customerID = curPtr->next->customerID;
                curPtr->totalAmount = curPtr->next->totalAmount;
                curPtr->items = curPtr->next->items;

                curPtr->next->purchaseID = tempPurchaseID;
                curPtr->next->customerID = tempCustomerID;
                curPtr->next->totalAmount = tempTotalAmount;
                curPtr->next->items = tempItems;

                swapped = true;
            }
            curPtr = curPtr->next;
        }
        prevPtr = curPtr;
    } while (swapped);
}


// Function to manage customer purchases
void CustomerPurchase::Menu() {
    int choice;
    do {
        cout << "\n--- Customer Purchase Management ---\n";
        cout << "1. Add Purchase\n";
        cout << "2. Edit Purchase\n";
        cout << "3. Delete Purchase\n";
        cout << "4. Display Purchase\n";
        cout << "5. Sort Purchases\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To ignore the newline left by previous input

        switch (choice) {
            case 1: {
                AddPurchase();  // Call the AddPurchase function
                break;
            }
            case 2: {
                EditPurchase();
                break;
            }
            case 3: {
                DeletePurchase();
                break;
            }
            case 4: {
                DisplayPurchase();
                break;
            }
            case 5: {
                bool ascending;
                cout << "Sort by total amount? (1 for ascending, 0 for descending): ";
                cin >> ascending;  // Get the sort order
                SortPurchases(ascending);  // Call the SortPurchases function
                cout << "Purchases sorted successfully.\n";
                break;
            }
            case 6: {
                cout << "Returning to the main menu.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);  // Loop until the user chooses to exit (choice 6)
}

int main() {
    CustomerPurchase custPur;
    custPur.Menu(); // Handle customer purchases management
    return 0;
}

#endif
