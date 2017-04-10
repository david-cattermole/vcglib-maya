/*
 *
 */

// Local
#include <vcgNodes/vcgMeshStats/vcgMeshStatsFty.h>

// Utils
#include <utilities/debugUtils.h>

// STL
#include <limits>

vcgMeshStatsFty::vcgMeshStatsFty() :
    fMesh(),
    fCentreOfMass(),
    fMass(0.0)
{}

vcgMeshStatsFty::~vcgMeshStatsFty()
{}

// Sets the mesh object for the factory to operate on
void vcgMeshStatsFty::setMesh(MObject &mesh)
{
  fMesh = mesh;
}

// Sets the centre of mass
void vcgMeshStatsFty::setCentreOfMass(MVector &value)
{
  fCentreOfMass = value;
}

// Sets mass
void vcgMeshStatsFty::setMass(float value)
{
  fMass = value;
}

// Sets the centre of mass
MVector & vcgMeshStatsFty::getCentreOfMass()
{
  return fCentreOfMass;
}

// Sets mass
float vcgMeshStatsFty::getMass()
{
  return fMass;
}
