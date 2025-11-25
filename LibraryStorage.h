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
 * Declarations for the library storage. The storage is composed of
 * Shelves; each Shelf contains up to 15 Compartments. Each Compartment owns
 * a pointer to an Item.
 *
 * Data Table (identifier | datatype | use)
 * ----------------------------------------------------------------------
 * ptr              | std::unique_ptr<Item>         | Compartment's owned Item pointer
 * Compartment      | class                         | Holds item pointer
 * CAP              | constexpr size_t              | Max compartments per shelf (15)
 * comps            | std::array<Compartment, CAP>  | Storage for compartments on a shelf
 * Shelf            | class                         | Contains an array of compartments
 * shelves          | std::vector<Shelf>            | Shelves in LibraryStorage
 * CheckedOutRecord | struct                        | Internal record of checked-out items
 * checkedOut       | std::vector<CheckedOutRecord> | List of checked-out items
 * person           | std::string                   | Name of person who checked out an item
 * dueDate          | std::string                   | Due date string for a checkout
 *
 */

class Compartment {
    std::unique_ptr<Item> ptr;
public:
    Compartment();
    bool isEmpty() const;
    const Item* get() const;
    Item* get();

    // Place an item into the compartment. Existing item should be null.
    void place(std::unique_ptr<Item> item);

    // Remove the item and return ownership.
    std::unique_ptr<Item> remove();
};

class Shelf {
    static constexpr size_t CAP = 15;
    std::array<Compartment, CAP> comps;
public:
    Shelf();
    size_t capacity() const;
    Compartment& operator[](size_t idx);
    const Compartment& operator[](size_t idx) const;
};

class LibraryStorage {
    std::vector<Shelf> shelves;

    struct CheckedOutRecord {
        std::unique_ptr<Item> item;
        size_t origShelf;
        size_t origComp;
        std::string person;
        std::string dueDate;
    };

    std::vector<CheckedOutRecord> checkedOut;

public:
    LibraryStorage(size_t numShelves = 3);
    size_t numShelves() const;
    Shelf& operator[](size_t idx);
    const Shelf& operator[](size_t idx) const;

    bool addItem(std::unique_ptr<Item> item, size_t shelfIdx, size_t compIdx);
    bool checkoutItem(size_t shelfIdx, size_t compIdx, std::string person,
                      std::string dueDate);
    bool checkinItem(size_t shelfIdx, size_t compIdx);
    bool removeItem(size_t shelfIdx, size_t compIdx);
    void printItemsInStorage() const;
    void printCheckedOutItems() const;
    bool swapItems(size_t s1, size_t c1, size_t s2, size_t c2);
};
