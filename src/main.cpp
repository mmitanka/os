/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <iostream.h>
#include "queue.h"

#include "pcb.h"
#include "system.h"

extern int userMain(int argc, char** argv);

int main(int argc, char** argv) {
	System::inic();
	userMain(argc,argv);
	System::restore();


}

