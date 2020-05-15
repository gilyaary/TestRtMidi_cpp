/*
 * StringTokenizer.h
 *
 *  Created on: May 7, 2020
 *      Author: gil
 */

#ifndef STRINGTOKENIZER_H_
#define STRINGTOKENIZER_H_

#include <vector>
#include <string.h>
#include <string>

class StringTokenizer {
public:
	StringTokenizer();
	virtual ~StringTokenizer();
	void tokenize(std::string cmd, std::vector<std::string> *v);
};

#endif /* STRINGTOKENIZER_H_ */
