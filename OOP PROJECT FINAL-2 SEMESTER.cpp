#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits> // for numeric_limits
#include <cstdlib> // for system("CLS") and Sleep(milliseconds)
#include <windows.h> // for Sleep(milliseconds)
using namespace std;

class VENUE {
public:
    VENUE() : VenueId(1234), Name("ABC Event"), Location("Lahore") {}

    void GetVenueDetails() {
        cout << "\t\t\t\t\tEnter Venue ID: ";
        cin >> VenueId;
        while (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
            cout << "\t\t\t\t\tInvalid input. Please enter a valid Venue ID: ";
            cin >> VenueId;
        }
        cin.ignore(); // clear the newline character from the buffer
        cout << "\t\t\t\t\tEnter Venue Name: ";
        getline(cin, Name);
        cout << "\t\t\t\t\tEnter Venue Location: ";
        getline(cin, Location);
    }

    void DisplayVenueDetails() const {
        cout << "\t\t\t\t\tVenue ID: " << VenueId << "\n\t\t\t\t\tVenue Name: " << Name << "\n\t\t\t\t\tVenue Location: " << Location << endl;
    }

    void ChangeVenueDetails(const string& newName, const string& newLocation) {
        Name = newName;
        Location = newLocation;
    }

    void LoadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            file >> VenueId;
            file.ignore();
            getline(file, Name);
            getline(file, Location);
            file.close();
        } else {
            cerr << "\t\t\t\t\tUnable to open venue file." << endl;
        }
    }

    void SaveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << VenueId << "\n" << Name << "\n" << Location << endl;
            file.close();
        } else {
            cerr << "\t\t\t\t\tUnable to save venue to file." << endl;
        }
    }

private:
    int VenueId;
    string Name;
    string Location;
};

class EVENT {
public:
    EVENT() : EventId(1234), EventName("Concert"), EventDate("5/6/2024"), venue(), capacity(500), price(50) {}

    void DisplayEventDetails() const {
        cout << "\t\t\t\t\tEvent ID: " << EventId << "\n\t\t\t\t\tEvent Name: " << EventName << "\n\t\t\t\t\tEvent Date: " << EventDate << endl;
        venue.DisplayVenueDetails();
        cout << "\t\t\t\t\tEvent Capacity: " << capacity << "\n\t\t\t\t\tEvent Price: " << price << endl;
    }

    void ChangePrice(double newPrice) { price = newPrice; }
    void ChangeEventName(const string &newName) { EventName = newName; }
    void ChangeCapacity(int newCapacity) { capacity = newCapacity; }
    void ChangeEventDate(const string& newDate) { EventDate = newDate; }
    void ChangeVenueDetails(const string& newName, const string& newLocation) { venue.ChangeVenueDetails(newName, newLocation); }

    void LoadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            file >> EventId;
            file.ignore();
            getline(file, EventName);
            getline(file, EventDate);
            file >> capacity >> price;
            file.close();
            venue.LoadFromFile("venue.txt");
        } else {
            cerr << "\t\t\t\t\tUnable to open event file." << endl;
        }
    }

    void SaveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << EventId << "\n" << EventName << "\n" << EventDate << "\n" << capacity << "\n" << price << endl;
            file.close();
            venue.SaveToFile("venue.txt");
        } else {
            cerr << "\t\t\t\t\tUnable to save event to file." << endl;
        }
    }

private:
    int EventId;
    string EventName;
    string EventDate;
    VENUE venue;
    int capacity;
    double price;
};

class CUSTOMER {
public:
    CUSTOMER() : CustomerId(nextId++) {}

    void Register() {
        cout << "\t\t\t\t\tEnter Your Name: ";
        cin.ignore();
        getline(cin, Name);
    }

    int GetCustomerId() const { return CustomerId; }
    string GetName() const { return Name; }

private:
    static int nextId;
    int CustomerId;
    string Name;
};

int CUSTOMER::nextId = 1;

class TICKET {
public:
    TICKET(int id, double price, const string &type, const string &customerName)
        : TicketId(id), Price(price), Type(type), CustomerName(customerName) {}

