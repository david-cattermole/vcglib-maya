cmake_minimum_required(VERSION 2.8)
project(vcgNodes)

# constant names
set(TRIDECM_CMD_NAME decimator)
set(AUTO_ALIGN_CMD_NAME autoAlign)
set(MESH_STATS_CMD_NAME meshStats)
set(MESH_OP_NAME meshOp)
set(VCG_NODES_NAME vcgNodes)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)
# set(CMAKE_BUILD_TYPE RELEASE)

# Maya SDK
set(MAYA_ROOT "/usr/autodesk/maya2016")
set(MAYA_INCLUDE ${MAYA_ROOT}/include)
set(MAYA_LIB ${MAYA_ROOT}/lib)

# VCG
set(VCG_ROOT "/home/davidc/dev/meshlab/vcglib")
set(VCG_INCLUDE ${VCG_ROOT})

# Utilities
set(UTILS_INCLUDE "/home/davidc/dev/cppTests/utilities")

set(PLY_LIB_SOURCE_FILES
        ${VCG_ROOT}/wrap/ply/plylib.cpp
        ${VCG_ROOT}/wrap/ply/plylib.h
        )

set(DECIMATE_HEADER_FILES
        include/other/decimateUtils.h
        )

set(AUTO_ALIGN_HEADER_FILES
        include/other/autoAlignUtils.h
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

set(AUTO_ALIGN_CMD_SOURCE_FILES
        ${PLY_LIB_SOURCE_FILES}
        ${AUTO_ALIGN_HEADER_FILES}
        include/other/loadModelUtils.h
        src/standalone/autoAlignCmd.cpp
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
        ${UTILS_INCLUDE}
        ${VCG_INCLUDE}
        ${MAYA_INCLUDE}
)

link_directories(
        ${LIBRARY_OUTPUT_PATH}
        ${MAYA_LIB}
)

# 'Tri-Decimator' standalone
add_executable(${TRIDECM_CMD_NAME} ${TRIDECM_CMD_SOURCE_FILES})

# 'Auto-Align' standalone
add_executable(${AUTO_ALIGN_CMD_NAME} ${AUTO_ALIGN_CMD_SOURCE_FILES})

# 'Mesh Stats' standalone
add_executable(${MESH_STATS_CMD_NAME} ${MESH_STATS_CMD_SOURCE_FILES})

## 'meshOp' maya plugin library
#add_library(${MESH_OP_NAME} SHARED ${MESH_OP_SOURCE_FILES})
#target_link_libraries(${MESH_OP_NAME}
#        OpenMaya
#        Foundation)
#set_target_properties(${MESH_OP_NAME} PROPERTIES
#        PREFIX "" # no 'lib' prefix to .so files
#        )

# 'vcgNodes' maya plugin library
add_library(${VCG_NODES_NAME} SHARED ${VCG_NODES_SOURCE_FILES})
target_link_libraries(${VCG_NODES_NAME}
        OpenMaya
        Foundation)
set_target_properties(${VCG_NODES_NAME} PROPERTIES
        PREFIX "" # no 'lib' prefix to .so files
        )

