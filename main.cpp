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

map<string, pair<double, pair<int, int>>> sections;
vector<int> shortestPath;
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

class Graph
{
    int V;
    vector<int> *adj;

public:
    Graph(int V);
    void addEdge(int v, int w, int weight);

    double findShortestPath(int s, int d);

    int printShortestPath(int parent[], int s, int d);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new vector<int>[2 * V];
}

void Graph::addEdge(int v, int w, int weight)
{
    // split all edges of weight 2 into two
    // edges of weight 1 each. The intermediate
    // vertex number is maximum vertex number + 1,
    // that is V.
    if (weight == 2)
    {
        adj[v].push_back(v + V);
        adj[v + V].push_back(w);
    }
    else                     // Weight is 1
        adj[v].push_back(w); // Add w to v’s list.
}

// To print the shortest path stored in parent[]
int Graph::printShortestPath(int parent[], int s, int d)
{
    static int level = 0;

    // If we reached root of shortest path tree
    if (parent[s] == -1)
    {
        // cout << "Shortest Path between " << s << " and "
        //      << d << " is " << s << " ";
        shortestPath.push_back(s);
        return level;
    }

    printShortestPath(parent, parent[s], d);

    level++;
    if (s < V)
    {
        // cout << s << " ";
        shortestPath.push_back(s);
    }

    return level;
}

// This function mainly does BFS and prints the
// shortest path from src to dest. It is assumed
// that weight of every edge is 1
double Graph::findShortestPath(int src, int dest)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[2 * V];
    int *parent = new int[2 * V];

    // Initialize parent[] and visited[]
    for (int i = 0; i < 2 * V; i++)
    {
        visited[i] = false;
        parent[i] = -1;
    }

    // Create a queue for BFS
    queue<int> queue;

    // Mark the current node as visited and enqueue it
    visited[src] = true;
    queue.push(src);

    // 'i' will be used to get all adjacent vertices of a vertex
    vector<int>::iterator i;

    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        int s = queue.front();

        if (s == dest)
            return printShortestPath(parent, s, dest);

        queue.pop();

        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it
        // visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push(*i);
                parent[*i] = s;
            }
        }
    }
}

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

            sections[tag].first = value;
        }
        getline(ss, word, ',');
        if (row != 0)
        {
            value = stod(word);

            sections[tag].second.first = value;
        }
        getline(ss, word, ',');
        if (row != 0)
        {
            value = stod(word);

            sections[tag].second.second = value;
        }

        row++;
    }
    for (auto const &x : sections)
    {
        cout << x.first << " (" << x.second.first << " ), ( " << x.second.second.first << ", " << x.second.second.second << ")" << endl;
    }
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

    // queue<string> q_path;
    // q_path.push("N2");
    // q_path.push("T");
    // q_path.push("N4");

    // Aircraft curr_aircraft = aircrafts[2];
    // double total_time = 0;
    // while (!q_path.empty())
    // {
    //     string curr = q_path.front();
    //     q_path.pop();
    //     double dist = sections[curr];
    //     total_time += dist * curr_aircraft.vel;
    // }
    // cout << curr_aircraft.name << " took " << total_time << " minutes" << endl;

    // creating graph
    int v = 22;
    Graph paths(v);
    for (auto const &x : sections)
    {
        paths.addEdge(x.second.second.first, x.second.second.second, x.second.first);
    }
    int src = 0, des = 21;
    double ans = paths.findShortestPath(src, des);
    ans = 0;
    Aircraft curr_aircraft = aircrafts[2];
    cout << endl;

    for (int i = 0; i < shortestPath.size(); i++)
    {
        for (auto const &x : sections)
        {
            if ((shortestPath[i] == x.second.second.first && shortestPath[i + 1] == x.second.second.second) ||
                (shortestPath[i] == x.second.second.second && shortestPath[i + 1] == x.second.second.first))
            {
                ans += x.second.first;
                break;
            }
        }
    }
    cout << "Total time taken by " << curr_aircraft.name << " to go from " << src << " to " << des << " is : " << curr_aircraft.vel * ans << endl;
    for (int i = 0; i < shortestPath.size(); i++)
    {
        cout << shortestPath[i] << " ";
    }
    cout << endl;
    return 0;
}