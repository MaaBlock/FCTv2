include(ExternalProject)

# ���� PhysX �� Git �ֿ�Ͱ汾
set(PHYSX_GIT_REPOSITORY "https://github.com/NVIDIA-Omniverse/PhysX.git")
set(PHYSX_GIT_TAG "106.5-physx-5.5.1")

# ���� PhysX �İ�װĿ¼
set(PHYSX_INSTALL_DIR "${CMAKE_BINARY_DIR}/physx-install")

# ���� PX_OUTPUT_BASE_DIR ��Ϊ����Ŀ¼
set(PX_OUTPUT_BASE_DIR "${CMAKE_BINARY_DIR}/physx-output")

# ���� ExternalProject �����غ͹��� PhysX
ExternalProject_Add(
    physx_external
    GIT_REPOSITORY ${PHYSX_GIT_REPOSITORY}
    GIT_TAG ${PHYSX_GIT_TAG}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${PHYSX_INSTALL_DIR}
        -DPHYSX_ROOT_DIR=<SOURCE_DIR>/physx
        -DPX_BUILDSNIPPETS=OFF
        -DPX_BUILDPUBLICSAMPLES=OFF
        -DPX_GENERATE_STATIC_LIBRARIES=ON
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DPHYSX_BUILD_TYPE=$<IF:$<CONFIG:Debug>,debug,checked>
        -DPX_OUTPUT_BIN_DIR=${PX_OUTPUT_BASE_DIR}
        -DPX_OUTPUT_LIB_DIR=${PX_OUTPUT_BASE_DIR}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    SOURCE_SUBDIR physx/compiler/public
    BUILD_ALWAYS 1
)

# ����һ�������Ŀ������ʾ PhysX ��
add_library(PhysX INTERFACE)
add_dependencies(PhysX physx_external)

# ���� PhysX �İ���Ŀ¼
target_include_directories(PhysX INTERFACE 
    ${PHYSX_INSTALL_DIR}/include
)

# ���� PhysX �Ŀ�Ŀ¼
if(WIN32)
    set(PHYSX_LIB_DIR "${PX_OUTPUT_BASE_DIR}/bin/win.x86_64.$<IF:$<CONFIG:Debug>,debug,checked>")
elseif(ANDROID)
    set(PHYSX_LIB_DIR "${PX_OUTPUT_BASE_DIR}/bin/android.arm64-v8a.$<IF:$<CONFIG:Debug>,debug,checked>")
else()
    set(PHYSX_LIB_DIR "${PX_OUTPUT_BASE_DIR}/bin/linux.clang.$<IF:$<CONFIG:Debug>,debug,checked>")
endif()

target_link_directories(PhysX INTERFACE ${PHYSX_LIB_DIR})

# ���� PhysX ��
target_link_libraries(PhysX INTERFACE 
    PhysXExtensions_static_64
    PhysX_static_64
    PhysXPvdSDK_static_64
    PhysXCommon_static_64
    PhysXFoundation_static_64
    PhysXCooking_static_64
)

# ��ӱ�Ҫ�ı��붨��
target_compile_definitions(PhysX INTERFACE 
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)

# ����� Linux �� Android����Ӷ�������ӿ�
if(UNIX AND NOT APPLE)
    target_link_libraries(PhysX INTERFACE pthread dl)
endif()