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
    // primeimp = petrick(unchecked, pow(2, v_inputs.size()));

    if (DEBUG)
    {
        cout << "size of prime implicants" << primeimp.size() << endl;
        cout << "Puting results together" << endl;
    }

    if (primeimp.size() > 0)
    {
        bool start = true;
        for (auto x : primeimp)
        {
            if (!start)
            {
                res += " + ";
            }
            start = false;
            res += x.second.tostring();
        }
    }
    else
    {
        bool start = true;
        // unchecked = petrick(unchecked, pow(2, v_inputs.size()));
        for (auto x : unchecked)
        {
            if (!start)
            {
                res += " + ";
            }
            start = false;
            res += x.second.tostring();
        }
    }

    return res;
}

unordered_map<string, Binary> simplify(unordered_map<string, Binary> unchecked, uint nummins)
{

    // TODO
    unordered_map<string, Binary> res;
    vector<vector<Binary>> primeimp_table;
    primeimp_table.resize(nummins);

    for (auto u : unchecked)
    {
        vector<uint> tmp = u.second.getinmins();
        for (uint i = 0; i < tmp.size(); ++i)
        {
            primeimp_table[tmp[i]].push_back(u.second);
        }
    }

    for (uint i = 0; i < primeimp_table.size(); ++i)
    {
        if (primeimp_table[i].size() == 1)
        {
            res[primeimp_table[i][0].getbins()] = primeimp_table[i][0];
        }
    }


    return res;
}

PBinary::PBinary(uint binsize, uint pos, Binary bin)
{
    this->binsize = binsize;
    string tmp = "";
    for (uint i = 0; i < binsize; ++i)
    {
        if (i == pos)
        {
            tmp += "1";
        }
        else
        {
            tmp += "-";
        }
    }
    if (DEBUG)
    {
        cout << "Pbin= " << tmp << endl;
    }

    this->pbins[tmp][bin.getbins()] = bin;
}

unordered_map<string, Binary> petrick(unordered_map<string, Binary> unchecked, uint nummins)
{

    // TODO
    unordered_map<string, Binary> res;
    vector<vector<PBinary>> primeimp_table;
    primeimp_table.resize(nummins);
    uint c = unchecked.size() - 1;
    // vector<PBinary> impls;

    /* Building prime implicants table */
    for (auto u : unchecked)
    {
        vector<uint> tmp = u.second.getinmins();
        for (uint i = 0; i < tmp.size(); ++i)
        {
            // inserting into the position of the minterm
            primeimp_table[tmp[i]].push_back(PBinary(unchecked.size(), c, u.second));
        }
    }

    PBinary tmpres;
    bool isfirst = true;
    /* petrik method */

    for (uint i = 0; i < primeimp_table.size(); ++i)
    {
        if (primeimp_table[i].size() > 0)
        {
            
            PBinary tmpres2;
            for (uint j = 0; j < primeimp_table[i].size(); ++j)
            {
                tmpres2 += primeimp_table[i][j];
            }

            if(isfirst)
            {
                tmpres = tmpres2;
                isfirst = false;
            }
            else
            {
                tmpres *= tmpres2;
            }
        }
    }

    // TODO: transition tempres to res

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

PBinary::PBinary()
{
    // implicitly defined
    this->binsize = 0;
}

PBinary::~PBinary()
{
    // implicitly defined
}

bool isdeletable(string x, string y)
{

    uint counter = 0;
    bool del = false;

    for (uint c = 0; c < x.size(); ++c)
    {
        if (x[c] != y[c])
        {
            ++counter;
            if (x[c] == '-' && y[c] == '1')
            {
                del = true;
            }
            else //(x[c] == '1' && y[c] == '-')
            {
                del = false;
                break;
            }
        }
    }

    return (del || counter == 0);
}

void PBinary::simplify()
{
    // clear unnecessary expression using boolean identity
    for (auto pair1 : this->pbins)
    {
        for (auto pair2 : this->pbins)
        {
            if (isdeletable(pair1.first, pair2.first))
            {
                // delete the second
                this->pbins.erase(pair2.first);
            }
        }
    }
}

string OR(string bin1, string bin2)
{
    string res;
    for (uint i = 0; i < bin1.length(); ++i)
    {
        if (bin1[i] == '1' || bin2[i] == '1')
        {
            res += '1';
        }
        else // neither is '1'
        {
            res += '-';
        }
    }
    return res;
}

void operator*=(PBinary &lhs, const PBinary &rhs)
{
    // and 2 PBinaries
    PBinary res = PBinary(lhs.getbinsize());
    unordered_map<string, Binary> tmp;

    for (auto x : lhs.pbins)
    {
        for (auto y : rhs.pbins)
        {
            //TODO: eliminate duplicates

            for (auto i : x.second)
            {
                tmp[i.first] = i.second;
            }

            for (auto j : y.second)
            {
                tmp[j.first] = j.second;
            }

            res.pbins[OR(x.first, y.first)] = tmp;

            tmp.clear();
        }
    }

    res.simplify();
    lhs = res;
}

void operator+=(PBinary &lhs, const PBinary &rhs)
{
    // or 2 PBinary
    PBinary res = PBinary(lhs.getbinsize());
    for (auto x : lhs.pbins)
    {
        res.pbins[x.first] = x.second;
    }

    for (auto x : rhs.pbins)
    {
        res.pbins[x.first] = x.second;
    }

    res.simplify();
    lhs = res;
}


PBinary::PBinary(uint binsize) {
    this->binsize = binsize;
}

uint PBinary::getbinsize() {
    return this->binsize;
}

PBinary& PBinary::operator=(const PBinary &rhs) {
    this->binsize = rhs.binsize;
    this->pbins = rhs.pbins;

    return *this;
}