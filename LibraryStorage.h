#pragma once
#include "Item.h"
#include <array>
#include <vector>
#include <memory>
#include <string>
#include <iostream>

/*
 * File: LibraryStorage.h
 * ----------------------
 * Declarations for the library storage model. The storage is composed of
 * Shelves; each Shelf contains up to 15 Compartments. Each Compartment owns
 * a pointer to an Item and also stores checkout metadata (if checked out).
 *
 * Data Table (identifier | datatype                     | use)
 * ----------------------------------------------------------------------
 * ptr                | std::unique_ptr<Item>         | Compartment's owned Item pointer
 * Compartment        | class                         | Holds item pointer + checkout record
 * CAP                | constexpr size_t              | Max compartments per shelf (15)
 * comps              | std::array<Compartment, CAP>  | Storage for compartments on a shelf
 * Shelf              | class                         | Contains an array of compartments
 * shelves            | std::vector<Shelf>            | Collection of shelves in LibraryStorage
 * CheckedOutRecord   | struct                        | (legacy) not used externally; internal record type
 * checkedOut         | std::vector<CheckedOutRecord> | List of checked-out items (kept for compatibility)
 * person             | std::string                   | Name of person who checked out an item
 * dueDate            | std::string                   | Due date string for a checkout
 *
 */

// A Compartment owns a single Item (unique_ptr) and stores optional
// checkout metadata: who checked the item out and when it is due.
class Compartment {
    std::unique_ptr<Item> ptr;     // Owned item, or null if empty
    std::string checkedOutBy;      // Person who checked out the item (empty if in storage)
    std::string dueDate;           // Due date string (empty if in storage)
public:
    Compartment();
    bool isEmpty() const;
    const Item* get() const;    // Get pointer to stored item (const)
    Item* get();                // Non-const accessor

    // Place an item into the compartment. Existing item should be null.
    void place(std::unique_ptr<Item> item);

    // Remove the item and return ownership; clears checkout metadata.
    std::unique_ptr<Item> remove();

    // Checkout metadata accessors/mutators
    void setCheckoutInfo(const std::string &person, const std::string &due);
    void clearCheckoutInfo();
    bool isCheckedOut() const;
    std::string getCheckedOutBy() const;
    std::string getDueDate() const;
};

// Shelf: fixed-capacity container of compartments, supports operator[]
class Shelf {
    static constexpr size_t CAP = 15;                // Maximum compartments per shelf
    std::array<Compartment, CAP> comps;              // The compartments themselves
public:
    Shelf();
    size_t capacity() const;
    Compartment& operator[](size_t idx);
    const Compartment& operator[](size_t idx) const;
};

// LibraryStorage manages multiple shelves and provides operations to add,
// checkout, checkin, swap, and print items. The checked-out state is
// recorded inside each Compartment; we additionally maintain a small
// checkedOut vector for convenience/compatibility with earlier behavior.
class LibraryStorage {
    std::vector<Shelf> shelves;                       // The shelves in the library

    struct CheckedOutRecord {                         // Optional helper record
        std::unique_ptr<Item> item;
        size_t origShelf;
        size_t origComp;
        std::string person;
        std::string dueDate;
    };

    std::vector<CheckedOutRecord> checkedOut;         // Convenience list of checked-out items

public:
    LibraryStorage(size_t numShelves = 3);
    size_t numShelves() const;
    Shelf& operator[](size_t idx);
    const Shelf& operator[](size_t idx) const;

    bool addItem(std::unique_ptr<Item> item, size_t shelfIdx, size_t compIdx);
    bool checkoutItem(size_t shelfIdx, size_t compIdx, std::string person, std::string dueDate);
    bool checkinItem(size_t shelfIdx, size_t compIdx);
    void printItemsInStorage() const;
    void printCheckedOutItems() const;
    bool swapItems(size_t s1, size_t c1, size_t s2, size_t c2);
};
