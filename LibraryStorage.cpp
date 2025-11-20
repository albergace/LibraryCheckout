#include "LibraryStorage.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

/*
 * File: LibraryStorage.cpp
 * ------------------------
 * Implements the storage model declared in LibraryStorage.h. A Compartment
 * owns an Item pointer and contains optional checkout metadata (person/due).
 * LibraryStorage provides operations to add, checkout, checkin, swap, and
 * print items. All operations validate indices and report clear errors.
 */

// ------------------ Compartment implementation ------------------
Compartment::Compartment() = default;

bool Compartment::isEmpty() const { return ptr == nullptr; }

const Item* Compartment::get() const { return ptr.get(); }
Item* Compartment::get() { return ptr.get(); }

void Compartment::place(unique_ptr<Item> item) { ptr = move(item); }

std::unique_ptr<Item> Compartment::remove() {
    // When removing the item, clear checkout metadata and return ownership.
    clearCheckoutInfo();
    return move(ptr);
}

void Compartment::setCheckoutInfo(const std::string &person, const std::string &due) {
    checkedOutBy = person;
    dueDate = due;
}

void Compartment::clearCheckoutInfo() {
    checkedOutBy.clear();
    dueDate.clear();
}

bool Compartment::isCheckedOut() const { return !checkedOutBy.empty(); }

std::string Compartment::getCheckedOutBy() const { return checkedOutBy; }
std::string Compartment::getDueDate() const { return dueDate; }

// ------------------ Shelf implementation ------------------
Shelf::Shelf() = default;
size_t Shelf::capacity() const { return CAP; }
Compartment& Shelf::operator[](size_t idx) {
    if (idx >= CAP) throw out_of_range("Compartment index out of range");
    return comps[idx];
}
const Compartment& Shelf::operator[](size_t idx) const {
    if (idx >= CAP) throw out_of_range("Compartment index out of range");
    return comps[idx];
}

// ------------------ LibraryStorage implementation ------------------
LibraryStorage::LibraryStorage(size_t numShelves) : shelves(numShelves) {}
size_t LibraryStorage::numShelves() const { return shelves.size(); }

Shelf& LibraryStorage::operator[](size_t idx) {
    if (idx >= shelves.size()) throw out_of_range("Shelf index out of range");
    return shelves[idx];
}
const Shelf& LibraryStorage::operator[](size_t idx) const {
    if (idx >= shelves.size()) throw out_of_range("Shelf index out of range");
    return shelves[idx];
}

bool LibraryStorage::addItem(unique_ptr<Item> item, size_t shelfIdx, size_t compIdx) {
    if (shelfIdx >= shelves.size()) {
        cerr << "Error: Shelf " << shelfIdx << " does not exist.\n";
        return false;
    }
    try {
        Compartment &c = shelves[shelfIdx][compIdx];
        if (!c.isEmpty()) {
            cerr << "Error: Compartment " << compIdx << " on shelf " << shelfIdx << " is already occupied.\n";
            return false;
        }
        c.place(move(item));
        c.clearCheckoutInfo();
        return true;
    } catch (const out_of_range &e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

bool LibraryStorage::checkoutItem(size_t shelfIdx, size_t compIdx, string person, string dueDate) {
    if (shelfIdx >= shelves.size()) {
        cerr << "Error: Shelf " << shelfIdx << " does not exist.\n";
        return false;
    }
    try {
        Compartment &c = shelves[shelfIdx][compIdx];
        if (c.isEmpty()) {
            cerr << "Error: Cannot checkout from empty compartment (" << shelfIdx << ", " << compIdx << ").\n";
            return false;
        }
        // Move ownership of the item into our checkedOut vector
        unique_ptr<Item> it = c.remove();
        CheckedOutRecord rec{move(it), shelfIdx, compIdx, move(person), move(dueDate)};
        checkedOut.push_back(move(rec));
        return true;
    } catch (const out_of_range &e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

bool LibraryStorage::checkinItem(size_t shelfIdx, size_t compIdx) {
    auto it = find_if(checkedOut.begin(), checkedOut.end(), [&](const CheckedOutRecord &r){
        return r.origShelf == shelfIdx && r.origComp == compIdx;
    });
    if (it == checkedOut.end()) {
        cerr << "Error: No checked-out item recorded for location (" << shelfIdx << ", " << compIdx << ").\n";
        return false;
    }
    try {
        Compartment &c = shelves[shelfIdx][compIdx];
        if (!c.isEmpty()) {
            cerr << "Error: Cannot check in; compartment (" << shelfIdx << ", " << compIdx << ") is already occupied.\n";
            return false;
        }
        c.place(move(it->item));
        c.clearCheckoutInfo();
        checkedOut.erase(it);
        return true;
    } catch (const out_of_range &e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

void LibraryStorage::printItemsInStorage() const {
    cout << "Items in storage:\n";
    for (size_t s = 0; s < shelves.size(); ++s) {
        for (size_t c = 0; c < shelves[s].capacity(); ++c) {
            const Compartment &comp = shelves[s][c];
            if (!comp.isEmpty()) {
                cout << " Shelf " << s << " Compartment " << c << " -> " << *comp.get() << "\n";
            }
        }
    }
}

void LibraryStorage::printCheckedOutItems() const {
    cout << "Checked-out items:\n";
    if (checkedOut.empty()) cout << " (none)\n";
    for (const auto &rec : checkedOut) {
        cout << " From (" << rec.origShelf << ", " << rec.origComp << ") -> " << *rec.item << " | Person: " << rec.person << " | Due: " << rec.dueDate << "\n";
    }
}

bool LibraryStorage::swapItems(size_t s1, size_t c1, size_t s2, size_t c2) {
    if (s1 >= shelves.size() || s2 >= shelves.size()) {
        cerr << "Error: One of the shelves does not exist.\n";
        return false;
    }
    try {
        Compartment &a = shelves[s1][c1];
        Compartment &b = shelves[s2][c2];
        if (a.isEmpty() || b.isEmpty()) {
            cerr << "Error: Both compartments must contain an item to swap.\n";
            return false;
        }
        unique_ptr<Item> tmp = a.remove();
        a.place(b.remove());
        b.place(move(tmp));
        return true;
    } catch (const out_of_range &e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
}
