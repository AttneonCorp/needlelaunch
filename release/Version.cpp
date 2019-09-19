#include <switch.h>
#include <switch/types.h>
#include <string>
std::string needlelaunch_get_version_info()
{
	std::string rc = "0.1a0";
	return rc;
}
std::string needlelaunch_get_credits()
{
	std::string rc = "\nDeltalaunch which is made by the ReiNX authors and AttneonCorp of course :)";
	return rc;
}
std::string needlelaunch_compile_all(){
	std::string ver = needlelaunch_get_version_info();
	std::string credits = needlelaunch_get_credits();
	std::string trailer = "\n";
	std::string rc = ver + trailer + credits + trailer;
	return rc;
}
