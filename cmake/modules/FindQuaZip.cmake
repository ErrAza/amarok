find_package(Qt4)
find_path(QuaZip_INCLUDE_DIR quazip.h ${CMAKE_INSTALL_PREFIX}/include/quazip ${CMAKE_INSTALL_PREFIX}/include /usr/include/quazip /usr/local/include/quazip ${QT_INCLUDE_DIR}/quazip ${QT_INCLUDE_DIR} ${QUAZIP_DIR}/include/quazip ${QUAZIP_DIR}/quazip ${QUAZIP_DIR}/include)
find_library(QuaZip_LIBRARY NAMES quazip PATHS ${CMAKE_INSTALL_PREFIX}/lib64 ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/Library/Frameworks ${QUAZIP_DIR}/lib64 ${QUAZIP_DIR}/lib ${QUAZIP_DIR}/quazip ${QUAZIP_DIR})
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(quazip DEFAULT_MSG QuaZip_LIBRARY QuaZip_INCLUDE_DIR)
set(QuaZip_LIBRARIES ${QuaZip_LIBRARY})
mark_as_advanced(QuaZip_LIBRARY QuaZip_INCLUDE_DIR)

if(QuaZip_LIBRARY AND QuaZip_INCLUDE_DIR)s
    set(QuaZip_FOUND TRUE)
endif()