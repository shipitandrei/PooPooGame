#include <orbis/libkernel.h>

typedef int (*sceSystemServiceLaunchWebBrowser_t)(const char *, void *);

static void *browser_thread(void *)
{
    sceKernelUsleep(10000000);

    int32_t module = (int32_t)sceKernelLoadStartModule(
        "/system/common/lib/libSceSystemService.sprx",
        0,
        NULL,
        0,
        NULL,
        NULL
    );

    if (module < 0)
        return NULL;

    sceSystemServiceLaunchWebBrowser_t launchBrowser = NULL;

    int32_t result = sceKernelDlsym(
        module,
        "sceSystemServiceLaunchWebBrowser",
        (void **)&launchBrowser
    );

    if (result < 0 || launchBrowser == NULL)
        return NULL;

    launchBrowser(
        "https://shipitandrei.github.io/PooPooWebStack",
        NULL
    );

    return NULL;
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    OrbisPthread thread;

    int32_t result = scePthreadCreate(
        &thread,
        NULL,
        browser_thread,
        NULL,
        "browser"
    );

    if (result < 0)
    {
        for (;;)
            sceKernelUsleep(1000000);
    }

    scePthreadJoin(thread, NULL);

    for (;;)
        sceKernelUsleep(1000000);

    return 0;
}
