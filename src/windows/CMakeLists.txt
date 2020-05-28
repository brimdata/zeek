include(ZeekSubdir)

bro_add_subdir_library(windows
    dlfcn.c
    fcntl.c
    fnmatch.c
    fts.c
    glob.c
    netdb.c
    regex.c
    stdio.c
    stdlib.c
    strptime.c
    sys.resource.c
    time.c
    unistd.c
)
