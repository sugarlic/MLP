set(LIB_NAME custom_plot)
AUX_SOURCE_DIRECTORY(./src ${LIB_NAME}_SRCS)
file(GLOB_RECURSE ${LIB_NAME}_HEADERS
    include/*.h
)
add_library(${LIB_NAME} STATIC
    ${${LIB_NAME}_SRCS}
    ${${LIB_NAME}_HEADERS}
)
target_link_libraries(${LIB_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
)
target_include_directories(${LIB_NAME} PUBLIC include)
