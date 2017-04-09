/*
 *
 */

#ifndef _vcgMeshStatsCmd
#define _vcgMeshStatsCmd


#include <polyModifier/polyModifierCmd.h>
#include <vcgNodes/vcgMeshStats/vcgMeshStatsFty.h>

// Function Sets
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
class MArgList;

class vcgMeshStats : public polyModifierCmd
{

public:
  ////////////////////
  // Public Methods //
  ////////////////////

  vcgMeshStats();

  virtual    ~vcgMeshStats();

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

  // vcgMeshStats Factory
  vcgMeshStatsFty fFactory;
};

#endif
