/*
 *
 */

#ifndef _vcgDecimateNode
#define _vcgDecimateNode

#include <vcgNodes/vcgDecimate/vcgDecimateFty.h>
#include <polyModifier/polyModifierNode.h>

// Maya
#include <maya/MTypeId.h>

class vcgDecimateNode : public polyModifierNode
{
public:
  vcgDecimateNode();

  virtual ~vcgDecimateNode();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static void *creator();

  static MStatus initialize();

public:

  // Base attributes that must exist for all nodes.
  static MObject inMesh;
  static MObject outMesh;

  // attributes
  static MObject aPercent;
  static MObject aMaxTime;
//  static MObject aMaxError;
//  static MObject aMinError;
  static MObject aUseQualityThreshold;
  static MObject aQualityThreshold;
  static MObject aUseNormalThreshold;
  static MObject aNormalThreshold;
  static MObject aBoundaryWeight;
  static MObject aPreserveBoundary;
  static MObject aVerbose;


  static MTypeId id;

  vcgDecimateFty fFactory;
};

#endif
