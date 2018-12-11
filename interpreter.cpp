#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
using namespace std;

typedef struct inst_header {
    string x;
    string y;
    int l;
} inst;

vector <inst> prog;
unordered_map <string, int> db;
int n;

void get_words(string s) {
    
    stringstream ss;
    ss.str(s);
    string tmp;
    while (ss >> tmp) {
        if (db.find(tmp) == db.end()) db.insert(make_pair(tmp, 0));
    }
}

int main () {
    
    cout<<"----- OILer: a one-instruction language interpreter -----\n\n";

    string tmp;

    cout<<"Specify names of input variables (separated by spaces): ";
    getline(cin, tmp); 
    get_words(tmp);

    cout<<"Enter inputs for the variables: \n";
    for (auto it = db.begin(); it != db.end(); ++it) {
        cout<<it->first<<": ";
        cin>>it->second;
    } 
    
    cout<<"Specify n (the number of instructions): ";
    cin>>n;

    cout<<"Enter the "<<n<<" instructions: \n";

    prog.resize(n+1);
    int init_pos = 1;
    for (int i = 1; i <= n; i++) {
        int k, l; 
        string x, y;
        cin>>k>>x>>y>>l;

        if (i == 1) init_pos = k;
        
        inst foo;
        foo.x = x;
        foo.y = y;
        foo.l = l;
        prog[k] = foo;
    }

    // run the program
    int cur_pos = init_pos;
    while (cur_pos != n+1) {
        
        string x = prog[cur_pos].x;
        string y = prog[cur_pos].y;
        int l = prog[cur_pos].l;

        if (db.find(x) == db.end()) db.insert(make_pair(x, 0));
        if (db.find(y) == db.end()) db.insert(make_pair(y, 0));
        db[x] -= db[y];
        if (db[x] == 0) cur_pos = l;
        else cur_pos++;
    }

    cout<<"What variables do you need the output for? \n";
    while (1) {
        cout<<"Enter variable name (Ctrl-C to exit): ";
        cin>>tmp;
        if (db.find(tmp) == db.end()) cout<<"No such variable\n";
        else cout<<tmp<<" = "<<db.find(tmp)->second<<"\n";
    }
    
    
}
