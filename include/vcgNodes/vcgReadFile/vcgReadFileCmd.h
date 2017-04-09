/*
 *
 */

#ifndef _vcgReadFileCmd
#define _vcgReadFileCmd


#include <polyModifier/polyModifierCmd.h>
#include <vcgNodes/vcgReadFile/vcgReadFileFty.h>

// Function Sets
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
class MArgList;

class vcgReadFile : public polyModifierCmd
{

public:
  ////////////////////
  // Public Methods //
  ////////////////////

  vcgReadFile();

  virtual    ~vcgReadFile();

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

  // vcgReadFile Factory
  vcgReadFileFty fFactory;
};

#endif
