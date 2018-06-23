/*
 *
 */

#ifndef _vcgWriteFileCmd
#define _vcgWriteFileCmd


#include <polyModifier/polyModifierCmd.h>
#include <vcgNodes/vcgWriteFile/vcgWriteFileFty.h>

// Function Sets
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
class MArgList;

class vcgWriteFile : public polyModifierCmd
{

public:
  ////////////////////
  // Public Methods //
  ////////////////////

  vcgWriteFile();

  virtual    ~vcgWriteFile();

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

  // vcgWriteFile Factory
  vcgWriteFileFty fFactory;
};

#endif
