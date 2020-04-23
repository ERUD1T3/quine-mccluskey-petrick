// file to run algo for quine mccluskey method

#include <iostream>
#include <string>
#include "includes/qmutils.h"


using namespace std;

int main(int argc, char const *argv[])
{
    /* code */

    string inputs = "A,B,C,D",          // inputs to expression, separated by comma
        minterms = "0,1,3,7,8,9,11,15"; // minterms to expression, separated by comma

    if (DEBUG)
    {
        cout << "Testing" << endl;
        cout << "Inputs: " << inputs << "Minterms: " << minterms << endl;
    }

    string res = quine_mcclusky(inputs, minterms);

    cout << "Solution: " << res << endl;

    return EXIT_SUCCESS;
}
