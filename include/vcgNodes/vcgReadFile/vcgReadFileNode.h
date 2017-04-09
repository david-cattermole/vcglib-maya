/*
 *
 */

#ifndef _vcgReadFileNode
#define _vcgReadFileNode

#include <vcgNodes/vcgReadFile/vcgReadFileFty.h>
#include <polyModifier/polyModifierNode.h>

// Maya
#include <maya/MTypeId.h>

class vcgReadFileNode : public polyModifierNode
{
public:
  vcgReadFileNode();

  virtual ~vcgReadFileNode();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static void *creator();

  static MStatus initialize();

public:

  // Base attributes needed for mesh data transport.
  static MObject inMesh;
  static MObject outMesh;

  // attributes
  static MObject aEnable;
  static MObject aFilePath;

  static MTypeId id;

  vcgReadFileFty fFactory;
};

#endif
