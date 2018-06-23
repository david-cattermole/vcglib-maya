/*
 *
 */

#ifndef _vcgWriteFileFty
#define _vcgWriteFileFty

#include <polyModifier/polyModifierFty.h>

// General Includes
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrixArray.h>

class MFnMesh;

class vcgWriteFileFty : public polyModifierFty
{

public:
  vcgWriteFileFty();

  virtual ~vcgWriteFileFty();

  void setMesh(MObject &mesh);

  void setFilePath(MString &value);

  void setCleanMesh(bool value);

  void setVerbose(bool value);

  MStatus doIt();

private:


  MObject fMesh;
  MString fFilePath;
  bool    fVerbose;
  bool    fCleanMesh;
};

#endif
