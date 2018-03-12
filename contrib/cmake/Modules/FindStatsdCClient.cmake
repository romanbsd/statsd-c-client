# - Try to find statsdcclient
# Once done this will define
#
#  STATSDCCLIENT_FOUND - system has statsd-c-client
#  STATSDCCLIENT_INCLUDE_DIRS - the statsd-c-client include directory
#  STATSDCCLIENT_LIBRARIES - Link these to use statsd-c-client
#
#  Copyright (c) 2018 Martin Duncan <mduncan@duncanpope.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (STATSDCCLIENT_LIBRARIES AND STATSDCCLIENT_INCLUDE_DIRS)
  # in cache already
  set(STATSDCCLIENT_FOUND TRUE)
else (STATSDCCLIENT_LIBRARIES AND STATSDCCLIENT_INCLUDE_DIRS)
  find_path(STATSDCCLIENT_INCLUDE_DIR
    NAMES
      statsd-client.h
    PATHS
      /usr/include
      /usr/local/include
      /usr/local/include/statsd
      /opt/local/include
      /sw/include
  )

find_library(STATSDCCLIENT_LIBRARY
    NAMES
      statsdclient
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

set(STATSDCCLIENT_INCLUDE_DIRS
  ${STATSDCCLIENT_INCLUDE_DIR}
  )

if (STATSDCCLIENT_LIBRARY)
  set(STATSDCCLIENT_LIBRARIES
    ${STATSDCCLIENT_LIBRARIES}
    ${STATSDCCLIENT_LIBRARY}
    )
endif (STATSDCCLIENT_LIBRARY)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(StatsdCClient DEFAULT_MSG
    STATSDCCLIENT_LIBRARIES STATSDCCLIENT_INCLUDE_DIRS)

  # show the STATSDCCLIENT_INCLUDE_DIRS and STATSDCCLIENT_LIBRARIES variables only in the advanced view
  mark_as_advanced(STATSDCCLIENT_INCLUDE_DIRS STATSDCCLIENT_LIBRARIES)

endif (STATSDCCLIENT_LIBRARIES AND STATSDCCLIENT_INCLUDE_DIRS)
