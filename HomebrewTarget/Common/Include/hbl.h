
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <switch.h>

void hbl_TargetOnce(const char *path, const char *argv);
void hbl_Target(const char *path, const char *argv);

#ifdef __cplusplus
}
#endif