#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>


/** Complete the setIntersection and setUnion functions below
 *  in this header file (since they are templates).
 *  Both functions should run in time O(n*log(n)) and not O(n^2)
 */
template <typename T>
std::set<T> setIntersection(std::set<T>& s1, std::set<T>& s2)
{
    // create iterators for the two sets and a set for the intersection
    typename std::set<T>::iterator it1 = s1.begin();
    typename std::set<T>::iterator it2 = s2.begin();
    std::set<T> intersect;

    // loop through the two sets
    while (it1 != s1.end() && it2 != s2.end()){
        // if the element is found in both, then add it to the intersection set
        if (*it1 == *it2){
            intersect.insert(*it1);
            it1++;
            it2++;
        }
        // skip over elements are not in both sets
        else if (*it1 < *it2){
            it1++;
        }
        else{
            it2++;
        }
    }
    return intersect;
}

template <typename T>
std::set<T> setUnion(std::set<T>& s1, std::set<T>& s2)
{
    // create iterators for the two sets and a set for the union
    typename std::set<T>::iterator it1 = s1.begin();
    typename std::set<T>::iterator it2 = s2.begin();
    std::set<T> unionSet;
    
    // loop through the two sets
    while(it1 != s1.end() && it2 != s2.end()){
        // add the elements to the union, ensuring that it is not added twice
        if (*it1 == *it2){
            unionSet.insert(*it1);
            it1++;
            it2++;
        }
        else if (*it1 < *it2){
            unionSet.insert(*it1);
            it1++;
        }
        else{
            unionSet.insert(*it2);
            it2++;
        }
    }
    // add any leftovers not already added
    while (it1 != s1.end()){
        unionSet.insert(*it1);
        it1++;
    }
    while (it2 != s2.end()){
        unionSet.insert(*it2);
        it2++;
    }
    return unionSet;
}

/***********************************************/
/* Prototypes of functions defined in util.cpp */
/***********************************************/

std::string convToLower(std::string src);

std::set<std::string> parseStringToWords(std::string line);

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// Removes any leading whitespace
std::string &ltrim(std::string &s) ;

// Removes any trailing whitespace
std::string &rtrim(std::string &s) ;

// Removes leading and trailing whitespace
std::string &trim(std::string &s) ;
#endif
