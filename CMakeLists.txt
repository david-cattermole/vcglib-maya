cmake_minimum_required(VERSION 2.6)
project(vcgNodes)

# constant names
set(TRIDECM_CMD_NAME decimator)
set(MESH_STATS_CMD_NAME meshStats)
set(MESH_OP_NAME meshOp)
set(VCG_NODES_NAME vcgNodes)

# Ensure we are set to use C++ 11, even if CMake lacks the ability to change this (version prior to 3.1).
if (CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS "-std=c++0x")
    endif(CMAKE_COMPILER_IS_GNUCC)
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

# Compile Flags, Release flags come from the Autodesk Maya build scripts.
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++0x -O0 -g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-std=c++0x -DBits64_ -m64 -DUNIX -D_BOOL -DLINUX -DFUNCPROTO -D_GNU_SOURCE -DLINUX_64 -fPIC -fno-strict-aliasing -DREQUIRE_IOSTREAM -O3 -Wall -Wno-multichar -Wno-comment -Wno-sign-compare -funsigned-char -pthread -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-deprecated -Wno-reorder -ftemplate-depth-25 -fno-gnu-keywords")

# VCG
set(VCG_INCLUDE_PATH "/usr/local/include" CACHE PATH "vcglib include directory")

# Standalone utilities?
set(WITH_STANDALONE ON CACHE BOOLEAN "Build the standalone utilities?")
set(WITH_DECIMATOR ON CACHE BOOLEAN "Build the standalone decimator utility?")
set(WITH_MESH_STATS ON CACHE BOOLEAN "Build the standalone meshStats utility?")

# Maya SDK
set(WITH_MAYA OFF CACHE BOOLEAN "Build the Maya plugins?")
set(MAYA_INCLUDE_PATH "/usr/autodesk/maya/include" CACHE PATH "Maya include directory")
set(MAYA_LIB_PATH "/usr/autodesk/maya/lib" CACHE PATH "Maya library directory")


set(UTILITIES_HEADER_FILES
        include/utilities/debugUtils.h
        )

set(PLY_LIB_SOURCE_FILES
        ${VCG_INCLUDE_PATH}/wrap/ply/plylib.cpp
        ${VCG_INCLUDE_PATH}/wrap/ply/plylib.h
        )

set(DECIMATE_HEADER_FILES
        include/other/decimateUtils.h
        )

set(MESH_STATS_HEADER_FILES
        include/other/meshStatsUtils.h
        )

set(TRIDECM_CMD_SOURCE_FILES
        ${PLY_LIB_SOURCE_FILES}
        ${DECIMATE_HEADER_FILES}
        include/other/loadModelUtils.h
        src/standalone/decimatorCmd.cpp
        )

set(MESH_STATS_CMD_SOURCE_FILES
        ${PLY_LIB_SOURCE_FILES}
        ${MESH_STATS_HEADER_FILES}
        include/other/loadModelUtils.h
        src/standalone/meshStatsCmd.cpp
        )
      
set(MAYA_UTILS_HEADER_FILES
        include/other/mayaConvertUtils.h
        )

set(POLY_MODIFIER_HEADER_FILES
        include/polyModifier/polyModifierCmd.h
        include/polyModifier/polyModifierFty.h
        include/polyModifier/polyModifierNode.h
        )

set(POLY_MODIFIER_SOURCE_FILES
        src/polyModifier/polyModifierCmd.cpp
        src/polyModifier/polyModifierFty.cpp
        src/polyModifier/polyModifierNode.cpp
        )

set(MESH_OP_HEADER_FILES
        ${POLY_MODIFIER_HEADER_FILES}
        ${DECIMATE_HEADER_FILES}
        ${MAYA_UTILS_HEADER_FILES}
        ${VCG_NODES_HEADER_FILES}
        include/meshOpCmd/meshOpCmd.h
        include/meshOpCmd/meshOpFty.h
        include/meshOpCmd/meshOpNode.h
        )

set(MESH_OP_SOURCE_FILES
        ${PLY_LIB_SOURCE_FILES}
        ${POLY_MODIFIER_SOURCE_FILES}
        ${MESH_OP_HEADER_FILES}
        src/meshOpCmd/meshOpCmd.cpp
        src/meshOpCmd/meshOpFty.cpp
        src/meshOpCmd/meshOpFtyAction.cpp
        src/meshOpCmd/meshOpMain.cpp
        src/meshOpCmd/meshOpNode.cpp
        )

