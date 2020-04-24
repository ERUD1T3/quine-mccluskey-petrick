// implementation of qine mcclusky utilities

#include "qmutils.h"

using namespace std;

Binary::Binary()
{
    this->binsize = 0;
    this->checked = false;
    this->bins = "";
}

Binary::Binary(uint binsize, uint minterm)
{
    //constructor, returns a Binary object
    this->binsize = binsize;
    this->checked = false;
    this->in_minterms.push_back(minterm);
    this->bins = minterm2bin(minterm, binsize);
}

Binary::Binary(uint binsize)
{
    //constructor, returns a Binary object
    this->binsize = binsize;
    this->checked = false;
    // this->in_minterms.push_back(minterm);
    // this->bins = minterm2bin(minterm);
}

string Binary::tostring()
{
    string res;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (uint c = 0; c < this->binsize; ++c)
    {
        if (this->bins[c] == '-')
        {
            continue;
        }

        if (this->bins[c] == '0')
        {
            res += '~';
            res += alphabet[c];
        }

        if (this->bins[c] == '1')
        {
            res += alphabet[c];
        }
    }

    return res;
}

Binary::~Binary()
{
    // no memory to deallocate
}

string &Binary::getbins()
{
    return this->bins;
}

string minterm2bin(uint minterm, uint size)
{
    // convert minterms to binary
    uint copy = minterm, rem;
    char tmp;
    string res;
    do
    {
        rem = copy % 2;
        tmp = '0' + rem;
        res.insert(0, 1, tmp);
        copy /= 2;
    } while (copy > 0);

    while (res.size() < size)
    {
        res.insert(0, 1, '0');
    }
    return res;
}

bool compatible(Binary bin1, Binary bin2) // return true if bins are matcheable
{
    // return true if binary are compatible for matching
    uint counter = 0;

    for (uint c = 0; c < bin1.getbins().size(); ++c)
    {
        if (bin1.getbins()[c] != bin2.getbins()[c])
        {
            ++counter;
        }

        if (counter >= 2)
            break;
    }

    return (counter == 1) ? true : false;
}

Binary match(Binary bin1, Binary bin2)
{
    // return a new Binary matched
    /* Check for compatibility before calling this functions*/
    Binary res = Binary(bin1.getsize());
    res.getbins() = bin1.getbins();

    for (uint i = 0; i < bin1.getinmins().size(); ++i)
    {
        res.getinmins().push_back(bin1.getinmins()[i]);
    }

    for (uint i = 0; i < bin2.getinmins().size(); ++i)
    {
        res.getinmins().push_back(bin2.getinmins()[i]);
    }

    for (uint c = 0; c < bin1.getbins().size(); ++c)
    {
        if (bin1.getbins()[c] != bin2.getbins()[c])
        {
            res.getbins()[c] = '-';
        }
    }
    return res;
}

vector<uint> &Binary::getinmins()
{
    return this->in_minterms;
}

uint Binary::getnumones()
{
    uint counter = 0;

    for (uint c = 0; c < this->getbins().size(); ++c)
    {
        if (this->getbins()[c] == '1')
        {
            ++counter;
        }
    }

    return counter;
}

