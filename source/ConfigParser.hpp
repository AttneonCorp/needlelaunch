// Parser for config.hpp in release/Config.hpp

#include "../release/Config.hpp"
#ifndef CONFIG_HPP_INCL
	#error "No config.hpp is present"
#endif
#ifndef CONFIG
	#error "You must select a CFW"
#endif
#ifdef LEGACY
	#error "LEGACY is not specific enough"
#endif
