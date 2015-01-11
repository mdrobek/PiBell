# TODO: not a plattform independet make file
#  LIBCURL_FOUND - system has the LIBCURL library
#  LIBCURL_INCLUDE_DIR - the LIBCURL include directory
#  LIBCURL_LIBRARY - The libraries needed to use LIBCURL

#set(LIBCURL_FOUND true)
#set(LIBCURL_INCLUDE_DIR /usr/include/)
#set(LIBCURL_LIBRARY /usr/lib/arm-linux-gnueabihf/libcurl.so)

find_path(LIBCURL_INCLUDE_DIR curl/curl.h HINTS /usr/include/curl/curl.h)
find_library(LIBCURL_LIBRARY NAMES curl libcurl HINTS /usr/lib/arm-linux-gnueabihf/)

set(LIBCURL_LIBRARY ${LIBCURL_LIBRARY} )
set(LIBCURL_INCLUDE_DIR ${LIBCURL_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libcurl  DEFAULT_MSG
                                  LIBCURL_LIBRARY LIBCURL_INCLUDE_DIR)

mark_as_advanced(LIBCURL_INCLUDE_DIR LIBCURL_LIBRARY)
