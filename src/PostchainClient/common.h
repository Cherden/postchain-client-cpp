#pragma once

#define POSTCHAIN_BYTE_MIN 0
#define POSTCHAIN_BYTE_MAX 255

namespace chromia {
	typedef unsigned char byte;
}  // namespace chromia

// If compilation is outside UE, or in UE version 4, then define BYTE as const char. 
// In UE version 5, there is already BYTE structure available

#ifdef CHROMIA_INSIDE_UNREAL_ENGINE
	#include "Runtime/Launch/Resources/Version.h"
	#if ENGINE_MAJOR_VERSION == 4
		namespace chromia {
			typedef unsigned char BYTE;
		}  // namespace chromia
	#endif
#else
	namespace chromia {
		typedef unsigned char BYTE;
	}  // namespace chromia
#endif

//#ifndef BYTE
//#define BYTE byte
//#endif

