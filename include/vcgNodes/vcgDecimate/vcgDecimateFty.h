/*
 *
 */

#ifndef _vcgDecimateFty
#define _vcgDecimateFty

#include <polyModifier/polyModifierFty.h>

// General Includes
//
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MString.h>

class MFnMesh;

class vcgDecimateFty : public polyModifierFty
{

public:
  vcgDecimateFty();

  virtual ~vcgDecimateFty();

  void setMesh(MObject &mesh);

  void setPercent(double value);

  void setTargetError(double value);

  void setMaxTime(double value);

  void setUseQualityThreshold(bool value);

  void setQualityThreshold(double value);

  void setUseNormalThreshold(bool value);

  void setNormalThreshold(double value);

  void setBoundaryWeight(double value);

  void setPreserveBoundary(bool value);

  void setVerbose(bool value);

  MObject &getMesh();

  MStatus doIt();

private:
  MObject fMesh;
  double fPercent;
  bool fUseQualityThreshold;
  double fQualityThreshold;
  bool fUseNormalThreshold;
  double fNormalThreshold;
  double fBoundaryWeight;
  bool fPreserveBoundary;
  double fTargetError;
  double fMaxTime;
  bool fVerbose;

};

#endif
