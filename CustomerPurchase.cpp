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
    void getFront(string& frontName, int& frontQuantity, float& frontPrice);
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

// Retrieves a purchase item from the front of the queue
void purchaseQueue:: getFront(string& frontName, int& frontQuantity, float& frontPrice)
{
    if (isEmpty()) // Cannot display first node if no purchases have been made
        cout << "Error! There are no purchases so far! Retrieve first purchase entry unsuccessful.";
    else
    {
        // Retrieve the first node of the queue
        frontName = frontPtr -> name;
        frontQuantity = frontPtr -> quantity;
        frontPrice = frontPtr -> price;
        // Display the first node from the queue
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

class CustomerPurchase{
private:
    // Definition of PurchaseNode representing a purchase in a linked list
    struct PurchaseNode{
        int purchaseID; // Unique identifier for the purchase
        int customerID;
        float totalAmount;
        purchaseQueue items; // Custom queue containing items in the purchase
        PurchaseNode* next; // Pointer to the next node in the linked list

        // Constructor to initialize the purchase node
        PurchaseNode (int pID, int cID, purchaseQueue item, float amount):
            purchaseID(pID), customerID(cID), items(item), totalAmount(amount), next(nullptr) {}
    };

    PurchaseNode* headPtr; // Pointer to the head of the linked list
    bool newPurchaseID(int purchaseID);
    PurchaseNode* findPurchase(int purchaseID) const;

public:
    CustomerPurchase() : headPtr(nullptr){}
    ~CustomerPurchase();

    bool AddPurchase(int purchaseID, int customerID, purchaseQueue items);
    bool EditPurchase(int purchaseID, purchaseQueue newItem);
    bool DeletePurchase(int purchaseID);
    void DisplayPurchase(int purchaseID) const;

    void SortPurchases(bool ascending);
};

// Function to check if a purchase ID is unique in the list
bool CustomerPurchase::newPurchaseID(int purchaseID){
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
CustomerPurchase::PurchaseNode* CustomerPurchase::findPurchase(int purchaseID)const {
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

// Adds a new purchase record to the linked list
bool CustomerPurchase::AddPurchase(int purchaseID, int customerID, purchaseQueue items) {
    if (!newPurchaseID(purchaseID)) {
        cout << "Error! This is an existing purchase. Insert a new purchase ID\n";
        return false; // Purchase ID already exists
    }

    // Calculate the total amount of the purchase
    float total = 0.0f;
    purchaseQueue tempItem;
    items.copy(tempItem); // Copy the queue to a temporary queue

    while (!tempItem.isEmpty()) {
        string itemName;
        int itemQuantity;
        float itemPrice;

        tempItem.dequeue(itemName, itemQuantity, itemPrice); // Remove the first node of the temporary queue
        total += itemPrice * itemQuantity; // Add each item cost to the total
    }

    // Create a new purchase node and add to the front of the list
    PurchaseNode* newPurchase = new PurchaseNode(purchaseID, customerID, items, total);
    newPurchase->next = headPtr;
    headPtr = newPurchase;

    return true; // Successfully added
}

// Modify an existing purchase record's details
bool CustomerPurchase::EditPurchase(int purchaseID, purchaseQueue newItems){
    PurchaseNode* purchase = findPurchase(purchaseID);
    if(!purchase){
        cout << "Error! The has been no such purchase.\n";
        return false; // Purchase ID does not exist
    }

    // Update the items in the purchase node
    purchase -> items = newItems;

    // Recalculate the total cost for the purchase
    float total = 0.0f;
    purchaseQueue tempItem;
    newItems.copy(tempItem); // Copy the queue to a temporary queue

    while (!tempItem.isEmpty()){
        string itemName;
        int itemQuantity;
        float itemPrice;

        tempItem.dequeue(itemName, itemQuantity, itemPrice); // Remove the first node of the temporary queue
        total = itemPrice * itemQuantity; // Add each item cost to the total
    }
    purchase -> totalAmount = total; // Update the total amount to the updated total cost
    return true; // Successfully edited;
}

// Deletes a purchase record by its ID
bool CustomerPurchase::DeletePurchase(int purchaseID){
    PurchaseNode* curPtr = headPtr;
    PurchaseNode* prevPtr = nullptr;

    while (curPtr){
        if (curPtr -> purchaseID == purchaseID){
            // Removes the node from the list if it matches the given purchaseID
            if (prevPtr){
                prevPtr -> next = curPtr -> next; // Bypass the current node
            } else {
                headPtr = curPtr -> next; // Update the head pointer to the node after the first node
            }
            delete curPtr; // Free memory for the deleted node
            return true; // Successfully deleted
        }
        prevPtr = curPtr;
        curPtr = curPtr -> next;
    }
    cout << "Error! Purchase entry does not exist in the system.\n";
    return false; // Purchase ID is invalid or not found
}

// Displays the details of a specific purchase record
void CustomerPurchase::DisplayPurchase (int purchaseID) const{
    PurchaseNode* purchase = findPurchase(purchaseID);
    if (!purchase){
        cout << "Error! The has been no such purchase.\n";
        return; // Purchase ID does not exist
    }

    // Display purchase details
    cout << "Purchase ID: " << purchase -> purchaseID << endl;
    cout << "Customer ID: " << purchase -> customerID << endl;
    cout << "Items: \n";

    // Display all items in the purchase
    purchaseQueue tempItem;
    purchase -> items.copy(tempItem);
    while (!tempItem.isEmpty()) {
        string itemName;
        int itemQuantity;
        float itemPrice;

        tempItem.dequeue(itemName, itemQuantity, itemPrice);
        cout << "Name: " << itemName << endl;
        cout << "Quantity: " << itemQuantity << endl;
        cout << "Price: " << itemPrice << endl;
    }

    // Display the total amount of all items in a purchase
    cout << "Total Amount: " << purchase -> totalAmount << endl;
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
                int tempPurchaseID = curPtr->purchaseID;
                int tempCustomerID = curPtr->customerID;
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


#endif
