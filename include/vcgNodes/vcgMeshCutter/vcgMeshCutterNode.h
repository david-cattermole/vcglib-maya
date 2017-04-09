/*
 *
 */

#ifndef _meshCutterNode
#define _meshCutterNode

#include <vcgNodes/vcgMeshCutter/vcgMeshCutterFty.h>
#include <polyModifier/polyModifierNode.h>

// Maya
#include <maya/MTypeId.h>

class vcgMeshCutterNode : public polyModifierNode
{
public:
  vcgMeshCutterNode();

  virtual ~vcgMeshCutterNode();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static void *creator();

  static MStatus initialize();

public:

  // Base attributes needed for mesh data transport.
  static MObject inMesh;
  static MObject outMesh;

  // attributes
  static MObject aEnable;
  static MObject aInvert;
  static MObject aWorldMatrix;

//  static MObject aCutters;
  static MObject aCutterBBoxMin;
  static MObject aCutterBBoxMax;
  static MObject aCutterMatrix;
  static MObject aCutterShapeType;

  static MTypeId id;

  vcgMeshCutterFty fFactory;
};

#endif
