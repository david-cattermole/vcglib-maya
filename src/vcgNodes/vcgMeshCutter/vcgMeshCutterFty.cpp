/*
 *
 */

// Local
#include <vcgNodes/vcgMeshCutter/vcgMeshCutterFty.h>

// Utils
#include <debugUtils.h>

// STL
#include <limits>

vcgMeshCutterFty::vcgMeshCutterFty() :
    fMesh(),
    fWorldMatrix(),
    fInvert(false),
    fCutterBBoxMin(),
    fCutterBBoxMax(),
    fCutterMatrix()
{}

vcgMeshCutterFty::~vcgMeshCutterFty()
{}

// Sets the mesh object for the factory to operate on
void vcgMeshCutterFty::setMesh(MObject &mesh)
{
  fMesh = mesh;
}

void vcgMeshCutterFty::setInvert(bool value)
{
  fInvert = value;
}

void vcgMeshCutterFty::setWorldMatrix(MMatrix &value)
{
  fWorldMatrix = value;
}

void vcgMeshCutterFty::setCutterBBoxMin(MFloatPoint &value)
{
  fCutterBBoxMin = value;
}

void vcgMeshCutterFty::setCutterBBoxMax(MFloatPoint &value)
{
  fCutterBBoxMax = value;
}

void vcgMeshCutterFty::setCutterMatrix(MMatrix &value)
{
  fCutterMatrix = value;
}

void vcgMeshCutterFty::setCutterShapeType(Shapes value)
{
  fCutterShapeType = value;
}
