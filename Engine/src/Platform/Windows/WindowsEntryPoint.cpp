#include "vxpch.h"
#include "WindowsEntryPoint.h"






static void UnEscapeQuotes(char* arg)
{
    char* last = NULL;

    while (*arg) {
        if (*arg == '"' && (last != NULL && *last == '\\')) {
            char* c_curr = arg;
            char* c_last = last;

            while (*c_curr) {
                *c_last = *c_curr;
                c_last = c_curr;
                c_curr++;
            }
            *c_last = '\0';
        }
        last = arg;
        arg++;
    }
}
// Parse a command line buffer into arguments 
static int ParseCommandLine(char* cmdline, char** argv)
{
    char* bufp;
    char* lastp = NULL;
    int argc, last_argc;

    argc = last_argc = 0;
    for (bufp = cmdline; *bufp;) {
        /* Skip leading whitespace */
        while (*bufp == ' ' || *bufp == '\t') {
            ++bufp;
        }
        /* Skip over argument */
        if (*bufp == '"') {
            ++bufp;
            if (*bufp) {
                if (argv) {
                    argv[argc] = bufp;
                }
                ++argc;
            }
            /* Skip over word */
            lastp = bufp;
            while (*bufp && (*bufp != '"' || *lastp == '\\')) {
                lastp = bufp;
                ++bufp;
            }
        }
        else {
            if (*bufp) {
                if (argv) {
                    argv[argc] = bufp;
                }
                ++argc;
            }
            /* Skip over word */
            while (*bufp && (*bufp != ' ' && *bufp != '\t')) {
                ++bufp;
            }
        }
        if (*bufp) {
            if (argv) {
                *bufp = '\0';
            }
            ++bufp;
        }

        /* Strip out \ from \" sequences */
        if (argv && last_argc != argc) {
            UnEscapeQuotes(argv[last_argc]);
        }
        last_argc = argc;
    }
    if (argv) {
        argv[argc] = NULL;
    }
    return (argc);
}


//MessageBoxA(NULL, "Memory allocation failed!", "Error", MB_OK | MB_ICONERROR);
static int OutOfMemory(void)
{
#if UNICODE
    MessageBox(NULL, L"Out of memory - aborting", L"Fatal Error", MB_OK | MB_ICONERROR);
#else
	MessageBox(NULL, "Out of memory - aborting", "Fatal Error", MB_OK | MB_ICONERROR);
#endif
    return BIT(32);
}

int VERTEX_API Vertex_main_getcmdline(int(*vx_main)(int, char**))
{
    char** argv = nullptr;
    int argc = 0;
    char* cmdline = nullptr;
    int retval = 0;
    int cmdalloc = 0;
    const TCHAR* text = GetCommandLine();
    const TCHAR* ptr;
    int argc_guess = 2;  // space for NULL and initial argument.
    int rc;

    // Estimate number of arguments.
    for (ptr = text; *ptr; ++ptr) {
        if ((*ptr == ' ') || (*ptr == '\t')) {
            ++argc_guess;
        }
    }

    // Allocate memory for arguments and the command line string.
#if UNICODE
    rc = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
    if (rc > 0)
    {
        cmdalloc = rc + (sizeof(char*) * argc_guess);
        argv = static_cast<char**>(VirtualAlloc(nullptr, cmdalloc, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
        if (argv) {
            cmdline = reinterpret_cast<char*>(argv + argc_guess);
            int rc2 = WideCharToMultiByte(CP_UTF8, 0, text, -1, cmdline, rc, nullptr, nullptr);
            VX_CORE_ASSERT(rc2 == rc);
        }
    }
#else
    rc = static_cast<int>(strlen(text)) + 1;
    cmdalloc = rc + (sizeof(char*) * argc_guess);
    argv = static_cast<char**>(VirtualAlloc(nullptr, cmdalloc, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    if (argv) {
        cmdline = reinterpret_cast<char*>(argv + argc_guess);
        strcpy(cmdline, text);
    }
#endif

    if (!cmdline) {
        return OutOfMemory();
    }

    // Parse the command line into argc and argv
    argc = ParseCommandLine(cmdline, argv);

    
    // Call the main function with parsed arguments
    retval = vx_main(argc, argv);
    

    // Free allocated memory
    VirtualFree(argv, cmdalloc, MEM_DECOMMIT);
    VirtualFree(argv, 0, MEM_RELEASE);

    return retval;
}

