#include "argparse.h"
#include <iostream>
#include <cstdlib>

ArgParse::ArgParse()
{
}

void ArgParse::parse(int argc, char *argv[])
{
    std::string str;
    for (int i = 1; i < argc; ++i) {
        if ((argv[i][0] == '-') &&
            (argv[i][1] == '-')) {
            str = argv[i];
            this->_map[str.substr(2)] = argv[i+1];
        }
    }
}

std::map<std::string, std::string>& ArgParse::getMap()
{
    return this->_map;
}

std::string ArgParse::getValueFromKey(std::string key)
{
    // returns empty string if not found
    auto it = this->_map.find(key);
    if (it == this->_map.end()) {
        // not found
        return "";
    }
    // found
    return it->second;
}

bool ArgParse::getBoolFromKey(std::string key, bool df)
{
    std::string str_value = this->getValueFromKey(key);
    if (!str_value.compare("true")) {
        return true;
    }
    else if (!str_value.compare("false")) {
        return false;
    }
    return df;
}

int ArgParse::getIntFromKey(std::string key, int df)
{
    std::string str_value = this->getValueFromKey(key);
    if (str_value.compare("")) {
        // found
        return atoi(str_value.c_str());
    }
    // not found
    return df;
}

unsigned int ArgParse::getUnsignedIntFromKey(std::string key, unsigned int df)
{
    std::string str_value = this->getValueFromKey(key);
    if (str_value.compare("")) {
        // found
        return (unsigned int)atoi(str_value.c_str());
    }
    // not found
    return df;
}

std::string ArgParse::getStringFromKey(std::string key, std::string df)
{
    std::string str_value = this->getValueFromKey(key);
    if (str_value.compare("")) {
        // found
        return str_value;
    }
    // not found
    return df;
}
