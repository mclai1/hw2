#include <sstream>
#include <iomanip>
#include "clothing.h"
#include "util.h"

using namespace std;

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, const std::string size, const std::string brand):
    Product(category, name, price, qty),
    size_(size),
    brand_(brand)
{

}

Clothing::~Clothing()
{

}

/**
 * Returns the appropriate keywords that this product should be associated with
 */
std::set<std::string> Clothing::keywords() const{
    // the keywords for clothing are the name and brand
    set<string> keywords;

    // parse the name and add it to the keywords
    set<string> productName = parseStringToWords(name_);
    keywords = setUnion(keywords, productName);

    // parse the brand name and add it to the keywords
    set<string> brandName = parseStringToWords(brand_);
    keywords = setUnion(keywords, brandName);

    return keywords;
}

/**
 * Returns a string to display the product info for hits of the search
 */
std::string Clothing::displayString() const{
    // return the contents of the product info
    return name_ + "\nSize: " + size_ + " Brand: " + brand_ + "\n" + to_string(price_) + " " + to_string(qty_) + " left.";
}

/**
 * Outputs the product info in the database format
 */
void Clothing::dump(std::ostream& os) const{
    // output the contents of the product in the file
    os << "clothing\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << endl;
}