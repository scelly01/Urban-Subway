// These are the header files used in our project

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <ctype.h>
#include <vector>
#include <algorithm>

using namespace std;

// Class of the Graph Node
class test
{
public:
    // This will contain a string that will show what kind of station is on this node
    string type;
    // This will contain the name of the station
    string name;
    // This will contain the index of the amenities available
    vector<string> amenities;
};

// Global Declarations
vector<pair<int, int>> graph[101];
int dist[101];
int V = 101;
vector<int> arr;
int fare;
int timetaken;
string Gamenities[] = {"Hospital", "Cafe", "Petrol Pump"};
map<int, test> metro;

// This is the function to print the graph
void printGraph()
{
    int v, w;

    for (int u = 0; u < 101; u++)
    {
        cout << "Node " << u << " makes an edge with \n";
        for (auto it = graph[u].begin(); it != graph[u].end(); it++)
        {
            v = it->first;
            w = it->second;
            cout << "\tNode " << v << " with edge weight =" << w << "\n";
        }
        cout << "\n";
    }
}

// This function joins the stations
// That means this function connects all the nodes of the graph
void joinstats(vector<pair<int, int>> graph[], int u, int v, int wt)
{
    graph[u].push_back(make_pair(v, wt));
    graph[v].push_back(make_pair(u, wt));
}

// This function assigns the nodes a specific number
// Which is stored in the map
// We operate with the index number
// And then get the data from the map using the index
void assign(map<int, test> metro)
{
    map<int, test>::iterator ptr;
    ptr = metro.begin();

    int i = 0;
    // red
    for (; i < 21; i++, ptr++)
    {
        joinstats(graph, ptr->first, ptr->first + 1, dist[i]);
    }
    // yellow
    for (i = 22; i < 56; i++, ptr++)
    {
        joinstats(graph, ptr->first, ptr->first + 1, dist[i]);
    }

    // blue
    for (i = 57; i < 101; i++, ptr++)
    {
        joinstats(graph, ptr->first, ptr->first + 1, dist[i]);
    }

    joinstats(graph, 8, 30, 0);
    joinstats(graph, 35, 85, 0);
    // add new intersection of blue + red
    joinstats(graph, 20, 65, 0);
}

// This function is used to print the path using the parent array
// That stores the parent of each station int the graph  in a path
// and a source station
void printPath(int parent[], int j, int source)
{

    // Base Case : If j is source
    if (parent[j] == source)
        return;

    printPath(parent, parent[j], source);

    // cout<<parent[j]<<" ";
    arr.push_back(parent[j]);
}

// This function uses djikstra algorithm ans gives us the shortest distances between
// the source station and all the remaining stations of the graph
void dijkstra()
{
    int source = 0;
    cout << endl
         << endl;
    cout << "Enter your starting metro station number: ";
    cin >> source;
    cout << endl;

    if (source == 0 || source > 101)
    {
        cout << "ERROR: Wrong entry - Please try again" << endl;
        // return 0;
    }

    int dest = 0;
    cout << "Enter your Destination station number: ";
    cin >> dest;
    cout << endl;

    if (source == 0 || source > 101 || dest == source)
    {
        cout << "ERROR: Wrong entry - Please try again" << endl;
        // return 0;
    }
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> distTo(101, INT_MAX);

    int parent[101];
    parent[0] = -1;
    distTo[source] = 0;
    pq.push(make_pair(0, source)); // (dist,from)

    while (!pq.empty())
    {
        int dist = pq.top().first;
        int prev = pq.top().second;
        pq.pop();

        // parent done on my own to store path

        vector<pair<int, int>>::iterator it;
        for (it = graph[prev].begin(); it != graph[prev].end(); it++)
        {
            int next = it->first;
            int nextDist = it->second;
            if (distTo[next] > distTo[prev] + nextDist)
            {
                parent[next] = prev;
                distTo[next] = distTo[prev] + nextDist;
                pq.push(make_pair(distTo[next], next));
            }
        }
    }

    printPath(parent, dest, source);

    timetaken = distTo[dest] * 2;
    fare = distTo[dest] * 3;

    cout << "This is the most efficient path: " << endl
         << endl;
    map<int, test>::iterator itr;
    itr = metro.find(source);
    cout << itr->second.name << "->";

    for (int j = 0; j < arr.size(); j++)
    {
        if (arr[j] == 30 && arr[j - 1] == 8)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR YELLOW LINE" << endl;
        }
        if (arr[j] == 8 && arr[j - 1] == 30)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR RED LINE" << endl;
        }
        if (arr[j] == 85 && arr[j - 1] == 35)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR BLUE LINE" << endl;
        }
        if (arr[j] == 35 && arr[j - 1] == 85)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR YELLOW LINE" << endl;
        }

        itr = metro.find(arr[j]);
        cout << itr->second.name << " -> ";
    }
    itr = metro.find(dest);
    cout << itr->second.name << endl;

    cout << endl
         << endl;
    cout << "The total fare is: " << fare << " Rupees";
    cout << endl;
    cout << "The total time is: " << timetaken << " minutes" << endl
         << endl;
}

