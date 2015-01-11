# TODO: not a plattform independet make file
#  LIBCURL_FOUND - system has the LIBCURL library
#  LIBCURL_INCLUDE_DIR - the LIBCURL include directory
#  LIBCURL_LIBRARY - The libraries needed to use LIBCURL

set(LIBCURL_FOUND true)
set(LIBCURL_INCLUDE_DIR /usr/lib/x86_64-linux-gnu/)
set(LIBCURL_LIBRARY /usr/lib/x86_64-linux-gnu/libcurl.so)
mark_as_advanced(LIBCURL_INCLUDE_DIR LIBCURL_LIBRARY)