    void DisplayTicketDetails() const {
        cout << "\t\t\t\t\tTicket ID: " << TicketId << "\n\t\t\t\t\tPrice: " << Price << "\n\t\t\t\t\tType: " << Type << "\n\t\t\t\t\tCustomer Name: " << CustomerName << endl;
    }

    double GetPrice() const { return Price; }
    string GetType() const { return Type; }
    string GetCustomerName() const { return CustomerName; }
    void ChangePrice(double newPrice) { Price = newPrice; }

    void LoadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            file >> TicketId >> Price;
            file.ignore();
            getline(file, Type);
            getline(file, CustomerName);
            file.close();
        } else {
            cerr << "\t\t\t\t\tUnable to open ticket file." << endl;
        }
    }

    void SaveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << TicketId << "\n" << Price << "\n" << Type << "\n" << CustomerName << endl;
            file.close();
        } else {
            cerr << "\t\t\t\t\tUnable to save ticket to file." << endl;
        }
    }

private:
    int TicketId;
    double Price;
    string Type;
    string CustomerName;
};

class ORDER {
public:
    ORDER(int id, const EVENT &evt) : OrderId(id), event(evt) {}

    void PlaceOrder(vector<TICKET> &eventTickets) {
        cout << "\t\t\t\t\tHow many tickets do you want to book? ";
        int numTickets;
        while (!(cin >> numTickets) || numTickets <= 0 || numTickets > 10) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
            cout << "\t\t\t\t\tInvalid input. Please enter a valid number of tickets (1-10): ";
        }
        cin.ignore(); // clear the newline character from the buffer

        cout << "\n\t\t\t\t\t      ---(Available Tickets)---     \n";
        cout << "\t\t\t\t\t**************************************\n";
        cout << "\t\t\t\t\t|   No.   |     Type    |    Price    |\n";
        cout << "\t\t\t\t\t**************************************\n";
        for (size_t i = 0; i < eventTickets.size(); ++i) {
            cout << "\t\t\t\t\t| " << i + 1 << "  \t  |   "<< eventTickets[i].GetType() << "   |     $" << eventTickets[i].GetPrice() << "     |\n";
        }
        cout << "\t\t\t\t\t**************************************\n";

        for (int i = 0; i < numTickets; ++i) {
            int choice;
            cout << "\t\t\t\t\t   Enter choice for Ticket " << i + 1 << ": ";
            while (!(cin >> choice) || choice < 1 || choice > eventTickets.size()) {
                cin.clear(); // clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
                cout << "\t\t\t\t\tInvalid choice. Please select again: ";
            }
            system("CLS");
            cin.ignore(); // clear the newline character from the buffer

            string customerName;
            cout << "\t\t\t\t\tEnter name for Ticket " << i + 1 << ": ";
            getline(cin, customerName);

            TICKET ticket(i + 1, eventTickets[choice - 1].GetPrice(), eventTickets[choice - 1].GetType(), customerName);
            tickets.push_back(ticket);
        }
    }

    void ViewOrderDetails() const {
        cout << "\t\t\t\t\tOrder ID: " << OrderId << endl;
        event.DisplayEventDetails();
        cout << "\t\t\t\t\tTickets Purchased:\n";
        for (const auto &ticket : tickets) {
            ticket.DisplayTicketDetails();
        }
    }

    double CalculateTotalPrice() const {
        double total = 0;
        for (const auto &ticket : tickets) {
            total += ticket.GetPrice();
        }
        return total;
    }

    const vector<TICKET>& GetTickets() const { return tickets; }

    void LoadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            file >> OrderId;
            file.close();
            event.LoadFromFile("event.txt");
            for (TICKET& ticket : tickets) {
                ticket.LoadFromFile("ticket.txt");
            }
        } else {
            cerr << "\t\t\t\t\tUnable to open order file." << endl;
        }
    }

    void SaveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << OrderId << endl;
            file.close();
            event.SaveToFile("event.txt");
            for (const TICKET& ticket : tickets) {
                ticket.SaveToFile("ticket.txt");
            }
        } else {
            cerr << "\t\t\t\t\tUnable to save order to file." << endl;
        }
    }

private:
    int OrderId;
    EVENT event;
    vector<TICKET> tickets;
};

