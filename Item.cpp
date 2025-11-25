/*
 * File: Item.cpp
 * ----------------
 * Implementations of the Item class declared in Item.h.
 *
 * Data Table (identifier | datatype | use)
 * ---------------------------------------------------------------
 * name             | std::string              | Item name (moved into object)
 * description      | std::string              | Item description (moved into object)
 * id               | int                      | ID Number
 * title            | std::string              | Title for Book/Movie (moved)
 * author           | std::string              | Book author (moved)
 * copyrightDate    | std::string              | Book copyright date (moved)
 * director         | std::string              | Movie director (moved)
 * mainActors       | std::vector<std::string> | Movie actor list (moved)
 * edition          | std::string              | Magazine edition (moved)
 * mainArticleTitle | std::string              | Magazine main article (moved)
 */

#include "Item.h"
#include <utility>

using namespace std;

Item::Item(string name_, string description_, int id_)
    : name(move(name_)), description(move(description_)), id(id_) {}

Item::~Item() = default;

int Item::getId() const { return id; }
string Item::getName() const { return name; }

void Item::print(ostream &os) const {
    os << "Item[id=" << id
       << ", name=\"" << name << "\""
       << ", desc=\"" << description << "\"]";
}

Book::Book(string name_, string description_, int id_,
           string title_, string author_, string copyrightDate_)
    : Item(move(name_), move(description_), id_),
      title(move(title_)),
      author(move(author_)),
      copyrightDate(move(copyrightDate_))
{}

void Book::print(ostream &os) const {
    os << "Book[id=" << id
       << ", name=\"" << name << "\""
       << ", title=\"" << title << "\""
       << ", author=\"" << author << "\""
       << ", copyright=\"" << copyrightDate << "\""
       << ", desc=\"" << description << "\"]";
}

Movie::Movie(string name_, string description_, int id_,
             string title_, string director_, vector<string> actors)
    : Item(move(name_), move(description_), id_),
      title(move(title_)),
      director(move(director_)),
      mainActors(move(actors))
{}

void Movie::print(ostream &os) const {
    os << "Movie[id=" << id
       << ", name=\"" << name << "\""
       << ", title=\"" << title << "\""
       << ", director=\"" << director << "\""
       << ", actors=[";
    for (size_t i = 0; i < mainActors.size(); ++i) {
        if (i) os << ", ";
        os << mainActors[i];
    }
    os << "], desc=\"" << description << "\"]";
}

Magazine::Magazine(string name_, string description_, int id_,
                   string edition_, string mainArticleTitle_)
    : Item(move(name_), move(description_), id_),
      edition(move(edition_)),
      mainArticleTitle(move(mainArticleTitle_))
{}

void Magazine::print(ostream &os) const {
    os << "Magazine[id=" << id
       << ", name=\"" << name << "\""
       << ", edition=\"" << edition << "\""
       << ", mainArticle=\"" << mainArticleTitle << "\""
       << ", desc=\"" << description << "\"]";
}

ostream &operator<<(ostream &os, const Item &item) {
    item.print(os);
    return os;
}
