set(LIB_NAME Network)
AUX_SOURCE_DIRECTORY(./src NETWORK_SRCS)
file(GLOB_RECURSE NETWORK_HEADERS
    includes/*.h
)
add_library(${LIB_NAME} STATIC
    ${NETWORK_SRCS}
    ${NETWORK_HEADERS}
)
target_link_libraries(${LIB_NAME} PRIVATE 
    Utils 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Core 
    EasyBMP
    allocators
)
target_include_directories(${LIB_NAME} PUBLIC includes)