// This function also uses djikstra to get the shortest distance array
// And then sorts it according to the type that the user chooses
// And then returns the 10 nearest stations
// Based on the user's choice
void nearestType()
{
    int source = 0;
    cout << endl
         << endl;
    cout << "Enter your starting metro station number: ";
    cin >> source;
    cout << endl;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> distTo(100, INT_MAX);

    int parent[101];
    parent[0] = -1;
    distTo[source] = 0;
    pq.push(make_pair(0, source)); // (dist,from)

    while (!pq.empty())
    {
        int dist = pq.top().first;
        int prev = pq.top().second;
        pq.pop();

        // parent done on my own to store path

        vector<pair<int, int>>::iterator it;
        for (it = graph[prev].begin(); it != graph[prev].end(); it++)
        {
            int next = it->first;
            int nextDist = it->second;
            if (distTo[next] > distTo[prev] + nextDist)
            {
                parent[next] = prev;
                distTo[next] = distTo[prev] + nextDist;
                pq.push(make_pair(distTo[next], next));
            }
        }
    }

    // Here we are sorting the array distTo
    // We have used a sorting algorithm
    sort(distTo.begin(), distTo.end());
    cout << "1.Historical\n";
    cout << "2.Entertainment\n";
    cout << "3.Parks\n";
    cout << "4.Shopping\n";
    cout << "Enter your choice: ";
    int choice, count = 0;
    cin >> choice;
    string str;
    if (choice == 1)
        str = "Historical";
    if (choice == 2)
        str = "Entertainment";
    if (choice == 3)
        str = "Parks";
    if (choice == 4)
        str = "Shopping";

    vector<pair<int, test>> nearestStationIndex;

    map<int, test>::iterator itr;
    cout << endl
         << "These are all the nearest stations of your choice" << endl;
    for (itr = metro.begin(); count != 10; ++itr)
    {
        if (itr->second.type == str)
        {
            count++;
            nearestStationIndex.push_back(make_pair(itr->first, itr->second));
            cout << itr->first << " -> " << itr->second.name << endl;
        }
    }

    cout << "Choose a station you want to visit\n";
    cout << "Enter your choice: ";
    int stnChoice;
    cin >> stnChoice;
    cout << endl;

    printPath(parent, stnChoice, source);

    timetaken = distTo[stnChoice] * 2;
    fare = distTo[stnChoice] * 3;

    // cout << "This is the most efficient path: " << endl
    //  << endl;
    // map<int, test>::iterator itr;
    itr = metro.find(source);
    cout << itr->second.name << "->";

    for (int j = 0; j < arr.size(); j++)
    {
        if (arr[j] == 30 && arr[j - 1] == 8)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR YELLOW LINE" << endl;
        }
        if (arr[j] == 8 && arr[j - 1] == 30)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR RED LINE" << endl;
        }
        if (arr[j] == 85 && arr[j - 1] == 35)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR BLUE LINE" << endl;
        }
        if (arr[j] == 35 && arr[j - 1] == 85)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR YELLOW LINE" << endl;
        }

        itr = metro.find(arr[j]);
        cout << itr->second.name << " -> ";
    }
    itr = metro.find(stnChoice);
    cout << itr->second.name << endl;

    cout << endl
         << endl;
    cout << "The total fare is: " << fare << " Rupees";
    cout << endl;
    cout << "The total time is: " << timetaken << " minutes" << endl
         << endl;
    // for (int i = 0; i < nearestStationIndex.size(); ++i)
    // {
    //     cout << nearestStationIndex[i].name << endl;
    // }
}

