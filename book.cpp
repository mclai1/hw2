#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"

using namespace std;

Book::Book(const std::string category, const std::string name, double price, int qty, const std::string author, const std::string isbn):
    Product(category, name, price, qty),
    author_(author),
    isbn_(isbn)
{

}

Book::~Book()
{

}

/**
 * Returns the appropriate keywords that this product should be associated with
 */
std::set<std::string> Book::keywords() const{
    // the keywords for the book are the isbn, name, and author
    set<string> keywords;
    keywords.insert(isbn_);

    // parse the name and add it to keywords
    set<string> productName = parseStringToWords(name_);
    keywords = setUnion(keywords, productName);

    // parse the author name and add it to keywords
    set<string> authorName = parseStringToWords(author_);
    keywords = setUnion(keywords, authorName);

    return keywords;
}

/**
 * Returns a string to display the product info for hits of the search
 */
std::string Book::displayString() const{
    // return the contents of the product info
    return name_ + "\nAuthor: " + author_ + " ISBN: " + isbn_ + "\n" + to_string(price_) + " " + to_string(qty_) + " left.";
}

/**
 * Outputs the product info in the database format
 */
void Book::dump(std::ostream& os) const{
    // output the contents of the product in the file
    os << "book\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << endl;
}