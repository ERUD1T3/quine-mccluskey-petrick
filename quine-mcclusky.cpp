// file to run algo for quine mccluskey method

#include <iostream>
#include <string>
#include "includes/qmutils.h"


using namespace std;

int main(int argc, char const *argv[])
{
    /* code */

    string inputs = "A,B,C",          // inputs to expression, separated by comma
        minterms = "0,2,3,4,5,7"; // minterms to expression, separated by comma

    if (DEBUG)
    {
        cout << "Testing" << endl;
        cout << "Inputs: " << inputs << endl;
        cout << "Minterms: " << minterms << endl;
    }

    string res = quine_mcclusky(inputs, minterms);

    cout << "Solution: " << res << endl;

    return EXIT_SUCCESS;
}
