/*
 *
 */

#ifndef _meshCutterCmd
#define _meshCutterCmd


#include <polyModifier/polyModifierCmd.h>
#include <vcgNodes/vcgMeshCutter/vcgMeshCutterFty.h>

// Function Sets
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
class MArgList;

class vcgMeshCutter : public polyModifierCmd
{

public:
  ////////////////////
  // Public Methods //
  ////////////////////

  vcgMeshCutter();

  virtual    ~vcgMeshCutter();

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

  // // Creates the syntax needed for the command
  // MSyntax createSyntax();

  //////////////////
  // Private Data //
  //////////////////

//  bool m_verbose;

  // vcgMeshCutter Factory
  vcgMeshCutterFty fFactory;
};

#endif
