/*
 *
 */

#ifndef _exampleTemplateFty
#define _exampleTemplateFty

#include <polyModifier/polyModifierFty.h>

// General Includes
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MMatrixArray.h>

class MFnMesh;

class exampleTemplateFty : public polyModifierFty
{

public:
  exampleTemplateFty();

  virtual ~exampleTemplateFty();

  void setMesh(MObject &mesh);
  
  MStatus doIt();

private:

  MObject fMesh;
};

#endif
