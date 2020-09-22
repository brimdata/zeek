include(ZeekSubdir)

bro_add_subdir_library(windows
    fcntl.c
    fts.c
    netdb.c
    regex.c
    stdio.c
    stdlib.c
    strptime.c
    sys.resource.c
    time.c
    unistd.c
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/usr.include/
    DESTINATION include/zeek
)
