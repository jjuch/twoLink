#include "manageSettingsFile.hpp"


/* #include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp> */

using namespace std;
// using namespace boost::property_tree;
using namespace sf;


SettingsFile::SettingsFile(string path) {
    cout << "constructor" << endl;
    ifstream f(path.c_str());
    cout << path << endl;

    f.open(path.c_str());
    if (f.is_open()) {
        cout << "file opened" << endl;
        // SettingsFile::file = f;
    } else {
        cout << "file not found" << endl;
    }
};

SettingsFile::~SettingsFile() { 
    cout << "deconstructor" << endl;
};
