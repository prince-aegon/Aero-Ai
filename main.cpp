#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <map>
#include <queue>

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

map<string, double> sections;

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

class Aircraft
{
public:
    int id;
    string name;
    double vel;
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

    // capture the runway data

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

    // for (int i = 0; i < row - 1; i++)
    // {
    //     cout << runway[i].id << " " << runway[i].call_sign << " " << runway[i].length << endl;
    // }
    fin.close();

    // capture the section data

    fin.open("data/sections.csv", ios::in);

    row = 0;
    while (fin.good())
    {
        getline(fin, line, '\n');

        stringstream ss(line);
        string word, tag;
        double value;
        getline(ss, word, ',');
        tag = word;
        getline(ss, word, ',');
        if (row != 0)
        {
            value = stod(word);

            sections[tag] = value;
        }
        row++;
    }
    // for (auto const &x : sections)
    // {
    //     cout << x.first << " " << x.second << endl;
    // }
    fin.close();

    // capture the aircraft data

    fin.open("data/aircraft.csv", ios::in);

    vector<Aircraft> aircrafts(12);

    row = 0;
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
                    aircrafts[row].id = stoi(word);
                }
                else if (i == 1)
                {
                    aircrafts[row].name = (word);
                }
                else
                {
                    aircrafts[row].vel = stod(word) * 10;
                }
            }
            i++;
        }

        row++;
    }
    aircrafts.erase(aircrafts.begin());

    // for (int i = 0; i < row - 1; i++)
    // {
    //     cout << aircrafts[i].id << " " << aircrafts[i].name << " " << aircrafts[i].vel << endl;
    // }

    fin.close();

    // assigning a demo path

    queue<string> q_path;
    q_path.push("N2");
    q_path.push("T");
    q_path.push("N4");
    q_path.push("N13A");

    Aircraft curr_aircraft = aircrafts[2];
    double total_time = 0;
    while (!q_path.empty())
    {
        string curr = q_path.front();
        q_path.pop();
        double dist = sections[curr];
        total_time += dist * curr_aircraft.vel;
    }
    cout << curr_aircraft.name << " took " << total_time << " minutes" << endl;

    return 0;
}