// This function assigns a path using djikstra algorithm's parent array
void assign_ans_path_dijkstra(int parent[], int j, int s, vector<int> &ans_path)
{
    // Base Case : If j is source
    if (parent[j] == s)
        return;

    assign_ans_path_dijkstra(parent, parent[j], s, ans_path);

    // cout << parent[j] << " ";
    ans_path.push_back(parent[j]);
    // cout << "Done done";
}

// This function gives us a cycle from the graph
// That is used to give thhe route for the Delhi Tour

// Here user's sourrce station is connected to the cycle
// And the user gets a path to start from the source station
// And come back to the same
void tsp()
{
    // declaring our path
    // error: 3 of the nodes are common ////     IMPPPPP
    vector<int> path_list; // will store indexes of stations that for th cycle
    int sno = 0;

    for (int i = 8; i <= 20; ++i)
        path_list.push_back(i);
    for (int i = 65; i <= 85; ++i)
        path_list.push_back(i);
    for (int i = 35; i >= 30; --i)
        path_list.push_back(i);

    cout << endl;
    cout << endl;
    int s; // source station index
    cout << "Enter your current station number" << endl;
    cin >> s;

    bool isInCycle = false;
    int index;
    for (int i = 0; i < path_list.size(); i++) // Check if s is in cycle
    {
        if (s == path_list[i])
        {
            isInCycle = true;
            index = i;
        }
    }

    vector<int> ans_path; // stores the final ans path

    // for (int i = 0; i < ans_path.size(); ++i)
    // {
    //     cout << ans_path[i] << " ";
    // }
    int intersect_station_index;
    int intersect_pt_index;
    if (isInCycle)
    {
        // int ans_index = 0;
        for (int i = index; i < path_list.size(); i++)
        {
            ans_path.push_back(path_list[i]);
        }
        for (int i = 0; i <= index; i++)
        {
            ans_path.push_back(path_list[i]);
        }
    }
    else // s is outside the cycle
    {
        // dijkstra on s and path_list
        // import distTo[]
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> distTo(100, INT_MAX);

        int parent[100];
        parent[0] = -1;
        distTo[s] = 0;
        pq.push(make_pair(0, s)); // (dist,from)
        // cout << "T1";
        while (!pq.empty())
        {
            int dist = pq.top().first;
            int prev = pq.top().second;
            pq.pop();
            vector<pair<int, int>>::iterator it;
            for (it = graph[prev].begin(); it != graph[prev].end(); it++)
            {
                int next = it->first;
                int nextDist = it->second;
                if (distTo[next] > distTo[prev] + nextDist)
                {
                    parent[next] = prev;
                    // cout << parent[next] << "  ";
                    distTo[next] = distTo[prev] + nextDist;
                    // cout << distTo[next] << endl;
                    pq.push(make_pair(distTo[next], next));
                }
            }
        }

        // get min index in dijkstra ans + add path to ans_path
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq_of_distTo;
        for (int i = 0; i < 100; i++)
        {
            if ((i >= 8 && i <= 20))
                pq_of_distTo.push({distTo[i], i});
            if ((i >= 30 && i <= 35))
                pq_of_distTo.push({distTo[i], i});
            if ((i >= 65 && i <= 85))
                pq_of_distTo.push({distTo[i], i});
        }
        intersect_pt_index = pq_of_distTo.top().second;
        assign_ans_path_dijkstra(parent, intersect_pt_index, s, ans_path);
        cout << endl;

        // find path from intersect_pt_index inside cycle
        // repeat what you did in isInCycle condition
        // will get updated ans_index

        for (int i = 0; i < 40; ++i)
        {
            if (path_list[i] == intersect_pt_index)
                intersect_station_index = i;
        }
        for (int i = intersect_station_index; i < 40; i++)
        {
            ans_path.push_back(path_list[i]);
        }
        for (int i = 0; i <= intersect_station_index; i++)
        {
            ans_path.push_back(path_list[i]);
        }
    }

    // printing final path
    cout << endl
         << endl
         << "Here is our recommended tour path for you: " << endl
         << endl;
    map<int, test>::iterator itr;
    itr = metro.find(s);
    // cout << itr->second.name << "->";

    for (int j = 0; j < ans_path.size(); j++)
    {
        if (ans_path[j] == 30 && ans_path[j - 1] == 8)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR YELLOW LINE" << endl
                 << endl;
        }
        if (ans_path[j] == 8 && ans_path[j - 1] == 30)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR RED LINE" << endl
                 << endl;
        }
        if (ans_path[j] == 85 && ans_path[j - 1] == 35)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR BLUE LINE" << endl
                 << endl;
        }
        if (ans_path[j] == 35 && ans_path[j - 1] == 85)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR YELLOW LINE" << endl
                 << endl;
        }
        if (ans_path[j] == 65 && ans_path[j - 1] == 20)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR BLUE LINE" << endl
                 << endl;
        }
        if (ans_path[j] == 20 && ans_path[j - 1] == 65)
        {
            cout << endl
                 << endl
                 << "CHANGE FOR RED LINE" << endl
                 << endl;
        }
        itr = metro.find(ans_path[j]);
        cout << itr->second.name << " -> ";
        // cout << itr->first << " -> ";
    }
    // printing reverse dijkstra path
    // itr = metro.find(intersect_station_index);
    // cout << itr->second.name << "->";

    for (int l = intersect_station_index - 1; l >= 0; l--)
    {
        itr = metro.find(ans_path[l]);
        cout << itr->second.name << " -> ";
        // cout << itr->first << " -> ";
    }
    cout << endl
         << endl
         << endl;
}

