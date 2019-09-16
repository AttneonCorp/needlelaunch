#include <HbTarget.hpp>
#include <switch.h>
#include <stdio.h>
#include <string.h>

u32 __nx_applet_type = AppletType_SystemApplication;
u32 __nx_applet_exit_mode = 2;

char entry[2048] = {0};
char entryargs[2048] = {0};

void CheckPushParameters()
{
    appletInitialize();
    AppletStorage params;
    Result rc = appletPopLaunchParameter(&params, AppletLaunchParameterKind_Application);
    if(rc == 0)
    {
        s64 st_size = 0;
        appletStorageGetSize(&params, &st_size);
        HbTargetArgs args = {};
        if(st_size >= sizeof(HbTargetArgs))
        {
            appletStorageRead(&params, 0, &args, sizeof(HbTargetArgs));
            if(args.Magic == HbTargetMagic)
            {
                if(strlen(args.Entry))
                {
                    strcpy(entry, args.Entry);
                    if(strlen(args.Argv))
                    {
                        strcpy(entry, args.Argv);
                    }
                }
            }
        }
        appletStorageClose(&params);
    }
    appletExit();
}

int main()
{
    entry[0] = '\0';
    entryargs[0] = '\0';

    CheckPushParameters();

    if(entry[0] == '\0') strcpy(entry, "sdmc:/hbmenu.nro");
    if(entryargs[0] == '\0') strcpy(entryargs, entry);

    hbl_Target(entry, entryargs);

    return 0;
}