#!/usr/bin/env bash

ROOT=`pwd`;


# Mesh Stats
../cmake-build-debug/meshStats ../data/gargoyle_align_base.obj ${ROOT}/gargoyle_stats_out.obj;
../cmake-build-debug/meshStats ../data/gargoyle_align_p1.obj ${ROOT}/gargoyle_stats_out.obj;
../cmake-build-debug/meshStats ../data/gargoyle_align_p2.obj ${ROOT}/gargoyle_stats_out.obj;
 ../cmake-build-debug/meshStats ../data/gargoyle_align_p3.obj ${ROOT}/gargoyle_stats_out.obj;
 ../cmake-build-debug/meshStats ../data/gargoyle_align_p4.obj ${ROOT}/gargoyle_stats_out.obj;
 ../cmake-build-debug/meshStats ../data/fertility_texCoords.obj ${ROOT}/fertility_stats_out.obj;
 ../cmake-build-debug/meshStats ../data/sphere.obj ${ROOT}/sphere_stats_out.obj;
 ../cmake-build-debug/meshStats ../data/trisphere.obj ${ROOT}/sphere_stats_out.obj;


## Very fast tests
#../cmake-build-debug/decimator ../data/quad.ply ${ROOT}/test_quad_100.obj 100;
#../cmake-build-debug/decimator ../data/quad.ply ${ROOT}/test_quad_050.obj 50;


# Maya Tests
 mayapy runVcgNodesTests.py


## Fast tests
#../cmake-build-debug/decimator ../data/fertility.ply ${ROOT}/test_fertility_100.obj 100 --verbose;
#../cmake-build-debug/decimator ../data/fertility_texCoords.obj ${ROOT}/test_fertility_100_uvs.obj 100  --verbose;
#../cmake-build-debug/decimator ../data/fertility.ply ${ROOT}/test_fertility_010.obj 10 --verbose;
#../cmake-build-debug/decimator ../data/fertility_texCoords.obj ${ROOT}/test_fertility_010_uvs.obj 10 --verbose;


## Alignment tests
#../cmake-build-debug/autoAlign ../data/gargoyle_align_base.obj ../data/gargoyle_align_p1.obj  ${ROOT}/gargoyle_align_p1_out.obj;
#../cmake-build-debug/autoAlign ../data/gargoyle_align_base.obj ../data/gargoyle_align_p2.obj  ${ROOT}/gargoyle_align_p2_out.obj;
#../cmake-build-debug/autoAlign ../data/gargoyle_align_base.obj ../data/gargoyle_align_p3.obj  ${ROOT}/gargoyle_align_p3_out.obj;
#../cmake-build-debug/autoAlign ../data/gargoyle_align_base.obj ../data/gargoyle_align_p4.obj  ${ROOT}/gargoyle_align_p4_out.obj;


## Medium tests
#../cmake-build-debug/decimator ../data/ganesha.ply ${ROOT}/test_ganesha_100.obj 100;
#../cmake-build-debug/decimator ../data/ganesha.ply ${ROOT}/test_ganesha_010.obj 10;


## Slow tests
#../cmake-build-debug/decimator ../data/nano_out_pc100.obj ${ROOT}/test_slow_001.obj 1  --verbose;


## Very Slow Tests
#../cmake-build-debug/decimator ../data/fxphd_all5Mil_fromMayaUVs.obj ${ROOT}/test_verySlow_001.obj 1 --verbose;