// This function is used to take input from the files
// We have already put in the data in the files
// And have used file handling to get the data
void input()
{
    // inputing station and distance list
    fstream newfile1, newfile2 /*newfile4*/;
    newfile1.open("stations.txt", ios::in);
    newfile2.open("type.txt", ios::in);
    // newfile4.open("amenities.txt", ios::in);
    if (newfile1.is_open() && newfile2.is_open())
    {
        test t1;
        int i = 1;
        // string line;
        while (getline(newfile1, t1.name) && getline(newfile2, t1.type) /*&& getline(newfile4,line)*/)
        {
            // stringstream sstream(line);
            // string index;

            //      char del=' ';
            //      while (std::getline(sstream, index, del))
            //      {
            //          t1.amenities.push_back(index);
            //      }

            metro.insert(pair<int, test>(i, t1));
            i++;
        }
    }
    newfile1.close();
    newfile2.close();
    // newfile4.close();

    fstream newfile3;
    newfile3.open("distances.txt", ios::in);
    if (newfile3.is_open())
    {
        string readdis;
        int i = 0;
        while (getline(newfile3, readdis))
        {
            dist[i] = stoi(readdis);
            i++;
        }
    }
    newfile3.close();
}

// This function is used to display the metro station list from the map
void disp()
{
    // display
    map<int, test>::iterator itr;
    cout << "List of functioning metro stations:" << endl
         << endl;
    cout << endl
         << "RED LINE-" << endl;
    for (itr = metro.begin(); itr != metro.end(); ++itr)
    {

        int i = 0;
        if (itr->first == 22)
        {
            cout << endl
                 << "YELLOW LINE-" << endl;
        }
        if (itr->first == 57)
        {
            cout << endl
                 << "BLUE LINE-" << endl;
        }
        cout << " " << itr->first << " -> " << itr->second.name << " (" << itr->second.type << ")";
        //      itr->second.+printAmenities();
        cout << endl;
    }

    //  metro[9].printAmenities();
}

// This function is used in the end of the program
// To thank the user for visiting THE URBAN SUBWAY
void thanks()
{
    cout << endl;
    cout << endl;
    cout << "THANK YOU FOR VISITNG THE URBAN SUBWAY!!";
    cout << endl;
    cout << endl;
    cout << endl;
}

// This is the main menu program
// Here we have used switch statements to give the user choices to choose from
// The user can choose any choice and then they can come back to the main menu
// If they want to
void menu()
{
    int ch;
    char ans;
    cout << endl;
    do
    {
        cout << "1.Get shortest route between two stations\n";
        cout << "2.Get a tour of Delhi\n";
        cout << "3.Get a list of 10 nearest metro stations based on a given type\n";
        cout << "Enter your choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1: // printGraph();
            dijkstra();
            break;

        case 2:
            tsp();
            break;

        case 3:
            nearestType();
            break;

        default:
            cout << "\nWrong input, Enter your choice again";
        }
        cout << "\nDo you want to continue?\nY/N: ";
        cin >> ans;
    } while (ans == 'y' || ans == 'Y');

    thanks();
}

// This is the main function
int main()
{

    cout << "***WELCOME TO THE URBAN SUUBWAY***" << endl
         << endl;

    // Taking input from files
    input();

    // Display all metro stations
    disp();

    // making graph
    assign(metro);

    // Displaying menu
    menu();

    return 0;
}
