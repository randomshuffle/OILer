/* 
 * OILer : a one-instruction language interpreter 
 * Anubhav Baweja
 * Andrew ID: abaweja
 */

#include "macros.hpp"
using namespace std;

macro prog; // we will obtain information from the user, 
// parse it into this data structure and then use our macro_runner
// function on it

string tmp; // useful variable

extern unordered_map <string, macro* > all_the_macros; 

void get_words(string s, bool input_yes) {
    
    stringstream ss;
    ss.str(s);
    while (ss >> tmp) {
        if (input_yes) prog.inputs.push_back(tmp);
        else prog.outputs.push_back(tmp);
    }
}


vector <int> macro_runner(macro m, vector <int> init_vals) {

    int n = m.insts.size() - 1;
    unordered_map <string, int> db;

    // initialize inputs
    for (int i = 0; i < m.inputs.size(); i++) {
        db.insert(make_pair(m.inputs[i], init_vals[i]));
    }

    // insert the two intermediate variables, 
    // instructions should not modify these
    db.insert(make_pair("_ONEVAR", 1));
    db.insert(make_pair("_ZEROVAR", 0));
    
    // run the program
    int cur_pos = 1;
    while (cur_pos != n+1) {

        inst my_inst = m.insts[cur_pos];
        
        // macro used at this line
        if (my_inst.mac != NULL) {

            // init values for macro at this line
            vector <int> init_vals_for_macro;
            for (int i = 0; i < my_inst.input_args.size(); i++) {
                init_vals_for_macro.push_back(db[my_inst.input_args[i]]);
            }

            // run macro
            vector <int> results = macro_runner(*(my_inst.mac), init_vals_for_macro);
            //for (int i = 0; i < results.size(); i++) cout<<results[i]<<" ";
            //cout<<endl;   

            // obtain return values from the macro
            for (int i = 0; i < my_inst.output_args.size(); i++) {
                if (db.find(my_inst.output_args[i]) == db.end()) 
                    db.insert(make_pair(my_inst.output_args[i], results[i]));
                else db[my_inst.output_args[i]] = results[i];
            }

            cur_pos++;
        }

        // regular instruction
        else {
            string x = my_inst.x;
            string y = my_inst.y;
            int l = my_inst.l;
            
            if (db.find(x) == db.end()) db.insert(make_pair(x, 0));
            if (db.find(y) == db.end()) db.insert(make_pair(y, 0));
            
            db[x] -= db[y];
            
            if (db[x] == 0) cur_pos = l;
            else cur_pos++;
            
        }

        /*
        if (macname == "top") {
            cout<<"Position: "<<cur_pos<<endl;
            for (auto it = db.begin(); it != db.end(); ++it) {
                cout<<it->first<<": "<<it->second<<endl;
            }
            cout<<endl;
        }
        */
    }

    // prepare outputs
    vector <int> res;
    for (int i = 0; i < m.outputs.size(); i++) {
        res.push_back(db[m.outputs[i]]);
    }

    return res;
}


int main () {
   
    init_macros();

    cout<<"----- OILer: a one-instruction language interpreter -----\n\n";

    // Obtain input variables
    cout<<"Specify names of input variables (separated by spaces): ";
    getline(cin, tmp); 
    get_words(tmp, 1);

    cout<<"Specify names of output variables (separated by spaces): ";
    getline(cin, tmp); 
    get_words(tmp, 0);

    vector <int> var_vals(prog.inputs.size());
    cout<<"\nEnter inputs for the variables: \n";
    for (int i = 0; i < prog.inputs.size(); i++) {
        cout<<prog.inputs[i]<<": ";
        cin>>var_vals[i];
    } 
    
    cout<<"\nSpecify n (the number of instructions): ";
    int n;
    cin>>n;

    cout<<"Enter the "<<n<<" instructions: \n";

    // clear cin buffer    
    char c;
    while (cin.get(c) && c != '\n') {
        if (!std::isspace(c)) {
            std::cerr << "ERROR unexpected character '" << c << "' found\n";
            exit(EXIT_FAILURE);
        } 
    }

    prog.insts.resize(n+1);
    for (int i = 1; i <= n; i++) {

        int k;
        string s;
        getline(cin, tmp);

        for (int i = 0; i < tmp.length(); i++) {
            if (tmp[i] == ' ') {
                k = stoi(tmp.substr(0, i));
                s = tmp.substr(i+1);
                break;
            }
        }

        
        // parsing the string s
        inst foo;
        
        // macro
        if (s[0] == '[') {
            
            string macro_name = s.substr(0, s.find("]") + 1);

            auto it = all_the_macros.find(macro_name);
            if (it == all_the_macros.end()) {
                cout<<"No such macro exists. Reenter instruction\n";
                i--;
                goto next_step;
            }
            
            vector <string> my_inputs;
            vector <string> my_outputs;
            
            stringstream ss;
            ss.str(s.substr(s.find("]") + 1));

            bool input_yes = 1;
            unordered_set <string> found_yet; // checks for duplicate variables
            while (ss >> tmp) {

                if (tmp == ";") {
                    input_yes = 0;
                    continue;
                }

                if (found_yet.find(tmp) != found_yet.end()) {
                    cout<<"Invalid arguments to the macro (same variables). Reenter instruction\n";
                    i--;
                    goto next_step;
                }

                if (input_yes) my_inputs.push_back(tmp);
                else my_outputs.push_back(tmp);
            }

            int num_inputs = it->second->inputs.size();
            int num_outputs = it->second->outputs.size();

            if (num_inputs != my_inputs.size() || num_outputs != my_outputs.size()) {
                cout<<"Incorrect number of variables for macro. Reenter instruction\n";
                i--;
                goto next_step;
            }

            foo.mac = it->second;
            foo.input_args = my_inputs;
            foo.output_args = my_outputs;
        }

        // not a macro
        else {
            stringstream ss;
            ss.str(s);
            ss>>foo.x;
            ss>>foo.y;
            ss>>tmp;
            foo.l = stoi(tmp);
            foo.mac = NULL;
        }

        // set the kth instruction
        prog.insts[k] = foo;
        next_step:;
    }

    // print outputs
    cout<<"\nOutputs:\n";
    vector <int> final_result = macro_runner(prog, var_vals); 
    for (int i = 0; i < prog.outputs.size(); i++) {
        cout<<prog.outputs[i]<<": "<<final_result[i]<<endl;
    }

    return 0; 
}
