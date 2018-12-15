#include "macros.hpp"
using namespace std;

unordered_map <string, macro* > all_the_macros;

void init_macros() {

    vector <string> files;

    // insert macros here
    files.push_back("ADD");
    files.push_back("SUB");
    files.push_back("COPY");
    files.push_back("PARITY");
    files.push_back("POS");
    files.push_back("MIN");
    files.push_back("ABS");
    files.push_back("MULT");
    files.push_back("EXP");
    files.push_back("DIVMOD");

    for (int f = 0; f < files.size(); f++) {

        macro* m = new macro;

        string filename = "macro_list/" + files[f] + ".txt";
        string macroname = "[" + files[f] + "]";

        ifstream macrofile(filename);

        string tmp, tmp2;
        stringstream ss1, ss2;
        
        getline(macrofile, tmp); 
        ss1.str(tmp);
        while (ss1 >> tmp2) m->inputs.push_back(tmp2);

        getline(macrofile, tmp); 
        ss2.str(tmp);
        while (ss2 >> tmp2) m->outputs.push_back(tmp2);

        int n;
        macrofile>>n;

        // clear cin buffer    
        char c;
        while (macrofile.get(c) && c != '\n') {
            if (!std::isspace(c)) {
                std::cerr << "ERROR unexpected character '" << c << "' found\n";
                exit(EXIT_FAILURE);
            } 
        }

        
        m->insts.resize(n+1);
        for (int i = 1; i <= n; i++) {

            int k;
            string s;
            getline(macrofile, tmp);

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
            m->insts[k] = foo;
            next_step:;
        }

        all_the_macros.insert(make_pair(macroname, m));
        macrofile.close();
    }
        
    return;
}
