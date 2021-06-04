#include "Common.h"
#include "Util/Resources.h"

//SPRX File
extern char _binary_Resources_Orbis_Toolbox_bin_start[];
extern char _binary_Resources_Orbis_Toolbox_bin_end[];

//Icons
extern char _binary_Resources_icon_daemon_bin_start[];
extern char _binary_Resources_icon_daemon_bin_end[];

extern char _binary_Resources_icon_payload_bin_start[];
extern char _binary_Resources_icon_payload_bin_end[];

extern char _binary_Resources_icon_pkg_bin_start[];
extern char _binary_Resources_icon_pkg_bin_end[];

extern char _binary_Resources_icon_plugin_bin_start[];
extern char _binary_Resources_icon_plugin_bin_end[];

extern char _binary_Resources_icon_reboot_bin_start[];
extern char _binary_Resources_icon_reboot_bin_end[];

extern char _binary_Resources_icon_reload_ui_bin_start[];
extern char _binary_Resources_icon_reload_ui_bin_end[];

extern char _binary_Resources_icon_shutdown_bin_start[];
extern char _binary_Resources_icon_shutdown_bin_end[];

extern char _binary_Resources_icon_suspend_bin_start[];
extern char _binary_Resources_icon_suspend_bin_end[];

extern char _binary_Resources_icon_system_settings_bin_start[];
extern char _binary_Resources_icon_system_settings_bin_end[];

extern char _binary_Resources_icon_toolbox_bin_start[];
extern char _binary_Resources_icon_toolbox_bin_end[];

#define _Write_File(File, Start, End) Write_File(File, Start, (size_t)(End - Start))

int Write_File(const char* File, char* Data, size_t size)
{
    int fd = sceKernelOpen(File, 0x200 | 0x0002, 0777);
    if(fd)
    {
        Log("Writing File \"%s\" %i...", File, size);

        sceKernelWrite(fd, Data, size);

        sceKernelClose(fd);

        return 1;
    }
    else
    {
        Log("Failed to make file \"%s\"\n", File);
        return 0;
    }
}

int MakeDir(char* Dir, ...)
{
    char buffer[0x400] = { 0 };
    va_list args;
    va_start(args, Dir);
    vsprintf(buffer, Dir, args);
    va_end(args);

    Log("Creating Directory \"%s\"...", buffer);
    return sceKernelMkdir(buffer, 0777);
}

void Install_Daemon(const char* TitleId, char* Eboot_start, char* Eboot_end, char* Param_sfo_start, char* Param_sfo_end, char* Icon_start, char* Icon_end)
{
    Log("Installing Daemon %s", TitleId);

    Log("Creating Directories...");
    MakeDir("/system/vsh/app/%s", TitleId);
    MakeDir("/system/vsh/app/%s/sce_module", TitleId);
    MakeDir("/system/vsh/app/%s/sce_sys", TitleId);

    Log("Writing Files...");
    char Eboot_Dir[PATH_MAX], Param_sfo_Dir[PATH_MAX], Icon_Dir[PATH_MAX];
    sprintf(Eboot_Dir, "/system/vsh/app/%s/eboot.bin", TitleId);
    sprintf(Param_sfo_Dir, "/system/vsh/app/%s/sce_sys/icon0.png", TitleId);
    sprintf(Icon_Dir, "/system/vsh/app/%s/sce_sys/param.sfo", TitleId);

    _Write_File("/data/Orbis Toolbox/Icons/icon_daemon.png", Eboot_start, Eboot_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_payload.png", Param_sfo_start, Param_sfo_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_pkg.png", Icon_start, Icon_end);

    Log("Installing Daemon %s Complete!", TitleId);
}

void Install_Resources()
{
    Log("Making Directories...");
    MakeDir("/data/Orbis Toolbox");
    MakeDir("/data/Orbis Toolbox/Plugins");
    MakeDir("/data/Orbis Toolbox/Icons");
    MakeDir("/data/Orbis Toolbox/Payloads");

    Log("Writing Files...");

    //Main Toolbox Module
    _Write_File("/data/Orbis Toolbox/Orbis Toolbox.sprx", _binary_Resources_Orbis_Toolbox_bin_start, _binary_Resources_Orbis_Toolbox_bin_end);

    //Toolbox Supporting Assets.
    _Write_File("/data/Orbis Toolbox/Icons/icon_daemon.png", _binary_Resources_icon_daemon_bin_start, _binary_Resources_icon_daemon_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_payload.png", _binary_Resources_icon_payload_bin_start, _binary_Resources_icon_payload_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_pkg.png", _binary_Resources_icon_pkg_bin_start, _binary_Resources_icon_pkg_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_plugin.png", _binary_Resources_icon_plugin_bin_start, _binary_Resources_icon_plugin_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_reboot.png", _binary_Resources_icon_reboot_bin_start, _binary_Resources_icon_reboot_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_reload_ui.png", _binary_Resources_icon_reload_ui_bin_start, _binary_Resources_icon_reload_ui_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_shutdown.png", _binary_Resources_icon_shutdown_bin_start, _binary_Resources_icon_shutdown_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_suspend.png", _binary_Resources_icon_suspend_bin_start, _binary_Resources_icon_suspend_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_system_settings.png", _binary_Resources_icon_system_settings_bin_start, _binary_Resources_icon_system_settings_bin_end);
    _Write_File("/data/Orbis Toolbox/Icons/icon_toolbox.png", _binary_Resources_icon_toolbox_bin_start, _binary_Resources_icon_toolbox_bin_end);

    Log("Installing Daemons...");
    //Install_Daemon("PLDR00000", ); //Install payload loader daemon

    Log("Install_Resources() -> Sucess!");
}