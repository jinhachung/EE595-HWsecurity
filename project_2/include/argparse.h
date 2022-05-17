#ifndef ARGPARSE_H
#define ARGPARSE_H
#include <string.h>
#include <string>
#include <map>

class ArgParse {
private:
    std::map<std::string, std::string> _map;
public:
    ArgParse();
    void parse(int argc, char *argv[]);
    std::map<std::string, std::string>& getMap();
    std::string getValueFromKey(std::string key);
    // return value with type & default value specified
    bool getBoolFromKey(std::string key, bool df);
    int getIntFromKey(std::string key, int df);
    unsigned int getUnsignedIntFromKey(std::string key, unsigned int df);
    std::string getStringFromKey(std::string key, std::string df);
}; // class ArgParse

#endif // ARGPARSE_H
