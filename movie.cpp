#include <sstream>
#include <iomanip>
#include "movie.h"
#include "util.h"

using namespace std;

Movie::Movie(const std::string category, const std::string name, double price, int qty, const std::string genre, const std::string rating):
    Product(category, name, price, qty),
    genre_(genre),
    rating_(rating)
{

}

Movie::~Movie()
{

}

/**
 * Returns the appropriate keywords that this product should be associated with
 */
std::set<std::string> Movie::keywords() const{
    // the keywords for the movie are the name and genre
    set<string> keywords;

    // parse the name and add it to the keywords
    set<string> productName = parseStringToWords(name_);
    keywords = setUnion(keywords, productName);

    // parse the genre and add it to the keywords
    set<string> genreName = parseStringToWords(genre_);
    keywords = setUnion(keywords, genreName);

    return keywords;
}

/**
 * Returns a string to display the product info for hits of the search
 */
std::string Movie::displayString() const{
    // return a string with the product info
    return name_ + "\nGenre: " + genre_ + " Rating: " + rating_ + "\n" + to_string(price_) + " " + to_string(qty_) + " left.";
}

/**
 * Outputs the product info in the database format
 */
void Movie::dump(std::ostream& os) const{
    // output the product info in the file
    os << "movie\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << endl;
}