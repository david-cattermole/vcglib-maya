/*
 *
 */

#ifndef _exampleTemplateCmd
#define _exampleTemplateCmd


#include <polyModifier/polyModifierCmd.h>
#include <vcgNodes/exampleTemplate/exampleTemplateFty.h>

// Function Sets
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
class MArgList;

class exampleTemplate : public polyModifierCmd
{

public:
  ////////////////////
  // Public Methods //
  ////////////////////

  exampleTemplate();

  virtual    ~exampleTemplate();

  static void *creator();

  bool isUndoable() const;

  MStatus doIt(const MArgList &);

  MStatus redoIt();

  MStatus undoIt();

  /////////////////////////////
  // polyModifierCmd Methods //
  /////////////////////////////

  MStatus initModifierNode(MObject modifierNode);

  MStatus directModifier(MObject mesh);

private:

  //////////////////
  // Private Data //
  //////////////////

  // bool m_verbose;

  // exampleTemplate Factory
  exampleTemplateFty fFactory;
};

#endif
