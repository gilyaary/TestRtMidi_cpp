/*
 * ReceiveMessagesCmd.cpp
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#include "ReceiveMessagesCmd.h"

namespace commands {

//TODO: we need to have fields for each section of the command string
//this class acts as a parser for the command. It can also do validation but that would throw an exception
//a better idea may be to have a static builder function to build an object, validate it and return it or throw a validation exception
//an abstract factory with buildObjectFromString(str) may be the best idea. This will make the actual commands simple
ReceiveMessagesCmd::ReceiveMessagesCmd() {
	// TODO Auto-generated constructor stub

}

ReceiveMessagesCmd::~ReceiveMessagesCmd() {
	// TODO Auto-generated destructor stub
}

void ReceiveMessagesCmd::receiveMessages(std::string cmd){

}

} /* namespace commands */
