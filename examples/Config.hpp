// The config file for needlelaunch

// Build for ReiNX
//#define REINX

// Build for Atmosphere
//#define AMS

// Build for SX OS (Unsupported)
//#define SXOS

// Experimental: Enable W.I.P code
//#define WIP

// Internal Stuff... Do not touch
#ifdef REINX
	#undef AMS
	#undef SXOS
	#undef NOT_RNX
	#define CONFIG
#endif
#ifdef AMS
	#undef REINX
	#undef SXOS
	#define CONFIG
	#define NOT_RNX
#endif
#ifdef SXOS
	#undef REINX
	#undef AMS
	#define CONFIG
	#define NOT_RNX
#endif
#ifdef WIP
	#warning "WIP Stuff is experimental"
#endif
#define CONFIG_HPP_INCL

// Done!
