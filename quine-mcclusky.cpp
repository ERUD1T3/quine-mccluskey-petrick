// file to run algo for quine mccluskey method

#include <iostream>
#include <string>
#include "includes/qmutils.h"

using namespace std;

int main(int argc, char **argv)
{
    /* code */

    // string inputfile = argv[1];
    // string outputfile = argv[2];

    string inputs = "A,B,C,D", // inputs to expression, separated by comma
        // minterms = "0,1,3,7,8,9,11,15",
        // minterms = "0,2,5,6,7,8,10,12,13,14,15",// minterms to expression, separated by comma
        // minterms = "0,2,3,4,5,6,7,8,9,10,11,12,13",
        minterms = "2,3,7,9,11,13",
        dontcare = "1,10,15";
    // dontcare = "";

    if (DEBUG)
    {
        cout << "Testing" << endl;
        cout << "Inputs: " << inputs << endl;
        cout << "Minterms: " << minterms << endl;
        cout << "Don't Care: " << dontcare << endl;
    }

    string res = quine_mcclusky(inputs, minterms, dontcare);

    cout << "Solution: " << res << endl;

    return EXIT_SUCCESS;
}
