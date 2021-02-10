/* File config.txt:
num = 123
str = hello
flt = 12.2
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Config {
    int    porta;
    string str;
    double flt;
};

void loadConfig(Config& config) {
    ifstream fin("config.txt");
    string line;
    while (getline(fin, line)) {
        istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("porta") != -1)
            sin >> config.porta;
        else if (line.find("str") != -1)
            sin >> config.str;
        else if (line.find("flt") != -1)
            sin >> config.flt;
    }
}

int main() {
    Config config;
    loadConfig(config);
    cout << config.porta << '\n';
    cout << config.str << '\n';
    cout << config.flt << '\n';
}
