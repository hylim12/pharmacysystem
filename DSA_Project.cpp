#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Structure to store customer details
struct Customer {
    int customerID;
    string custName;
    string custContact;
    string custEmail;
    string custAddress;
    Customer* next;

    Customer(int id, string name, string contact, string email, string address)
        : customerID(id), custName(name), custContact(contact), custEmail(email), custAddress(address), next(nullptr) {}
};

class CustomerManagementADT {
private:
    Customer* head; // Pointer to the head of the linked list

    // Function to find a customer by ID
    Customer* findCustomer(int customerID) {
        Customer* current = head;
        while (current) {
            if (current->customerID == customerID) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Function to save customer data to a file
    void saveToFile() {
        ofstream file("customers.txt");
        if (!file) {
            cout << "Error: Unable to save customer data.\n";
            return;
        }

        Customer* current = head;
        while (current) {
            file << current->customerID << "\n"
                 << current->custName << "\n"
                 << current->custContact << "\n"
                 << current->custEmail << "\n"
                 << current->custAddress << "\n";
            current = current->next;
        }
        file.close();
    }

    // Function to load customer data from a file
    void loadFromFile() {
        ifstream file("customers.txt");
        if (!file) {
            cout << "No existing data found. Starting fresh.\n";
            return;
        }

        while (!file.eof()) {
            int id;
            string name, contact, email, address;

            file >> id;
            file.ignore();
            getline(file, name);
            getline(file, contact);
            getline(file, email);
            getline(file, address);

            if (file.fail()) break; // Stop reading if file format is incorrect

            Customer* newCustomer = new Customer(id, name, contact, email, address);
            newCustomer->next = head;
            head = newCustomer;
        }
        file.close();
    }

public:
    // Constructor to initialize the system and load existing data
    CustomerManagementADT() : head(nullptr) {
        loadFromFile();
    }

    // Function to add a new customer
    void CreateCustomer() {
        int id;
        string name, contact, email, address;

        cout << "Enter Customer ID: ";
        cin >> id;

        if (findCustomer(id)) {
            cout << "Error: Customer ID already exists.\n";
            return;
        }

        cin.ignore(); // Clear input buffer
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Contact: ";
        getline(cin, contact);
        cout << "Enter Email: ";
        getline(cin, email);
        cout << "Enter Address: ";
        getline(cin, address);

        Customer* newCustomer = new Customer(id, name, contact, email, address);
        newCustomer->next = head;
        head = newCustomer;

        saveToFile();
        cout << "Customer added successfully.\n";
    }

    // Function to edit an existing customer
    void EditCustomer() {
        int id;
        cout << "Enter Customer ID to edit: ";
        cin >> id;

        Customer* customer = findCustomer(id);
        if (!customer) {
            cout << "Error: Customer not found.\n";
            return;
        }

        cin.ignore(); // Clear input buffer
        cout << "Enter new Name: ";
        getline(cin, customer->custName);
        cout << "Enter new Contact: ";
        getline(cin, customer->custContact);
        cout << "Enter new Email: ";
        getline(cin, customer->custEmail);
        cout << "Enter new Address: ";
        getline(cin, customer->custAddress);

        saveToFile();
        cout << "Customer updated successfully.\n";
    }

    // Function to remove a customer by ID
    void RemoveCustomer() {
        int id;
        cout << "Enter Customer ID to remove: ";
        cin >> id;

        Customer* current = head;
        Customer* previous = nullptr;

        while (current && current->customerID != id) {
            previous = current;
            current = current->next;
        }

        if (!current) {
            cout << "Error: Customer not found.\n";
            return;
        }

        if (previous) {
            previous->next = current->next;
        } else {
            head = current->next;
        }

        delete current;
        saveToFile();
        cout << "Customer removed successfully.\n";
    }

    // Function to display a single customer's details
    void DisplayCustomer() {
        int id;
        cout << "Enter Customer ID to display: ";
        cin >> id;

        Customer* customer = findCustomer(id);
        if (!customer) {
            cout << "Error: Customer not found.\n";
            return;
        }

        cout << "Customer ID: " << customer->customerID << "\n"
             << "Name: " << customer->custName << "\n"
             << "Contact: " << customer->custContact << "\n"
             << "Email: " << customer->custEmail << "\n"
             << "Address: " << customer->custAddress << "\n";
    }

    // Function to display all customers in the system
    void DisplayAllCustomers() {
        if (!head) {
            cout << "No customers to display.\n";
            return;
        }

        Customer* current = head;
        while (current) {
            cout << "Customer ID: " << current->customerID << " | Name: " << current->custName << "\n";
            current = current->next;
        }
    }

    // Function to display the main menu
    void Menu() {
        int choice;
        do {
            cout << "\nCustomer Management Menu:\n"
                 << "1. Create Customer\n"
                 << "2. Edit Customer\n"
                 << "3. Remove Customer\n"
                 << "4. Display Customer\n"
                 << "5. Display All Customers\n"
                 << "6. Exit\n"
                 << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    CreateCustomer();
                    break;
                case 2:
                    EditCustomer();
                    break;
                case 3:
                    RemoveCustomer();
                    break;
                case 4:
                    DisplayCustomer();
                    break;
                case 5:
                    DisplayAllCustomers();
                    break;
                case 6:
                    cout << "Exiting system.\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } while (choice != 6);
    }

    // Destructor to clean up the linked list
    ~CustomerManagementADT() {
        while (head) {
            Customer* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    CustomerManagementADT cm;
    cm.Menu();
    return 0;
}
