#include <HbTarget.hpp>
#include <switch.h>
#include <stdio.h>
#include <string.h>

u32 __nx_applet_type = AppletType_LibraryApplet;
u32 __nx_applet_exit_mode = 2;

char entry[2048] = {0};
char entryargs[2048] = {0};
bool once = false;

void CheckStorages()
{
    appletInitialize();
    AppletStorage la;
    Result rc = appletLibPopInData(&la);
    if(R_SUCCEEDED(rc))
    {
        LibAppletArgs la_args;
        appletStorageRead(&la, 0, &la_args, sizeof(la_args));
        if(la_args.LaVersion == HbTargetMagic)
        {
            once = la_args.PlayStartupSound;
            AppletStorage params;
            rc = appletLibPopInData(&params);
            if(R_SUCCEEDED(rc))
            {
                s64 st_size;
                appletStorageGetSize(&params, &st_size);
                HbTargetArgs args;
                memset(&args, 0, sizeof(HbTargetArgs));
                if((u64)st_size >= sizeof(HbTargetArgs))
                {
                    appletStorageRead(&params, 0, &args, sizeof(HbTargetArgs));
                    if(args.Magic == HbTargetMagic)
                    {
                        if(strlen(args.Entry))
                        {
                            strcpy(entry, args.Entry);
                            if(strlen(args.Argv))
                            {
                                strcpy(entryargs, args.Argv);
                            }
                        }
                    }
                }
                appletStorageClose(&params);
            }
        }
        appletStorageClose(&la);
    }
    appletExit();
}

int main()
{
    entry[0] = '\0';
    entryargs[0] = '\0';

    CheckStorages();

    if(entry[0] == '\0') strcpy(entry, "sdmc:/hbmenu.nro");
    if(entryargs[0] == '\0') strcpy(entryargs, entry);

    if(once) hbl_TargetOnce(entry, entryargs);
    else hbl_Target(entry, entryargs);

    return 0;
}