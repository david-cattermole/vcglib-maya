/*
 *
 */

#ifndef _meshCutterFty
#define _meshCutterFty

#include <polyModifier/polyModifierFty.h>

// General Includes
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrixArray.h>

enum Shapes
{
  kNone = -1,
  kCube = 0,
  kSphere = 1
};

class MFnMesh;

class vcgMeshCutterFty : public polyModifierFty
{

public:
  vcgMeshCutterFty();

  virtual ~vcgMeshCutterFty();

  void setMesh(MObject &mesh);

  void setWorldMatrix(MMatrix &value);

  void setInvert(bool value);

  void setCutterBBoxMin(MFloatPoint &value);

  void setCutterBBoxMax(MFloatPoint &value);

  void setCutterMatrix(MMatrix &value);

  void setCutterShapeType(Shapes value);

  MStatus doIt();

private:

  double distance(MPoint p0, MPoint p1);

  bool isInsideCutterVolume(MPoint       point,
                            MPointArray  &bboxMinArray,
                            MPointArray  &bboxMaxArray,
                            MMatrixArray &bboxMatrixArray,
                            MIntArray    &bboxShapeArray);

  MObject fMesh;
  MMatrix fWorldMatrix;
  bool fInvert;
  MFloatPoint fCutterBBoxMin;
  MFloatPoint fCutterBBoxMax;
  MMatrix fCutterMatrix;
  Shapes fCutterShapeType;
};

#endif
