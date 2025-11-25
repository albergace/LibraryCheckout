#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "LibraryStorage.h"
#include "Item.h"

using namespace std;

// ===== Helper input functions =====
string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

vector<string> splitActors(const string &line) {
    vector<string> result;
    string current;

    for (char ch : line) {
        if (ch == ',') {
            string t = trim(current);
            if (!t.empty()) {
                result.push_back(t);
            }
            current.clear();
        } else {
            current.push_back(ch);
        }
    }

    string t = trim(current);
    if (!t.empty()) {
        result.push_back(t);
    }

    return result;
}

int readInt(const string &prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            cout << "Input error. Exiting.\n";
            return minValue;
        }
        try {
            int value = stoi(line);
            if (value < minValue || value > maxValue) {
                cout << "Please enter a value between "
                     << minValue << " and " << maxValue << ".\n";
                continue;
            }
            return value;
        } catch (...) {
            cout << "Invalid integer, try again.\n";
        }
    }
}

string readLine(const string &prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

size_t getMaxCompartment(const LibraryStorage &lib) {
    return lib[0].capacity();
}

// ===== Menu actions =====

void addItemMenu(LibraryStorage &lib) {
    cout << "\n=== Add Item ===\n";

    int maxShelfIndex = static_cast<int>(lib.numShelves()) - 1;
    int shelf = readInt("Shelf index (0-" + to_string(maxShelfIndex) + "): ",
                        0, maxShelfIndex);

    size_t maxComp = getMaxCompartment(lib);
    int maxCompIndex = static_cast<int>(maxComp) - 1;
    int compartment = readInt("Compartment index (0-" + to_string(maxCompIndex) + "): ",
                              0, maxCompIndex);

    cout << "Item type:\n";
    cout << "  1. Book\n";
    cout << "  2. Movie\n";
    cout << "  3. Magazine\n";
    int type = readInt("Choose type (1-3): ", 1, 3);

    int id = readInt("Item id (integer): ", 0, 1000000);
    string name = readLine("Name: ");
    string description = readLine("Description: ");

    bool ok = false;

    if (type == 1) {
        // Book
        string title = readLine("Book title: ");
        string author = readLine("Author: ");
        string copyright = readLine("Copyright date (e.g. 2013): ");
        auto book = make_unique<Book>(name, description, id, title, author, copyright);
        ok = lib.addItem(move(book),
                         static_cast<size_t>(shelf),
                         static_cast<size_t>(compartment));
    } else if (type == 2) {
        // Movie
        string title = readLine("Movie title: ");
        string director = readLine("Director: ");
        string actorsLine = readLine("Main actors (comma-separated,"
                                     " e.g. \"Actor A, Actor B\"): ");

        vector<string> actors = splitActors(actorsLine);
        if (actors.empty()) {
            cout << "No actors entered; storing a single empty entry.\n";
            actors.push_back("");
        }

        auto movie = make_unique<Movie>(name, description, id, title, director, actors);
        ok = lib.addItem(move(movie),
                         static_cast<size_t>(shelf),
                         static_cast<size_t>(compartment));
    } else {
        // Magazine
        string edition = readLine("Edition (e.g. \"Vol 10\"): ");
        string mainArticle = readLine("Main article title: ");
        auto mag = make_unique<Magazine>(name, description, id, edition, mainArticle);
        ok = lib.addItem(move(mag),
                         static_cast<size_t>(shelf),
                         static_cast<size_t>(compartment));
    }

    if (ok) {
        cout << "Item added successfully.\n";
    } else {
        cout << "Failed to add item (see error above).\n";
    }
}

void removeItemMenu(LibraryStorage &lib) {
    cout << "\n=== Remove Item ===\n";

    int maxShelfIndex = static_cast<int>(lib.numShelves()) - 1;
    int shelf = readInt("Shelf index (0-" + to_string(maxShelfIndex) + "): ",
                        0, maxShelfIndex);

    size_t maxComp = getMaxCompartment(lib);
    int maxCompIndex = static_cast<int>(maxComp) - 1;
    int compartment = readInt("Compartment index (0-" + to_string(maxCompIndex) + "): ",
                              0, maxCompIndex);

    if (lib.removeItem(static_cast<size_t>(shelf),
                       static_cast<size_t>(compartment))) {
        cout << "Item removed successfully.\n";
    } else {
        cout << "Failed to remove item.\n";
    }
}

void checkoutMenu(LibraryStorage &lib) {
    cout << "\n=== Checkout Item ===\n";

    int maxShelfIndex = static_cast<int>(lib.numShelves()) - 1;
    int shelf = readInt("Shelf index (0-" + to_string(maxShelfIndex) + "): ",
                        0, maxShelfIndex);

    size_t maxComp = getMaxCompartment(lib);
    int maxCompIndex = static_cast<int>(maxComp) - 1;
    int compartment = readInt("Compartment index (0-" + to_string(maxCompIndex) + "): ",
                              0, maxCompIndex);

    string person = readLine("Person name: ");
    string due = readLine("Due date (YYYY-MM-DD): ");

    if (lib.checkoutItem(static_cast<size_t>(shelf),
                         static_cast<size_t>(compartment),
                         person, due)) {
        cout << "Checkout succeeded.\n";
    } else {
        cout << "Checkout failed.\n";
    }
}

void checkinMenu(LibraryStorage &lib) {
    cout << "\n=== Checkin Item ===\n";

    int maxShelfIndex = static_cast<int>(lib.numShelves()) - 1;
    int shelf = readInt("Shelf index (0-" + to_string(maxShelfIndex) + "): ",
                        0, maxShelfIndex);

    size_t maxComp = getMaxCompartment(lib);
    int maxCompIndex = static_cast<int>(maxComp) - 1;
    int compartment = readInt("Compartment index (0-" + to_string(maxCompIndex) + "): ",
                              0, maxCompIndex);

    if (lib.checkinItem(static_cast<size_t>(shelf),
                        static_cast<size_t>(compartment))) {
        cout << "Checkin succeeded.\n";
    } else {
        cout << "Checkin failed.\n";
    }
}

void swapMenu(LibraryStorage &lib) {
    cout << "\n=== Swap Items ===\n";

    int maxShelfIndex = static_cast<int>(lib.numShelves()) - 1;
    size_t maxComp = getMaxCompartment(lib);
    int maxCompIndex = static_cast<int>(maxComp) - 1;

    cout << "First location:\n";
    int s1 = readInt("  Shelf index (0-" + to_string(maxShelfIndex) + "): ",
                     0, maxShelfIndex);
    int c1 = readInt("  Compartment index (0-" + to_string(maxCompIndex) + "): ",
                     0, maxCompIndex);

    cout << "Second location:\n";
    int s2 = readInt("  Shelf index (0-" + to_string(maxShelfIndex) + "): ",
                     0, maxShelfIndex);
    int c2 = readInt("  Compartment index (0-" + to_string(maxCompIndex) + "): ",
                     0, maxCompIndex);

    if (lib.swapItems(static_cast<size_t>(s1), static_cast<size_t>(c1),
                      static_cast<size_t>(s2), static_cast<size_t>(c2))) {
        cout << "Swap succeeded.\n";
    } else {
        cout << "Swap failed.\n";
    }
}

void showStorage(const LibraryStorage &lib) {
    cout << "\n=== Items in Storage ===\n";
    lib.printItemsInStorage();
}

void showCheckedOut(const LibraryStorage &lib) {
    cout << "\n=== Checked-out Items ===\n";
    lib.printCheckedOutItems();
}

// ===== original scripted demo moved into a function =====

void runDemo() {
    cout << "\n=== Running scripted demo ===\n\n";
    cout << "Library Inventory System demo\n\n";

    LibraryStorage lib(3);

    // Add some items
    cout << "Adding items...\n";
    lib.addItem(make_unique<Book>("The C++ Guide", "Comprehensive guide to C++", 1,
                                  "C++ Guide", "Bjarne Stroustrup", "2013"),
                2, 4);
    lib.addItem(make_unique<Movie>("A Great Movie", "An epic tale", 2,
                                   "A Great Movie", "Director X",
                                   vector<string>{"Actor A","Actor B"}),
                0, 0);
    lib.addItem(make_unique<Magazine>("Tech Monthly", "Latest in tech", 3,
                                      "Vol 10", "The Future of AI"),
                1, 14);

    try {
        cout << "Accessing via operator[]: libraryInventory[2][4] -> ";
        const Compartment &comp = lib[2][4];
        if (!comp.isEmpty()) cout << *comp.get() << "\n";
        else cout << "(empty)\n";
    } catch (const out_of_range &e) {
        cout << "Error accessing compartment: " << e.what() << "\n";
    }

    cout << "\nCurrent storage state:\n";
    lib.printItemsInStorage();
    cout << "\n";

    cout << "Checking out item at (2,4) by Alice, due 2025-12-01...\n";
    lib.checkoutItem(2, 4, "Alice", "2025-12-01");

    cout << "Attempting to checkout empty slot (0,1)...\n";
    lib.checkoutItem(0, 1, "Bob", "2025-11-30");

    cout << "\nAfter checkout:\n";
    lib.printItemsInStorage();
    cout << "\n";
    lib.printCheckedOutItems();
    cout << "\n";

    cout << "Swapping items at (0,0) and (1,14)...\n";
    if (lib.swapItems(0,0,1,14)) cout << "Swap succeeded.\n";
    else cout << "Swap failed.\n";

    cout << "Attempting swap where one is empty: (0,0) and (0,1)...\n";
    if (lib.swapItems(0,0,0,1)) cout << "Swap succeeded.\n";
    else cout << "Swap failed as expected.\n";

    cout << "\nState after swaps:\n";
    lib.printItemsInStorage();
    cout << "\nChecked out items:\n";
    lib.printCheckedOutItems();
    cout << "\n";

    cout << "Checking in item back to (2,4)...\n";
    if (lib.checkinItem(2,4)) cout << "Checkin succeeded.\n";
    else cout << "Checkin failed.\n";

    cout << "\nFinal storage state:\n";
    lib.printItemsInStorage();
    cout << "\nFinal checked-out list:\n";
    lib.printCheckedOutItems();

    cout << "\nDemonstrating error: attempt to checkin at (0,1) which isn't checked"
            " out...\n";
    if (!lib.checkinItem(0,1)) cout << "Expected failure when checking in non-checked-out"
                                       " item.\n";

    cout << "\nDemo complete.\n\n";
}

// ===== Main menu =====

int main() {
    LibraryStorage lib(3);

    bool running = true;
    while (running) {
        cout << "=============================\n";
        cout << " Library Inventory System\n";
        cout << "=============================\n";
        cout << "1. Add item\n";
        cout << "2. Remove item\n";
        cout << "3. Checkout item\n";
        cout << "4. Checkin item\n";
        cout << "5. Swap items\n";
        cout << "6. Show items in storage\n";
        cout << "7. Show checked-out items\n";
        cout << "8. Run scripted demo\n";
        cout << "0. Quit\n";

        int choice = readInt("Select an option: ", 0, 8);
        cout << "\n";

        switch (choice) {
            case 1:
                addItemMenu(lib);
                break;
            case 2:
                removeItemMenu(lib);
                break;
            case 3:
                checkoutMenu(lib);
                break;
            case 4:
                checkinMenu(lib);
                break;
            case 5:
                swapMenu(lib);
                break;
            case 6:
                showStorage(lib);
                break;
            case 7:
                showCheckedOut(lib);
                break;
            case 8:
                runDemo();
                break;
            case 0:
                running = false;
                break;
        }

        cout << "\n";
    }

    cout << "Goodbye!\n";
    return 0;
}
