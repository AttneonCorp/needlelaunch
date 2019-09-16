
#pragma once
#include <hbl.h>

struct HbTargetArgs
{
    u32 Magic;
    char Entry[2048];
    char Argv[2048];
};

static constexpr u32 HbTargetMagic = 0x47544248;