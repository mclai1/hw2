#include <string>
#include <set>
#include <vector>
#include <map>
#include "product.h"
#include "user.h"
#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore(){

}

MyDataStore::~MyDataStore() {
    for (Product* product : products_){
        delete product;
    }
    for (map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it){
        delete it->second;
    }
}

/**
 * Adds a product to the data store
 */
void MyDataStore::addProduct(Product* p){
    // add the product to the products vector
    products_.push_back(p);
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u){
    // add the username the map of users
    users_[convToLower(u->getName())] = u;
    // create a vector for the cart and add it to the carts map
    vector<Product*> cart;
    carts_[convToLower(u->getName())] = cart;
}

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term) while
 *  type 1 = OR search (union of results for each term)
 */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
    // create a map
    map<string, set<Product*>> prodmap;
    // loop through all products
    for (Product* product : products_){
        // get the keywords for the product
        set<string> keywords = product->keywords();
        // loop through all the keywords, adding it to the map
        for (set<string>::iterator it = keywords.begin(); it!=keywords.end(); ++it){
            prodmap[*it].insert(product);
        }
    }

    // set of products to return
    set<Product*> searchProducts;
    // OR search (union)
    if (type == 1){
        for (string term : terms){
            if (prodmap.find(convToLower(term)) != prodmap.end()){
                searchProducts = setUnion(searchProducts, prodmap[term]);
            }
        }
    }
    // AND search (intersection)
    else{
        for (string term : terms){
            if (prodmap.find(convToLower(term)) != prodmap.end()){
                // if there are no products in the set yet
                if (searchProducts.empty()){
                    searchProducts = prodmap[term];
                }
                // if there are already products in the set, use setIntersection
                else{
                    searchProducts = setIntersection(searchProducts, prodmap[term]);
                }
            }
        }
    }
    // convert the set to a vector and return it
    vector<Product*> productVector(searchProducts.begin(), searchProducts.end());
    return productVector;
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(std::ostream& ofile){
    // output the contents in the file
    ofile << "<products>" << endl;
    for (Product* product : products_){
        product->dump(ofile);
    }
    ofile << "</products>" << endl;
    ofile << "<users>" << endl;
    for (map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it){
        User *user = it->second;
        user->dump(ofile);
    }
    ofile << "</users>\n";
}
