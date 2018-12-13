#ifndef macros_hpp
#define macros_hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <fstream>
using namespace std;

struct macro_header;
struct inst_header;

struct inst_header {

    // not a macro instruction
    string x;
    string y;
    int l;

    // macro instruction
    struct macro_header *mac;
    vector <string> input_args;
    vector <string> output_args;
    vector <int> init_vals;
};

struct macro_header {
    vector <string> inputs; // input var names
    vector <string> outputs; // output var names
    vector <struct inst_header> insts; // instruction list
};

typedef struct inst_header inst;
typedef struct macro_header macro;

void init_macros();

#endif