class ADMIN {
public:
    bool Login() {
        string inputPassword, inputPin;
        cout << "\t\t\t\t\tEnter Password: ";
        cin >> inputPassword;
        cout << "\t\t\t\t\tEnter PIN: ";
        cin >> inputPin;
        if (inputPassword == password && inputPin == pin) {
            cout << "\t\t\t\t\tLogged in successfully.\n";
            system("CLS");
            return true;
        } else {
            cout << "\t\t\t\t\tInvalid credentials. Access denied.\n";
            return false;
        }
    }

    void ChangeTicketPrices(vector<TICKET> &tickets) {
        for (const auto &ticket : tickets) {
            ticket.DisplayTicketDetails();
        }

        cout << "\t\t\t\t\tEnter new prices for tickets:\n";
        for (auto &ticket : tickets) {
            double newPrice;
            cout << "\t\t\t\t\tEnter new price for Ticket Type " << ticket.GetType() << ": ";
            while (!(cin >> newPrice) || newPrice <= 0) {
                cin.clear(); // clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
                cout << "\t\t\t\t\tInvalid input. Please enter a valid price (> 0): ";
            }
            ticket.ChangePrice(newPrice);
        }

        cout << "\t\t\t\t\tTicket prices updated successfully.\n";
    }

    void ChangeEventDetails(EVENT &event) {
        string newName, newDate, newVenueName, newLocation;
        int newCapacity;
        double newPrice;

        cout << "\t\t\t\t\tPrevious Event Details:\n";
        event.DisplayEventDetails();

        cout << "\t\t\t\t\tEnter new event name: ";
        cin.ignore();
        getline(cin, newName);
        cout << "\t\t\t\t\tEnter new event date (YYYY-MM-DD): ";
        getline(cin, newDate);
        cout << "\t\t\t\t\tEnter new venue name: ";
        getline(cin, newVenueName);
        cout << "\t\t\t\t\tEnter new venue location: ";
        getline(cin, newLocation);
        cout << "\t\t\t\t\tEnter new event capacity: ";
        while (!(cin >> newCapacity) || newCapacity <= 0) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
            cout << "\t\t\t\t\tInvalid input. Please enter a valid capacity (> 0): ";
        }
        cout << "\t\t\t\t\tEnter new event price: ";
        while (!(cin >> newPrice) || newPrice <= 0) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
            cout << "\t\t\t\t\tInvalid input. Please enter a valid price (> 0): ";
        }

        event.ChangeEventName(newName);
        event.ChangeEventDate(newDate);
        event.ChangeCapacity(newCapacity);
        event.ChangePrice(newPrice);
        event.ChangeVenueDetails(newVenueName, newLocation);

        cout << "\t\t\t\t\tEvent details updated successfully.\n";
    }

    void ShowTotalBookedTickets(const vector<ORDER> &orders) {
        cout << "\t\t\t\t\tTotal Booked Tickets:\n";
        for (const auto &order : orders) {
            for (const auto &ticket : order.GetTickets()) {
                cout << "\t\t\t\t\tOrder ID: " << order.GetTickets().size() << ", \t\t\t\t\tCustomer Name: " << ticket.GetCustomerName() << endl;
            }
        }
    }

private:
    const string password = "admin";
    const string pin = "1234";
};

void PrintReceipt(const ORDER &order) {
    // Save the event details to ensure they are up-to-date before printing the receipt
    order.SaveToFile("order.txt");
    
    cout << "\n\t\t\t\t\tOrder Receipt:\n";
    order.ViewOrderDetails();
    cout << "\t\t\t\t\tTotal Price: $" << order.CalculateTotalPrice() << endl;
}

