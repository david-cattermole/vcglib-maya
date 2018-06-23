# VCG Library for command line and Maya
VCG wrapper nodes for Autodesk Maya and standalone command line mesh processing utilities.

This set of nodes and commands for Autodesk Maya are designed to integrate [vcglib](http://vcg.isti.cnr.it/vcglib/) into the Maya workflow and allow batch mesh processing.

## Features

- Standalone command line utilities for decimation and mesh statistics.
- Conversion from Maya mesh data to VCG mesh.
- Conversion from VCG mesh data to Maya mesh.
- Reading of common mesh formats (OBJ, PLY, etc) files as a Maya node (without storing data into the Maya file).
- Writing of common mesh formats (OBJ, PLY, etc) using a Maya node.
- Fine-tuned decimation of mesh data, for optimal quality and speed.
- Calculation of mesh statistics (area, volume, etc).
- Cuts mesh faces based on volumetric objects (not 'booleans'). 

## Usage

Example 1:

Create a polygon sphere and decimate it with VCG's triangle decimation algorithm.
```python
import maya.cmds
maya.cmds.loadPlugin('vcgNodes')
sphere = maya.cmds.polySphere()
node = maya.cmds.vcgDecimate(sphere[0], percent=20)
```

Example 2:

A more elaborate node network which performs the following steps.
1) Read an .OBJ file from disk
2) Compute the mesh statistics
3) Decimate the mesh
4) Write it out to disk

```python
import maya.cmds

# Load the plugin.
maya.cmds.loadPlugin('vcgNodes')

# Create nodes
stubNode = maya.cmds.createNode('polyCube')
readFileNode = maya.cmds.createNode('vcgReadFileNode')
decimateNode = maya.cmds.createNode('vcgDecimateNode')
meshStatsNode = maya.cmds.createNode('vcgMeshStatsNode')
writeFileNode = maya.cmds.createNode('vcgWriteFileNode')
meshNode = maya.cmds.createNode('mesh')
maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

# Connect nodes together
maya.cmds.connectAttr(stubNode + '.output', readFileNode + '.inMesh')
maya.cmds.connectAttr(readFileNode + '.outMesh', meshStatsNode + '.inMesh')
maya.cmds.connectAttr(meshStatsNode + '.outMesh', decimateNode + '.inMesh')
maya.cmds.connectAttr(decimateNode + '.outMesh', meshNode + '.inMesh')
maya.cmds.connectAttr(meshNode + '.outMesh', writeFileNode + '.inMesh')

# Set Input Path
nodeAttr = readFileNode + '.filePath'
inputPath = '/path/to/input_file.obj'
maya.cmds.setAttr(nodeAttr, inputPath, type='string')

# Set Output Path
nodeAttr = writeFileNode + '.filePath'
outputPath = '/path/to/output_file.obj'
maya.cmds.setAttr(nodeAttr, outputPath, type='string')

print 'centre of mass:', maya.cmds.getAttr(meshStatsNode + '.outCentreOfMass')
print 'mass:', maya.cmds.getAttr(meshStatsNode + '.outMass')

# Trigger the write the node
outNodeAttr = writeFileNode + '.outMesh'
maya.cmds.dgeval(outNodeAttr, verbose=True)
```

## Building and Install

### Dependencies

Ensure you have the following installed 

- C++ compiler ([GCC](https://gcc.gnu.org/), Clang, VC++, etc)
  - The compiler must have support for C++11 (or C++0x)
- [CMake 2.6+](https://cmake.org/)
- [vcglib 1.0.1](https://github.com/cnr-isti-vclab/vcglib/releases/tag/v1.0.1)
- [Autodesk Maya 2016+](https://www.autodesk.com.au/products/maya/overview)

### Build & Install

Run the following commands in a Bash-like shell.
Note the following build instructions have been tested on a CentOS 6.x installation.

#### Download vcglib

Download the stable version of vcglib from github and untar it into the current working directory.

```commandline
$ wget https://github.com/cnr-isti-vclab/vcglib/archive/v1.0.1.tar.gz
$ tar -xf v1.0.1.tar.gz
```
If everything worked, you'll have a new directory in your current working directory named 'vcglib-1.0.1', this is the root of the VCG library headers. Everything is related to VCG is contained in this directory.

#### Compile VCG Maya Plugin

Now we have our dependencies, we can now compile the Maya plugin.
To build with Maya support make sure to set WANT_MAYA=1. You may also turn off building the standalone utilities decimator and meshStats by setting WANT_STANDALONE=0, it is enabled by default.  

```commandline
$ cd <project root>
$ mkdir build
$ cd build
$ cmake \
  -DWANT_MAYA=1 \
  -DVCG_INCLUDE_PATH=/path/to/vcglib \
  -DMAYA_INCLUDE_PATH=/path/to/maya/include \
  -DMAYA_LIB_PATH=/path/to/maya/lib ..
$ make -j 4
```

Do not forget to add the '..' at the end of the cmake command.
For debug you may add the flag '-DCMAKE_BUILD_TYPE=Debug' to cmake.

#### Install VCG Maya Plugin

Now lets install into our home directory maya 'plug-ins' directory.

```commandline
$ mkdir ~/maya/<maya version>/plug-ins
$ cd <project root>/build
$ cp vcgNodes.so ~/maya/<maya version>/plug-ins
```

#### Testing VCG

To run the test suite that comes with Maya VCG Nodes, cd into the ./test directory and run test.sh. By default only the standalone utilities are tested. To test the Maya nodes edit test.sh.

```commandline
$ cd <project root>/test
$ ./test.sh
```

## Limitations and Known Bugs 

- Maya Read / Write support is buggy and should not be used in production.
- Maya / VCG mesh conversion only supports vertex position and triangular faces.
  - UVs are not supported
  - Colour Sets are not supported.