string quine_mcclusky(string inputs, string minterms)
{
    // main functions
    string res;
    vector<string> v_inputs;
    vector<uint> v_minterms;
    unordered_map<string, Binary> unchecked, primeimp;
    vector<vector<Binary>> prev_group;
    bool to_continue = true;

    /* obtaining midterms */
    if (DEBUG)
    {
        cout << "Parsing Minterms " << endl;
    }
    parse<uint>(v_minterms, minterms, [](string a) -> uint { return stoi(a); });

    /* Getting inputs */
    if (DEBUG)
    {
        cout << "Parsing inputs " << endl;
    }
    parse<string>(v_inputs, inputs, [](string a) -> string { return a; });

    /* build first group */
    if (DEBUG)
    {
        cout << "Building Init Group" << endl;
    }

    prev_group.resize(v_inputs.size() + 1);

    if (DEBUG)
    {
        cout << "Size of minterms " << v_minterms.size() << endl;
        cout << "Size of inputs " << v_inputs.size() << endl;
    }

    for (uint i = 0; i < v_minterms.size(); ++i)
    {

        // read minterms into first group
        Binary tmp = Binary(v_inputs.size(), v_minterms[i]);
        if (DEBUG)
        {
            cout << "building first binaries " << i << endl;
            cout << "bin= " << tmp.getbins() << "from " << v_minterms[i] << endl;
        }

        prev_group[tmp.getnumones()].push_back(tmp);
        if (DEBUG)
        {
            cout << "populating prev group" << i << endl;
        }
    }

    if (DEBUG)
    {
        cout << "starting crossmatching " << endl;
        // exit(0);
    }

    // uint counter = 0;
    while (to_continue)
    {
        vector<vector<Binary>> curr_group;
        to_continue = crossmatch(curr_group, prev_group, unchecked);
        prev_group = curr_group;

        if (DEBUG)
        {
            static uint counter = 0;
            cout << "iter = " << ++counter << endl;
            // if(counter > 5)
            // exit(0);
        }
    }

    if (DEBUG)
    {
        cout << "simplifying prime implicants" << endl;
        cout << "size of unchecked" << unchecked.size() << endl;
    }
    primeimp = simplify(unchecked, pow(2, v_inputs.size()));

    if (DEBUG)
    {
        cout << "size of prime implicants" << primeimp.size() << endl;
        cout << "Puting results together" << endl;
    }

    for (auto x: primeimp)
    {

        res += " + ";
        res += x.second.tostring();
        
    }

    return res;
}

unordered_map<string, Binary> simplify(unordered_map<string, Binary> unchecked, uint nummins)
{

    // TODO
    unordered_map<string, Binary> res;
    vector<vector<Binary>> bin_counter;
    bin_counter.resize(nummins);

    for (auto u : unchecked)
    {
        vector<uint> tmp = u.second.getinmins();
        for (uint i = 0; i < tmp.size(); ++i)
        {
            bin_counter[tmp[i]].push_back(u.second);
        }
    }

    for (uint i = 0; i < bin_counter.size(); ++i)
    {
        if (bin_counter[i].size() == 1)
        {
            res[bin_counter[i][0].getbins()] = bin_counter[i][0];
        }
    }

    return res;
}

void Binary::check()
{
    this->checked = true;
}

bool Binary::ischecked()
{
    return this->checked;
}

bool crossmatch(
    vector<vector<Binary>> &curr,
    vector<vector<Binary>> &prev,
    unordered_map<string, Binary> &unchecked)
{

    curr.resize(prev.size() - 1);

    bool atleastonepairmatched = false;

    for (uint i = 0; i < curr.size(); ++i)
    {
        for (uint r1 = 0; r1 < prev[i].size(); ++r1)
        {
            for (uint r2 = 0; r2 < prev[i + 1].size(); ++r2)
            {
                if (DEBUG)
                {
                    cout << "bin1 " << prev[i][r1].getbins() << endl;
                    cout << "bin2 " << prev[i + 1][r2].getbins() << endl;
                }
                if (compatible(prev[i][r1], prev[i + 1][r2]))
                {

                    atleastonepairmatched = true;
                    Binary tmp = match(prev[i][r1], prev[i + 1][r2]);

                    if (DEBUG)
                    {

                        cout << "results= " << tmp.getbins() << endl;
                    }

                    curr[i].push_back(tmp);
                    prev[i][r1].check();
                    prev[i + 1][r2].check();
                }
            }
        }
    }

    if (DEBUG)
    {
        cout << "-----------------------------------------------" << endl;
    }

    for (uint i = 0; i < prev.size(); ++i)
    {
        for (uint j = 0; j < prev[i].size(); ++j)
        {
            if (!prev[i][j].ischecked())
            {
                unchecked[prev[i][j].getbins()] = prev[i][j];
            }
        }
    }

    //TODO populate the unchecked vector

    return atleastonepairmatched;
}

template <typename T>
void parse(vector<T> &dest, string input, T (*fn)(string))
{
    // parse input into the destination vector
    string tmp = "";
    for (uint i = 0; i <= input.size(); ++i)
    {
        if (i == input.size())
        {
            if (DEBUG)
            {
                cout << "tmp: " << tmp << endl;
            }
            dest.push_back((*fn)(tmp));
            tmp = "";
            break;
        }

        if (input[i] != DELIM)
        {
            tmp += input[i];
        }
        else
        { // delim found
            if (DEBUG)
            {
                cout << "tmp: " << tmp << endl;
            }
            dest.push_back((*fn)(tmp));
            tmp = "";
        }
    }
}

uint Binary::getsize()
{
    return this->binsize;
}