int main() {
    VENUE venue;
    venue.LoadFromFile("venue.txt");

    EVENT event;
    event.LoadFromFile("event.txt");

    vector<TICKET> tickets;
    tickets.push_back(TICKET(1, 50.0, "VIP", ""));
    tickets.push_back(TICKET(2, 30.0, "General", ""));
    tickets.push_back(TICKET(3, 20.0, "Student", ""));

    for (TICKET &ticket : tickets) {
        ticket.LoadFromFile("ticket.txt");
    }
cout<<"\n\n\n\n\n\n\n";
    	cout<<"\t\t\t\t\t********************************\n\n";
    	Sleep(150);
    	cout<<"\t\t\t\t\t|     W E L C O M E  T O       |\n\n";
    	cout<<"\t\t\t\t\t|                              |\n\n";
    	cout<<"\t\t\t\t\t|           O U R              |\n\n";
    	cout<<"\t\t\t\t\t|                              |\n\n";
    	cout<<"\t\t\t\t\t|         E V E N T            |\n\n";
    	cout<<"\t\t\t\t\t********************************\n";
    	Sleep(3000);
    	system("CLS");
    ORDER order(1, event);
    vector<ORDER> orders;
    orders.push_back(order);
    ADMIN admin;
    int choice;
    do {
        cout << "\n\n\n\n\n\n\n";
        cout<<"\t\t\t\t\t      ----(Main Menu)----      \n\n";
        cout<<"\t\t\t\t\t********************************\n\n";
        Sleep(150);
        cout<<"\t\t\t\t\t|     1. View Event            |\n\n";
        cout<<"\t\t\t\t\t|     2. Book Ticket           |\n\n";
        cout<<"\t\t\t\t\t|     3. Print Receipt         |\n\n";
        cout<<"\t\t\t\t\t|     4. Admin Panel           |\n\n";
        cout<<"\t\t\t\t\t|     0. Exit                  |\n\n";
        cout<<"\t\t\t\t\t********************************\n";
        cout<<"\n\n";
        cout<<"\t\t\t\t\tEnter your choice: ";
        while (!(cin >> choice) || choice < 0 || choice > 4) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
            cout << "\t\t\t\t\tInvalid choice. Please enter a valid option (0-4): ";
        }
        system("CLS");

        switch (choice) {
            case 1:
                event.DisplayEventDetails();
                break;
            case 2:
                if (order.GetTickets().empty()) {
                    order.PlaceOrder(tickets);
                    PrintReceipt(order);
                } else {
                    cout << "\t\t\t\t\tYou have already booked tickets.\n";
                }
                break;
            case 3:
                if (!order.GetTickets().empty()) {
                    PrintReceipt(order);
                } else {
                    cout << "\t\t\t\t\tNo tickets booked yet.\n";
                }
                break;
            case 4:
                if (admin.Login()) {
                    int adminChoice;
                    do {
                        cout<<"\n\n\n\n\n\n\n";
                        cout<<"\t\t\t\t\t           ----(Admin Menu)----         \n\n";
                        cout<<"\t\t\t\t\t****************************************\n\n";
                        Sleep(150);
                        cout<<"\t\t\t\t\t|     1. Change Ticket Prices          |\n\n";
                        cout<<"\t\t\t\t\t|     2. Change Event Details          |\n\n";
                        cout<<"\t\t\t\t\t|     3. Show Total Booked Tickets     |\n\n";
                        cout<<"\t\t\t\t\t|     0. Exit                          |\n\n";
                        cout<<"\t\t\t\t\t****************************************\n";
                        cout<<"\n\n";
                        cout<<"\t\t\t\t\tEnter your choice: ";
                        while (!(cin >> adminChoice) || adminChoice < 0 || adminChoice > 3) {
                            cin.clear(); // clear the error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard incorrect input
                            cout << "\t\t\t\t\tInvalid choice. Please enter a valid option (0-3): ";
                        }
                        system("CLS");

                        switch (adminChoice) {
                            case 1:
                                admin.ChangeTicketPrices(tickets);
                                break;
                            case 2:
                                admin.ChangeEventDetails(event);
                                break;
                            case 3:
                                admin.ShowTotalBookedTickets(orders);
                                break;
                            case 0:
                                cout << "\t\t\t\t\tExiting Admin Menu...\n";
                                break;
                            default:
                                cout << "\t\t\t\t\tInvalid choice. Try again.\n";
                                break;
                        }
                    } while (adminChoice != 0);
                }
                break;
            case 0:
                venue.SaveToFile("venue.txt");
                event.SaveToFile("event.txt");
                for (const TICKET& ticket : tickets) {
                    ticket.SaveToFile("ticket.txt");
                }
                for (const ORDER& ord : orders) {
                    ord.SaveToFile("order.txt");
                }
                cout << "\t\t\t\t\tExiting...\n";
                break;
            default:
                cout << "\t\t\t\t\tInvalid choice. Try again.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

