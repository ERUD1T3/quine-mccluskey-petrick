// file to run algo for quine mccluskey method

#include <iostream>
#include <fstream>
#include <string>
#include "includes/quine-mcclusky-petrick.h"

using namespace std;

string alphabet = ""; // read from input

int main(int argc, char **argv)
{
    if (argc > 1) // input file present
    {
        string line, tmp, res, input = "", minterms = "", dontcare = ""; // line read from input file
        uint counter;
        // string inputfile = argv[1]; //
        ifstream file(argv[1]);
        if (file.is_open())
        {
            while (getline(file, line))
            {
                counter = 0;
                tmp = "";
                /* read input file */
                for (uint i = 0; i <= line.length(); ++i)
                {
                    
                    if (line[i] != ',')
                    {
                        tmp += line[i];
                    }
                    else
                    {
                        switch (counter)
                        {
                        case 0:
                            input = tmp;
                            break;
                        case 1:
                            minterms = tmp;
                            break;
                        case 2:
                            dontcare = tmp;
                            break;
                        default:
                            break;
                        }
                        tmp = "";
                        ++counter;
                    }
                }

                for (auto x : input)
                    if (x != '-')
                        alphabet += x;

                if (DEBUG)
                {
                    cout << "Testing" << endl;
                    cout << "alphabet: " << alphabet << endl;
                    cout << "Inputs: " << input << endl;
                    cout << "Minterms: " << minterms << endl;
                    cout << "Don't Care: " << dontcare << endl;
                }

                res = quine_mcclusky(input, minterms, dontcare, alphabet);

                cout << "Solution: " << res << endl;
            }

            /* finish reading input file */
            file.close();
        }
        else
            cout << "Error opening file!" << endl;
    }
    else // no input file, exec default
    {
        string input = "A-B-C-D", // inputs to expression, separated by comma
            minterms = "0-2-3-4-5-6-7-8-9-10-11-12-13",
               dontcare = "";

        for (auto x : input)
            if (x != '-')
                alphabet += x;

        if (DEBUG)
        {
            cout << "Testing" << endl;
            cout << "Inputs: " << input << endl;
            cout << "Minterms: " << minterms << endl;
            cout << "Don't Care: " << dontcare << endl;
        }

        string res = quine_mcclusky(input, minterms, dontcare, alphabet);

        cout << "Solution: " << res << endl;
    }

    return EXIT_SUCCESS;
}
