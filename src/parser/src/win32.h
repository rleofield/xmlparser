
#ifndef RL_XML_WIN32_H
#define RL_XML_WIN32_H

#include  <iostream>

#ifdef _WIN32
#pragma warning( disable : 4291 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4804 )

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

const std::locale locDE( "German_germany" );

//#include <mutex>

#else
const std::locale locDE( "de_DE.utf8" );
#endif



#endif
// EOF

