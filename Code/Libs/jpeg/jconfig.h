// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

// Try to keep this free from platform-specific stuff. Use CMakeLists.txt for that.

#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
#define HAVE_STDLIB_H
#define NO_GETENV

// Should only be needed when using XDK. Do it everywhere to be consistent.
typedef unsigned char boolean;
#ifndef FALSE
	#define FALSE 0	
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#define HAVE_BOOLEAN
