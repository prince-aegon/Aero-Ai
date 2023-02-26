#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>

#define REP(i, a, b) for (int i = (a); i <= (b); i++)
#define REP_Dec(i, a, b) for (int i = (a); i >= (b); i--)
#define FOR(i, n) for (int i = 0; i < n; i++)
#define FOR1(i, n) for (int i = 1; i <= n; i++)
#define print(n) cout << n << endl
#define mp make_pair
#define pb push_back
#define eb emplace_back

using namespace std;

typedef long long int ll;
const int inf = 1e9;
const ll N = 1000000000000L;

class Runway
{
public:
    int id;
    string call_sign;
    int length;
};
enum class CSVState
{
    UnquotedField,
    QuotedField,
    QuotedQuote
};

std::vector<std::string> readCSVRow(const std::string &row)
{
    CSVState state = CSVState::UnquotedField;
    std::vector<std::string> fields{""};
    size_t i = 0; // index of the current field
    for (char c : row)
    {
        switch (state)
        {
        case CSVState::UnquotedField:
            switch (c)
            {
            case ',': // end of field
                fields.push_back("");
                i++;
                break;
            case '"':
                state = CSVState::QuotedField;
                break;
            default:
                fields[i].push_back(c);
                break;
            }
            break;
        case CSVState::QuotedField:
            switch (c)
            {
            case '"':
                state = CSVState::QuotedQuote;
                break;
            default:
                fields[i].push_back(c);
                break;
            }
            break;
        case CSVState::QuotedQuote:
            switch (c)
            {
            case ',': // , after closing quote
                fields.push_back("");
                i++;
                state = CSVState::UnquotedField;
                break;
            case '"': // "" -> "
                fields[i].push_back('"');
                state = CSVState::QuotedField;
                break;
            default: // end of quote
                state = CSVState::UnquotedField;
                break;
            }
            break;
        }
    }
    return fields;
}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::string>> readCSV(std::istream &in)
{
    std::vector<std::vector<std::string>> table;
    std::string row;
    while (!in.eof())
    {
        std::getline(in, row);
        if (in.bad() || in.fail())
        {
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);
    }
    return table;
}

void simple_tokenizer(string s)
{
    stringstream ss(s);
    string word;
    while (ss >> word)
    {
        cout << word << endl;
    }
}

// A quick way to split strings separated via any character
// delimiter.
void adv_tokenizer(string s, char del)
{
    stringstream ss(s);
    string word;
    while (!ss.eof())
    {
        getline(ss, word, del);
        cout << word << endl;
    }
}

int main()
{
    fstream fin;

    fin.open("data/runway.csv", ios::in);
    vector<Runway> runway(5);
    string line, word, temp;

    int row = 0;
    while (fin.good())
    {
        getline(fin, line, '\n');

        stringstream ss(line);
        string word;

        int i = 0;
        while (!ss.eof())
        {
            getline(ss, word, ',');
            if (row != 0)
            {
                if (i == 0)
                {
                    runway[row].id = stoi(word);
                }
                else if (i == 1)
                {
                    runway[row].call_sign = word;
                }
                else
                {
                    runway[row].length = stoi(word);
                }
            }
            i++;
        }
        row++;
    }
    runway.erase(runway.begin());
    runway.resize(4);

    for (int i = 0; i < runway.size(); i++)
    {
        cout << runway[i].id << " " << runway[i].call_sign << " " << runway[i].length << endl;
    }

    return 0;
}