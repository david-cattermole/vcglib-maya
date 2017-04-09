/*
 *
 */

#ifndef _vcgMeshStatsFty
#define _vcgMeshStatsFty

#include <polyModifier/polyModifierFty.h>

// General Includes
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrixArray.h>

class MFnMesh;

class vcgMeshStatsFty : public polyModifierFty
{

public:
  vcgMeshStatsFty();

  virtual ~vcgMeshStatsFty();

  void setMesh(MObject &mesh);

  void setCentreOfMass(MVector &value);

  MVector & getCentreOfMass();

  void setMass(float value);

  float getMass();

  MStatus doIt();

private:

  MObject fMesh;
  MVector fCentreOfMass;
  float fMass;
};

#endif
