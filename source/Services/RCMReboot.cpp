#include <switch.h>
#include <switch/types.h>
static inline void RebootToRcm() {
    SecmonArgs args = {0};
    args.X[0] = 0xC3000401; /* smcSetConfig */
    args.X[1] = 65001; /* Exosphere reboot */
    args.X[3] = 1; /* Perform reboot to RCM. */
    svcCallSecureMonitor(&args);
}
