file(GLOB CONTEXT_MODULE_SOURCES "./Context/*.cpp")
file(GLOB CONTEXT_MODULE_HEADERS "./Context/*.h")

file(GLOB IMAGELOADER_MODULE_SOURCES "./ImageLoader/*.cpp")
file(GLOB IMAGELOADER_MODULE_HEADERS "./ImageLoader/*.h")

file(GLOB MUTILTHREADBASE_MODULE_SOURCES "./MutilThreadBase/*.cpp")
file(GLOB MUTILTHREADBASE_MODULE_HEADERS "./MutilThreadBase/*.h")

file(GLOB OBJECT_MODULE_SOURCES "./Object/*.cpp")
file(GLOB OBJECT_MODULE_HEADERS "./Object/*.h")

file(GLOB PHYSICS_MODULE_SOURCES "./Physics/*.cpp")
file(GLOB PHYSICS_MODULE_HEADERS "./Physics/*.h")

file(GLOB RUNTIME_MODULE_SOURCES "./Runtime/*.cpp")
file(GLOB RUNTIME_MODULE_HEADERS "./Runtime/*.h")

file(GLOB SHAPES_MODULE_SOURCES "./Shapes/*.cpp")
file(GLOB SHAPES_MODULE_HEADERS "./Shapes/*.h")

file(GLOB TEXT_MODULE_SOURCES "./Text/*.cpp")
file(GLOB TEXT_MODULE_HEADERS "./Text/*.h")

file(GLOB TOOLS_MODULE_SOURCES "./Tools/*.cpp")
file(GLOB TOOLS_MODULE_HEADERS "./Tools/*.h")

file(GLOB UI_MODULE_SOURCES "./UI/*.cpp")
file(GLOB UI_MODULE_HEADERS "./UI/*.h")

add_library(FCT STATIC 
    ${CONTEXT_MODULE_SOURCES}
    ${CONTEXT_MODULE_HEADERS}
    ${IMAGELOADER_MODULE_SOURCES}
    ${IMAGELOADER_MODULE_HEADERS}
    ${MUTILTHREADBASE_MODULE_SOURCES}
    ${MUTILTHREADBASE_MODULE_HEADERS}
    ${OBJECT_MODULE_SOURCES}
    ${OBJECT_MODULE_HEADERS}
    ${PHYSICS_MODULE_SOURCES}
    ${PHYSICS_MODULE_HEADERS}
    ${RUNTIME_MODULE_SOURCES}
    ${RUNTIME_MODULE_HEADERS}
    ${SHAPES_MODULE_SOURCES}
    ${SHAPES_MODULE_HEADERS}
    ${TEXT_MODULE_SOURCES}
    ${TEXT_MODULE_HEADERS}
    ${TOOLS_MODULE_SOURCES}
    ${TOOLS_MODULE_HEADERS}
    ${UI_MODULE_SOURCES}
    ${UI_MODULE_HEADERS}
    "headers.h"
)

find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(freeimage CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread locale)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FCT PROPERTY CXX_STANDARD 17)
endif()


target_include_directories(FCT PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/libnoise/include 
)

target_link_libraries(FCT PUBLIC 
    glad
    unofficial::omniverse-physx-sdk::sdk
    glfw
    harfbuzz::harfbuzz 
    harfbuzz::harfbuzz-subset
    freeimage::FreeImage freeimage::FreeImagePlus
    Freetype::Freetype
    libnoise
    ${Boost_LIBRARIES} 
)

if(TARGET unofficial::omniverse-physx-sdk::gpu-library)
    if(UNIX)
        # Add rpath setting to find .so libraries on unix based systems
        set_target_properties(FCT PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "$ORIGIN"
        )
    endif()
    add_custom_command(TARGET FCT POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:unofficial::omniverse-physx-sdk::gpu-library>
                    $<TARGET_FILE_DIR:FCT>)
    if(WIN32)
        add_custom_command(TARGET FCT POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:unofficial::omniverse-physx-sdk::gpu-device-library>
                        $<TARGET_FILE_DIR:FCT>)
    endif()
else()
    message(WARNING "GPU acceleration library target not defined - GPU acceleration will NOT be available!")
endif()
