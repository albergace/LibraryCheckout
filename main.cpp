#include <iostream>
#include "LibraryStorage.h"
#include "Item.h"

using namespace std;

int main() {
    cout << "Library Inventory System demo\n\n";

    LibraryStorage lib(3); // 3 shelves, each with 15 compartments

    // Add some items
    cout << "Adding items...\n";
    lib.addItem(make_unique<Book>("The C++ Guide", "Comprehensive guide to C++", 1, "C++ Guide", "Bjarne Stroustrup", "2013"), 2, 4);
    lib.addItem(make_unique<Movie>("A Great Movie", "An epic tale", 2, "A Great Movie", "Director X", vector<string>{"Actor A","Actor B"}), 0, 0);
    lib.addItem(make_unique<Magazine>("Tech Monthly", "Latest in tech", 3, "Vol 10", "The Future of AI"), 1, 14);

    // Demonstrate [] access: print the item at shelf 2 compartment 4
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

    // Checkout item
    cout << "Checking out item at (2,4) by Alice, due 2025-12-01...\n";
    lib.checkoutItem(2, 4, "Alice", "2025-12-01");

    // Attempt checkout from empty slot
    cout << "Attempting to checkout empty slot (0,1)...\n";
    lib.checkoutItem(0, 1, "Bob", "2025-11-30");

    cout << "\nAfter checkout:\n";
    lib.printItemsInStorage();
    cout << "\n";
    lib.printCheckedOutItems();
    cout << "\n";

    // Swap two occupied compartments (0,0) and (1,14)
    cout << "Swapping items at (0,0) and (1,14)...\n";
    if (lib.swapItems(0,0,1,14)) cout << "Swap succeeded.\n";
    else cout << "Swap failed.\n";

    // Attempt swap where one compartment is empty
    cout << "Attempting swap where one is empty: (0,0) and (0,1)...\n";
    if (lib.swapItems(0,0,0,1)) cout << "Swap succeeded.\n";
    else cout << "Swap failed as expected.\n";

    cout << "\nState after swaps:\n";
    lib.printItemsInStorage();
    cout << "\nChecked out items:\n";
    lib.printCheckedOutItems();
    cout << "\n";

    // Checkin the item back to (2,4)
    cout << "Checking in item back to (2,4)...\n";
    if (lib.checkinItem(2,4)) cout << "Checkin succeeded.\n";
    else cout << "Checkin failed.\n";

    cout << "\nFinal storage state:\n";
    lib.printItemsInStorage();
    cout << "\nFinal checked-out list:\n";
    lib.printCheckedOutItems();

    // Demonstrate error: try to checkin at a location that isn't checked out
    cout << "\nDemonstrating error: attempt to checkin at (0,1) which isn't checked out...\n";
    if (!lib.checkinItem(0,1)) cout << "Expected failure when checking in non-checked-out item.\n";

    cout << "\nDemo complete.\n";

    return 0;
}
