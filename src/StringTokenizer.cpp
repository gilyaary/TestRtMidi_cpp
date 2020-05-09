/*
 * StringTokenizer.cpp
 *
 *  Created on: May 7, 2020
 *      Author: gil
 */

#include "StringTokenizer.h"

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>


StringTokenizer::StringTokenizer() {
	// TODO Auto-generated constructor stub

}

StringTokenizer::~StringTokenizer() {
	// TODO Auto-generated destructor stub
}

void StringTokenizer::tokenize(char* chars, std::vector<std::string> *v){
	v->clear();
	char *token = strtok(chars, ".");
	while (token != NULL)
	{
		std::cout << "Pushed Token: " << token << "\n";
		v->push_back(token);
		token = strtok(NULL, ".");
	}
	std::cout << "Token Vector Size: " << v->size() << "\n";
}