set(VCG_NODES_HEADER_FILES
        ${POLY_MODIFIER_HEADER_FILES}
        ${DECIMATE_HEADER_FILES}
        ${MAYA_UTILS_HEADER_FILES}
        ${VCG_NODES_HEADER_FILES}
        include/other/readFileUtils.h
        include/vcgNodes/vcgReadFile/vcgReadFileCmd.h
        include/vcgNodes/vcgReadFile/vcgReadFileFty.h
        include/vcgNodes/vcgReadFile/vcgReadFileNode.h
        include/vcgNodes/vcgWriteFile/vcgWriteFileCmd.h
        include/vcgNodes/vcgWriteFile/vcgWriteFileFty.h
        include/vcgNodes/vcgWriteFile/vcgWriteFileNode.h
        include/vcgNodes/vcgMeshCutter/vcgMeshCutterCmd.h
        include/vcgNodes/vcgMeshCutter/vcgMeshCutterFty.h
        include/vcgNodes/vcgMeshCutter/vcgMeshCutterNode.h
        include/vcgNodes/vcgMeshStats/vcgMeshStatsCmd.h
        include/vcgNodes/vcgMeshStats/vcgMeshStatsFty.h
        include/vcgNodes/vcgMeshStats/vcgMeshStatsNode.h
        include/vcgNodes/vcgDecimate/vcgDecimateCmd.h
        include/vcgNodes/vcgDecimate/vcgDecimateFty.h
        include/vcgNodes/vcgDecimate/vcgDecimateNode.h
        )

set(VCG_NODES_SOURCE_FILES
        ${VCG_NODES_HEADER_FILES}
        ${PLY_LIB_SOURCE_FILES}
        ${POLY_MODIFIER_SOURCE_FILES}
        src/vcgNodes/vcgReadFile/vcgReadFileCmd.cpp
        src/vcgNodes/vcgReadFile/vcgReadFileFty.cpp
        src/vcgNodes/vcgReadFile/vcgReadFileFtyAction.cpp
        src/vcgNodes/vcgReadFile/vcgReadFileNode.cpp
        src/vcgNodes/vcgWriteFile/vcgWriteFileCmd.cpp
        src/vcgNodes/vcgWriteFile/vcgWriteFileFty.cpp
        src/vcgNodes/vcgWriteFile/vcgWriteFileFtyAction.cpp
        src/vcgNodes/vcgWriteFile/vcgWriteFileNode.cpp
        src/vcgNodes/vcgMeshCutter/vcgMeshCutterCmd.cpp
        src/vcgNodes/vcgMeshCutter/vcgMeshCutterFty.cpp
        src/vcgNodes/vcgMeshCutter/vcgMeshCutterFtyAction.cpp
        src/vcgNodes/vcgMeshCutter/vcgMeshCutterNode.cpp
        src/vcgNodes/vcgMeshStats/vcgMeshStatsCmd.cpp
        src/vcgNodes/vcgMeshStats/vcgMeshStatsFty.cpp
        src/vcgNodes/vcgMeshStats/vcgMeshStatsFtyAction.cpp
        src/vcgNodes/vcgMeshStats/vcgMeshStatsNode.cpp
        src/vcgNodes/vcgDecimate/vcgDecimateCmd.cpp
        src/vcgNodes/vcgDecimate/vcgDecimateFty.cpp
        src/vcgNodes/vcgDecimate/vcgDecimateFtyAction.cpp
        src/vcgNodes/vcgDecimate/vcgDecimateNode.cpp
        src/vcgNodes/vcgNodesMain.cpp
        )

include_directories(
        include
        ${VCG_INCLUDE_PATH}
)

link_directories(
        ${LIBRARY_OUTPUT_PATH}
)

# Standalone Utilities
if (WANT_STANDALONE)
    # 'Tri-Decimator' standalone
    if (WANT_DECIMATOR)
        add_executable(${TRIDECM_CMD_NAME} ${TRIDECM_CMD_SOURCE_FILES})
    endif()

    # 'Mesh Stats' standalone
    if (WANT_MESH_STATS)
        add_executable(${MESH_STATS_CMD_NAME} ${MESH_STATS_CMD_SOURCE_FILES})
    endif()
endif()

# Add Maya paths and compile targets
if (WANT_MAYA)
    include_directories(
            ${MAYA_INCLUDE_PATH}
    )

    link_directories(
            ${MAYA_LIB_PATH}
    )

    # 'vcgNodes' maya plugin library
    add_library(${VCG_NODES_NAME} SHARED ${VCG_NODES_SOURCE_FILES})
    target_link_libraries(${VCG_NODES_NAME}
            OpenMaya
            Foundation)
    set_target_properties(${VCG_NODES_NAME} PROPERTIES
            PREFIX "" # no 'lib' prefix to .so files
            )
endif()


