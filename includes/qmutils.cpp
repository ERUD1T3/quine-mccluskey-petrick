// implementation of qine mcclusky utilities

#include "qmutils.h"

using namespace std;

Binary::Binary(uint binsize, uint minterm)
{
    //constructor, returns a Binary object
    this->binsize = binsize;
    this->checked = false;
    this->in_minterms.push_back(minterm);
    this->bins = minterm2bin(minterm);
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

string minterm2bin(uint minterm)
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
        if (this->getbins[c] == '1')
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
    vector<Binary> unchecked, primeimp;
    vector<vector<Binary>> curr_group, prev_group;
    bool to_continue = true;

    /* obtaining midterms */
    parse<uint>(v_minterms, inputs, [](string a) -> uint { return stoi(a); });

    /* Getting inputs */
    parse<string>(v_inputs, inputs, [](string a) -> string { return a; });

    /* build first group */
    prev_group.resize(v_inputs.size());
    for (uint i = 0; i < v_minterms.size(); ++i)
    {
        // read minterms into first group
        Binary tmp = Binary(v_inputs.size(), v_minterms[i]);
        prev_group[tmp.getnumones()].push_back(tmp);
    }

    while (to_continue)
    {
        to_continue = crossmatch(curr_group, prev_group, unchecked);
    }

    primeimp = simplify(unchecked, pow(2, v_inputs.size()));

    for (uint i = 0; i < primeimp.size(); ++i)
    {

        res += primeimp[i].tostring();
        res += " + ";
    }

    return res;
}

vector<Binary> simplify(vector<Binary> unchecked, uint nummins)
{
    // TODO
    vector<Binary> res;
    vector<vector<Binary>> bin_counter;
    bin_counter.resize(nummins);

    for (uint u = 0; u < unchecked.size(); ++u)
    {
        vector<uint> tmp = unchecked[u].getinmins();
        for (uint i = 0; i < tmp.size(); ++i)
        {
            bin_counter[tmp[i]].push_back(unchecked[u]);
        }
    }

    for (uint i = 0; i < bin_counter.size(); ++i)
    {
        if (bin_counter[i].size() == 1)
        {
            res.push_back(bin_counter[i][0]);
        }
    }

    return res;
}

void Binary::check()
{
    this->checked = true;
}

bool Binary::ischecked() {
    return this->checked;
}

bool crossmatch(
    vector<vector<Binary>> &curr,
    vector<vector<Binary>> &prev,
    vector<Binary> &unchecked)
{

    curr.resize(prev.size() - 1);

    bool atleastonepairmatched = false;

    for (uint i = 0; i < curr.size(); ++i)
    {
        for (uint r1 = 0; r1 < prev[i].size(); ++r1)
        {
            for (uint r2 = 0; r2 < prev[i + 1].size(); ++r2)
            {
                if (compatible(prev[i][r1], prev[i + 1][r2]))
                {
                    atleastonepairmatched = true;
                    curr[i].push_back(match(prev[i][r1], prev[i + 1][r2]));
                    prev[i][r1].check();
                    prev[i + 1][r2].check();
                }
            }
        }
    }

    for (uint i = 0; i < prev.size(); ++i)
    {
        for (uint j = 0; j < prev[i].size(); ++j)
        {
            if(!prev[i][j].ischecked()) 
            {
                unchecked.push_back(prev[i][j]);
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
            dest.push_back((*fn)(tmp));
            tmp = "";
        }
    }
}

uint Binary::getsize()
{
    return this->binsize;
}