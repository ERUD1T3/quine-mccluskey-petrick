// quine mcclusky utility functions
#pragma once
#include <vector>
#include <string>
using namespace std;

typedef unsigned int uint;
#define DELIM ','
#define DEBUG true

class Binary
{
private:
    /* data */
    uint binsize;             // number of inputs
    string bins;              //binary array representing the minterms
    vector<uint> in_minterms; // vector of minterm involved in the binary expressions
    bool checked;             // defaulted to false

public:
    Binary(uint binsize, uint minterm); //constructor, returns a Binary object
    Binary(uint binsize);
    string tostring(); // convert binaries to string input equivalent
    string &getbins();
    vector<uint> &getinmins();
    uint getnumones(); // return the number of ones in minterm
    uint getsize();
    ~Binary(); // destructor, free memory allocated for Binary object
};

string minterm2bin(uint minterm);
bool compatible(Binary bin1, Binary bin2); // return true if bins are matcheable
Binary match(Binary bin1, Binary bin2);
string quine_mcclusky(string inputs, string midterms); // takes a number of inputs and midterms, returns reduced expression
vector<Binary> simplify(vector<Binary> unchecked);
bool crossmatch(
    vector<vector<Binary>> &curr, 
    vector<vector<Binary>> &prev, 
    vector<Binary> &unchecked
    );



template <typename T>
void parse(vector<T> &dest, string input, T (*fn)(string));

/* basic data structure for the method */
