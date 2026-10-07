#include <sstream>
#include <iostream>
#include <orbis/libkernel.h>

#include "../../_common/log.h"

// Logging
std::stringstream debugLogStream;

typedef int (*sceSystemServiceLaunchWebBrowser_t)(const char *uri, void *);

int main(void)
{
    // No buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    DEBUGLOG << "Loading libSceSystemService...";

    int32_t module = (int32_t)sceKernelLoadStartModule(
        "/system/common/lib/libSceSystemService.sprx",
        0,
        NULL,
        0,
        NULL,
        NULL
    );

    if (module < 0)
    {
        DEBUGLOG << "Failed to load libSceSystemService: 0x"
                 << std::hex << module;

        for (;;)
            sceKernelUsleep(1000000);
    }

    sceSystemServiceLaunchWebBrowser_t launchBrowser = NULL;

    int32_t result = sceKernelDlsym(
        module,
        "sceSystemServiceLaunchWebBrowser",
        (void **)&launchBrowser
    );

    if (result < 0 || launchBrowser == NULL)
    {
        DEBUGLOG << "Failed to resolve sceSystemServiceLaunchWebBrowser: 0x"
                 << std::hex << result;

        for (;;)
            sceKernelUsleep(1000000);
    }

    DEBUGLOG << "Launching PooPooWebStack...";

    result = launchBrowser(
        "https://shipitandrei.github.io/PooPooWebStack",
        NULL
    );

    DEBUGLOG << "sceSystemServiceLaunchWebBrowser returned: 0x"
             << std::hex << result;

    for (;;)
        sceKernelUsleep(1000000);

    return 0;
}
