#!/usr/bin/env bash

ROOT=`pwd`;
OUTPUT=${ROOT}/output;
BUILD_DIR=${ROOT}/../cmake-build-debug;
DATA_DIR=${ROOT}/../data;


# Mesh Stats
${BUILD_DIR}/meshStats ${DATA_DIR}/gargoyle_align_base.obj ${OUTPUT}/gargoyle_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/gargoyle_align_p1.obj ${OUTPUT}/gargoyle_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/gargoyle_align_p2.obj ${OUTPUT}/gargoyle_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/gargoyle_align_p3.obj ${OUTPUT}/gargoyle_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/gargoyle_align_p4.obj ${OUTPUT}/gargoyle_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/fertility_texCoords.obj ${OUTPUT}/fertility_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/sphere.obj ${OUTPUT}/sphere_stats_out.obj;
${BUILD_DIR}/meshStats ${DATA_DIR}/trisphere.obj ${OUTPUT}/sphere_stats_out.obj;


# Very fast tests
${BUILD_DIR}/decimator ${DATA_DIR}/quad.ply ${OUTPUT}/test_quad_100.obj 100;
${BUILD_DIR}/decimator ${DATA_DIR}/quad.ply ${OUTPUT}/test_quad_050.obj 50;


# Fast tests
${BUILD_DIR}/decimator ${DATA_DIR}/fertility.ply ${OUTPUT}/test_fertility_100.obj 100 --verbose;
${BUILD_DIR}/decimator ${DATA_DIR}/fertility_texCoords.obj ${OUTPUT}/test_fertility_100_uvs.obj 100  --verbose;
${BUILD_DIR}/decimator ${DATA_DIR}/fertility.ply ${OUTPUT}/test_fertility_010.obj 10 --verbose;
${BUILD_DIR}/decimator ${DATA_DIR}/fertility_texCoords.obj ${OUTPUT}/test_fertility_010_uvs.obj 10 --verbose;


# # Maya Tests
# env MAYA_PLUG_IN_PATH=${MAYA_PLUG_IN_PATH}:${BUILD_DIR}/  mayapy ${ROOT}/runVcgNodesTests.py


# # Alignment tests
# ${BUILD_DIR}/autoAlign ${DATA_DIR}/gargoyle_align_base.obj ${DATA_DIR}/gargoyle_align_p1.obj  ${OUTPUT}/gargoyle_align_p1_out.obj;
# ${BUILD_DIR}/autoAlign ${DATA_DIR}/gargoyle_align_base.obj ${DATA_DIR}/gargoyle_align_p2.obj  ${OUTPUT}/gargoyle_align_p2_out.obj;
# ${BUILD_DIR}/autoAlign ${DATA_DIR}/gargoyle_align_base.obj ${DATA_DIR}/gargoyle_align_p3.obj  ${OUTPUT}/gargoyle_align_p3_out.obj;
# ${BUILD_DIR}/autoAlign ${DATA_DIR}/gargoyle_align_base.obj ${DATA_DIR}/gargoyle_align_p4.obj  ${OUTPUT}/gargoyle_align_p4_out.obj;


# Medium tests
${BUILD_DIR}/decimator ${DATA_DIR}/ganesha.ply ${OUTPUT}/test_ganesha_100.obj 100;
${BUILD_DIR}/decimator ${DATA_DIR}/ganesha.ply ${OUTPUT}/test_ganesha_010.obj 10;


# Slow tests
${BUILD_DIR}/decimator ${DATA_DIR}/nano_out_pc100.obj ${OUTPUT}/test_slow_001.obj 1  --verbose;
