# cmake macro to test LibV4LConvert

# Copyright (c) 201, Sebastian Krzyszkowiak <dos@dosowisko.net>
#
# https://github.com/dos1/kamerka/blob/master/modules/FindLibV4LConvert.cmake
#
# LIBV4LCONVERT_FOUND
# LIBV4LCONVERT_INCLUDE_DIR
# LIBV4LCONVERT_LIBRARY
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

INCLUDE (FindPackageHandleStandardArgs)

IF (LIBV4LCONVERT_INCLUDE_DIR AND LIBV4LCONVERT_LIBRARY)
    # Already in cache, be silent
    SET (LIBV4LCONVERT_FIND_QUIETLY TRUE)
ENDIF (LIBV4LCONVERT_INCLUDE_DIR AND LIBV4LCONVERT_LIBRARY)

IF (NOT LIBV4LCONVERT_FOUND)
  FIND_PATH (LIBV4LCONVERT_INCLUDE_DIR libv4lconvert.h)
  FIND_LIBRARY (LIBV4LCONVERT_LIBRARY NAMES v4lconvert)

  IF (LIBV4LCONVERT_INCLUDE_DIR AND LIBV4LCONVERT_LIBRARY)
    SET (LIBV4LCONVERT_FOUND TRUE)
  ENDIF( LIBV4LCONVERT_INCLUDE_DIR AND LIBV4LCONVERT_LIBRARY )
ENDIF ( NOT LIBV4LCONVERT_FOUND)

IF( LIBV4LCONVERT_FOUND )
  IF( NOT LIBV4LCONVERT_FIND_QUIETLY )
    MESSAGE( STATUS "Found LIBV4LCONVERT: ${LIBV4LCONVERT_LIBRARY}")
  ENDIF( NOT LIBV4LCONVERT_FIND_QUIETLY )
ENDIF( LIBV4LCONVERT_FOUND )

