/*
 *
 */

#ifndef _vcgMeshStatsNode
#define _vcgMeshStatsNode

#include <vcgNodes/vcgMeshStats/vcgMeshStatsFty.h>
#include <polyModifier/polyModifierNode.h>

// Maya
#include <maya/MTypeId.h>

class vcgMeshStatsNode : public polyModifierNode
{
public:
  vcgMeshStatsNode();

  virtual ~vcgMeshStatsNode();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static void *creator();

  static MStatus initialize();

public:

  // Base attributes needed for mesh data transport.
  static MObject inMesh;
  static MObject outMesh;

  // attributes
  static MObject aEnable;
  static MObject aOutCentreOfMass;
  static MObject aOutMass;

  static MTypeId id;

  vcgMeshStatsFty fFactory;
};

#endif
