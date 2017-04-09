/*
 *
 */

#ifndef _vcgReadFileFty
#define _vcgReadFileFty

#include <polyModifier/polyModifierFty.h>

// General Includes
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrixArray.h>

class MFnMesh;

class vcgReadFileFty : public polyModifierFty
{

public:
  vcgReadFileFty();

  virtual ~vcgReadFileFty();

  void setMesh(MObject &mesh);

  void setFilePath(MString &value);

  MStatus doIt();

private:

  MObject fMesh;
  MString fFilePath;
};

#endif
