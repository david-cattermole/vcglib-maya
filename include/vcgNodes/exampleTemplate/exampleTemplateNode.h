/*
 *
 */

#ifndef _exampleTemplateNode
#define _exampleTemplateNode

#include <vcgNodes/exampleTemplate/exampleTemplateFty.h>
#include <polyModifier/polyModifierNode.h>

// Maya
#include <maya/MTypeId.h>

class exampleTemplateNode : public polyModifierNode
{
public:
  exampleTemplateNode();

  virtual ~exampleTemplateNode();

  virtual MStatus compute(const MPlug &plug, MDataBlock &data);

  static void *creator();

  static MStatus initialize();

public:

  // Base attributes needed for mesh data transport.
  static MObject inMesh;
  static MObject outMesh;

  // attributes
  static MObject aEnable;

  static MTypeId id;

  exampleTemplateFty fFactory;
};

#endif
