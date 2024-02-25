AUX_SOURCE_DIRECTORY(./src ${P_NAME}_SRCS)
file(GLOB_RECURSE ${P_NAME}_HEADERS
    includes/*.h
)
file(GLOB_RECURSE ${P_NAME}_UIS
    UI/*.ui
)
set(CMAKE_AUTOUIC_SEARCH_PATHS ./UI)
set(PROJECT_SOURCES
    ${${P_NAME}_SRCS}
    ${${P_NAME}_HEADERS}
    ${${P_NAME}_UIS}
    ${${P_NAME}_RES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${P_NAME}
        MANUAL_FINALIZATION
        MACOSX_BUNDLE   
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(${P_NAME} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${P_NAME}
            ${PROJECT_SOURCES}
        )
    endif() 
endif()

target_link_libraries(${P_NAME} PRIVATE
    Network
    Utils
    EasyBMP
    custom_plot
    allocators
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Widgets
)
foreach(RESOURCE_FILE ${RESOURCE_FILES})
    file(RELATIVE_PATH RELATIVE_PATH_TO_RESOURCE ${RES_DIR} ${RESOURCE_FILE})
    add_custom_command(
        TARGET ${P_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RES_DIR}/${RELATIVE_PATH_TO_RESOURCE}
            $<TARGET_FILE_DIR:${P_NAME}>/${RES_NAME}/${RELATIVE_PATH_TO_RESOURCE}
    )
endforeach()
target_include_directories(${P_NAME} PUBLIC 
    ./includes
)
set_target_properties(${P_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ${P_NAME}_esi
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
