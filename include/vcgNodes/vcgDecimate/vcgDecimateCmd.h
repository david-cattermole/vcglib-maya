/*
 *
 */

#ifndef _vcgDecimateCmd
#define _vcgDecimateCmd


#include <polyModifier/polyModifierCmd.h>
#include <vcgNodes/vcgDecimate/vcgDecimateFty.h>

// Function Sets
//
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
//
class MArgList;

class vcgDecimate : public polyModifierCmd
{

public:
  ////////////////////
  // Public Methods //
  ////////////////////

  vcgDecimate();

  virtual    ~vcgDecimate();

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

  // // Creates the syntax needed forthe command
  // MSyntax createSyntax();

  //////////////////
  // Private Data //
  //////////////////

  double m_percent;
  double m_maxTime;
  bool m_useQualityThreshold;
  double m_qualityThreshold;
  bool m_useNormalThreshold;
  double m_normalThreshold;
  bool m_preserveBoundary;
  double m_boundaryWeight;
  bool m_verbose;

  // vcgDecimate Factory
  vcgDecimateFty fFactory;
};

#endif
