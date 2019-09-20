# NeedleLaunch
[![License (GPL version 3)](https://img.shields.io/badge/license-GNU%20GPL%20version%203-red.svg?style=flat-square)](http://opensource.org/licenses/GPL-3.0)

*A Qlaunch rewrite that is based on deltalaunch. It aims to be different, more reliable and have more features than deltalaunch when it is done*

**Configuring:**

Before building needlelaunch, you will need to configure it. Enter the project dir (the one with this README in it) and copy examples/Config.hpp to the release/ folder.<br/> Then open release/Config.hpp and uncomment the lines depending on your needs.

**Compiling:**

You'll need devkitpro, switch-sdl2, switch-libwebp. You can install dependencies by running `pacman -S switch-sdl2 switch-sdl2_mixer switch-sdl2_gfx switch-sdl2_image switch-sdl2_ttf switch-opus`.<br/>
*Note that the above may be incorrect please wait for a final release where the final dependency list will be shared*

Once you are done with the configuration step, run the 'make' command.<br/> The output will be in the out folder.
Copy the out/titles folder to your SD Card.

**Status**

Currently it is still a WIP and I am not responsible for any issues while using this software.<br/>
Only use this if you have a NAND Backup and a fusee vulnerable switch

**Troubleshooting**

For any help and support, please make an issue on the Github Issue Tracker.<br/> Once this releases, I will make a discord for troubleshooting.

**Credits**

**XorTroll** - for StoreWithPicture from Goldleaf<br/>
**Rei** - for the original deltalaunch<br/>
**switchbrew** - for a good documentation<br/>
