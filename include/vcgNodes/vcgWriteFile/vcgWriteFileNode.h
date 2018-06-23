/*
 *
 */

#ifndef _vcgWriteFileNode
#define _vcgWriteFileNode

#include <vcgNodes/vcgWriteFile/vcgWriteFileFty.h>
#include <polyModifier/polyModifierNode.h>

// Maya
#include <maya/MTypeId.h>

class vcgWriteFileNode : public polyModifierNode
{
public:
  vcgWriteFileNode();

  virtual ~vcgWriteFileNode();

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
  static MObject aCleanMesh;
  static MObject aVerbose;

  static MTypeId id;

  vcgWriteFileFty fFactory;
};

#endif
