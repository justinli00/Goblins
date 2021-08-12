/*
* Justin W Li
* word_handler.h
* word handler class definition
*/

#ifndef WORD_HANDLER_H
#define WORD_HANDLER_H

#include <vector>	//std::vector
#include <string>	//std::string
#include <fstream>	//std::fstream

//reads in word bank, provides word and spell checks
class word_handler {
	std::vector<std::vector<std::string>> word_bank;						//list of alphabetized word list, sorted by length
public:
	word_handler();															//ctor
	void load_bank();														//loads word bank from file -- will likely take some time
	const std::string get_string(unsigned int type) const;							//gets string based on enemy type
	bool string_compare(std::string str1, const std::string str2) const;	//case-insensitive string comparison function


	//throwaway functions to get process words.txt -- todo -- delete
};

#endif