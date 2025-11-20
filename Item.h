#pragma once
#include <string>
#include <vector>
#include <iostream>

/*
 * File: Item.h
 * ----------------
 * Declarations for the item hierarchy used by the library inventory.
 * Includes: Item (base), Book, Movie, Magazine. Each derived type
 * provides additional fields and overrides a virtual print method.
 *
 * Data Table (identifier | datatype                     | use)
 * ---------------------------------------------------------------
 * name               | std::string                   | Item name
 * description        | std::string                   | Item description
 * id                 | int                           | Unique item identifier
 * title              | std::string                   | Title (Book/Movie)
 * author             | std::string                   | Book author
 * copyrightDate      | std::string                   | Book copyright date
 * director           | std::string                   | Movie director
 * mainActors         | std::vector<std::string>      | Movie main actors
 * edition            | std::string                   | Magazine edition
 * mainArticleTitle   | std::string                   | Magazine main article title
 *
 */

// Base class representing a generic library item. Stores the common
// attributes shared by all items and exposes a polymorphic print.
class Item {
protected:
    std::string name;         // Item name
    std::string description;  // Item description
    int id;                   // Unique identifier for the item
public:
    Item(std::string name, std::string description, int id);
    virtual ~Item();

    int getId() const;                // Return the item's id
    std::string getName() const;      // Return the item's name

    // Polymorphic printable representation
    virtual void print(std::ostream &os) const;
};

// Book: derived from Item, adds title/author/copyrightDate
class Book : public Item {
    std::string title;        // Book title
    std::string author;       // Book author
    std::string copyrightDate;// Copyright date for the book
public:
    Book(std::string name, std::string description, int id, std::string title, std::string author, std::string copyrightDate);
    void print(std::ostream &os) const override;
};

// Movie: derived from Item, adds title, director and list of main actors
class Movie : public Item {
    std::string title;                       // Movie title
    std::string director;                    // Movie director
    std::vector<std::string> mainActors;     // List of main actors
public:
    Movie(std::string name, std::string description, int id, std::string title, std::string director, std::vector<std::string> actors);
    void print(std::ostream &os) const override;
};

// Magazine: derived from Item, adds an edition and main article title
class Magazine : public Item {
    std::string edition;          // Magazine edition identifier
    std::string mainArticleTitle; // Title of the main article
public:
    Magazine(std::string name, std::string description, int id, std::string edition, std::string mainArticleTitle);
    void print(std::ostream &os) const override;
};

// Polymorphic stream operator for any Item-derived object. Uses
// the virtual print() method to produce a textual representation.
std::ostream &operator<<(std::ostream &os, const Item &item);
