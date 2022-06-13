#pragma once

#include <vector>
#include <map>

#include "oniguruma/oniguruma.h"

using namespace std;

typedef map<int, string> Matches;


/*
	Matches matches;
	if ( PregMatch("”\—Í‚Æ•\Œ»", "(”\)(.+)(Œ»)", &matches) ) {
		OutputDebugStream << "0:" << matches[0] << ",1:" << matches[1] << ",2:" << matches[2] << endl;
	}		
 */
int OnigMatch( string str_target, string str_regex, Matches* pmatches=NULL );


