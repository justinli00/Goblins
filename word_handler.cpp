/*
* Justin W Li
* word_handler.cpp
* word handler class implementations
*/

#include "word_handler.h"
#include <fstream>      //std::fstream
#include <cctype>       //toupper 


#include <algorithm>


word_handler::word_handler() : word_bank() {}

void word_handler::load_bank() {
    //open file
    std::ifstream words;
    words.open("words.txt", std::ifstream::in);
    if (!words.is_open())
    {
        //file failed to open
        throw "word(): failed to open file!\n";
        return;
    }

    //read each line -- program expects one word per line
    std::string str;
    while (getline(words, str))
    {
        //check that length of string doesn't exceed max word length of word bank
        if (str.size() >= word_bank.size())
        {
            //resize to accommodate 
            word_bank.resize(str.size() + 1);
        }

        //insert into appropriate slot
        word_bank[str.size()].push_back(str);
    }
    words.close();
}

const std::string word_handler::get_string(unsigned int type) const {

    //determine upper, lower bounds of string length
    long unsigned int range = 0, start = 0;
    switch (type)
    {
    case 0:
        range = 2;
        start = 3;
        break;
    case 1:
        range = 3;
        start = 5;
        break;
    case 2:
        range = 3;
        start = 8;
        break;
    case 3:
        range = 3;
        start = 11;
        break;
    case 4:
        range = word_bank.size() - 14;
        start = 14;
        break;
    default:
        throw "get_string(): invalid type!\n";
    }

    //generate random word length that has words
    long unsigned int word_length = start;
    do
    {
        word_length = static_cast<long unsigned int>(rand()) % range + start;
    } while (word_bank[word_length].empty());

    //return random word with that length in word bank
    return word_bank[word_length][static_cast<long unsigned int>(rand()) % word_bank[word_length].size()];
}

bool word_handler::string_compare(std::string str1, const std::string str2) const {
    //check that string length is same
    if (str1.size() != str2.size())
        return false;
    else
    {
        //check each letter against the other
        long unsigned int strlen = str1.size();
        for (long unsigned int i = 0; i < strlen; ++i)
        {
            //convert both to upper case
            if (toupper(str1[i]) != toupper(str2[i]))
                return false;
        }
    }
    return true;
}

