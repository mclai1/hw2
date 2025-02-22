#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <queue>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "mydatastore.h"
#include "util.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            else if ( cmd == "ADD") {
                string username;
                int hit_result_index;
                // check if the username and indexes were inputted correctly
                if (ss >> username >> hit_result_index && ds.carts_.find(username) != ds.carts_.end() && hit_result_index > 0 && hit_result_index <= int(hits.size())){
                    // add the corresponding hit to the cart
                    ds.carts_.find(convToLower(username))->second.push_back(hits[hit_result_index - 1]);
                }
                // print if there is an invalid request
                else{
                    cout << "Invalid request" << endl;
                }
            }
            else if ( cmd == "VIEWCART"){
                string username;
                // check if the username is inputted correctly and is a valid username
                if (ss >> username && ds.carts_.find(convToLower(username)) != ds.carts_.end()){
                    // create a copy of the cart to iterate through
                    vector<Product*> user_cart = ds.carts_.find(convToLower(username))->second;
                    // print the cart contents until you reach the end
                    int i = 1;
                    while (!user_cart.empty()){
                        cout << "Hit " << i << endl;
                        cout << user_cart.front()->displayString() << '\n' << endl;
                        user_cart.erase(user_cart.begin());
                        i++;
                    }
                }
                // print if there is an invalid request
                else{
                    cout << "Invalid username" << endl;
                }
            }
            else if ( cmd == "BUYCART"){
                string username;
                // check if the username is inputted correctly and is a valid username
                if (ss >> username && ds.carts_.find(convToLower(username)) != ds.carts_.end()){
                    username = convToLower(username);
                    // iterate through the products
                    vector<Product*>& user_cart = ds.carts_.find(username)->second;
                    int i = 0;
                    while (i < int (user_cart.size())){
                        Product* current = user_cart[i];
                        // if there is stock and the user has enough money, purchase the item, decrease the quantity, deduct the amount, and remove it from the cart
                        if (current->getQty() > 0 && current->getPrice() <= ds.users_.find(username)->second->getBalance()){
                            current->subtractQty(1);
                            ds.users_.find(username)->second->deductAmount(current->getPrice());
                            user_cart.erase(user_cart.begin() + i);
                        }
                        // if there is no stock, go to the next item in the cart
                        else{
                            i++;
                        }
                    }
                }
                // print if there is an invalid username
                else{
                    cout << "Invalid username" << endl;
                }
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
