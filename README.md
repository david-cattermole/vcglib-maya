# Maya VCG Library
VCG wrapper nodes for Autodesk Maya

This set of nodes and commands for Autodesk Maya are designed to integrate vcglib (http://vcg.isti.cnr.it/vcglib/) into the Maya workflow.

## Features
- Conversion from Maya mesh data to VCG mesh.
- Conversion from VCG mesh data to Maya mesh.
- Reading of common mesh formats (OBJ, PLY, etc) files as a Maya node (without storing data into the Maya file).
- Writing of common mesh formats (OBJ, PLY, etc) using a Maya node.
- Decimation of mesh data.
- Calculation of mesh statistics (area, volume, etc).
- Cuts mesh faces based on cutting objects (not booleans).

## Usage

_To be written._

## Building and Install

_To be written._

### Dependencies

- C++ compiler with support for C++11
- CMake 2.6+
- vcglib (Visualisation and Computer Graphics library)
- Autodesk Maya 2016+

### Build

_To be written._

### Install

_To be written._

## Limitations and Known Bugs 

- Maya / VCG mesh conversion only supports vertex position, triangle faces.
  - UVs are not supported
  - Colour Sets are not supported 