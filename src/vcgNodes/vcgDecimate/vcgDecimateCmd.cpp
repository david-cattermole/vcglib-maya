/*
 *
 */

#include <vcgNodes/vcgDecimate/vcgDecimateCmd.h>
#include <vcgNodes/vcgDecimate/vcgDecimateNode.h>

// Utils
#include <utilities/debugUtils.h>

// Function Sets
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
//#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

// Iterators
#include <maya/MItSelectionList.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>

// General Maya
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MArgParser.h>
#include <maya/MArgDatabase.h>
#include <maya/MStatus.h>
#include <maya/MSyntax.h>



// Status Checking Macro - MCheckStatus (Debugging tool)
//
#define MCheckStatus(status, message)  \
  if( MS::kSuccess != status ) {    \
    cerr << message << "\n";    \
    return status;          \
  }


vcgDecimate::vcgDecimate()
//
//  Description:
//    vcgDecimate constructor
//
{
  m_percent = 100.0;
  m_maxTime = 3600.0;
}

vcgDecimate::~vcgDecimate()
//
//  Description:
//    vcgDecimate destructor
//
{}

void *vcgDecimate::creator()
//
//  Description:
//    this method exists to give Maya a way to create new objects
//      of this type. 
//
//  Return Value:
//    a new object of this type
//
{
  return new vcgDecimate();
}

bool vcgDecimate::isUndoable() const
//
//  Description:
//    this method tells Maya this command is undoable.  It is added to the 
//    undo queue if it is.
//
//  Return Value:
//    true if this command is undoable.
//
{
  return true;
}


MStatus vcgDecimate::doIt(const MArgList &args)
//
//  Description:
//    implements the MEL vcgDecimate command.
//
//  Arguments:
//    argList - the argument list that was passes to the command from MEL
//
//  Return Value:
//    MS::kSuccess - command succeeded
//    MS::kFailure - command failed (returning this value will cause the 
//                     MEL script that is being run to terminate unless the
//                     error is caught using a "catch" statement.
//
{
  MStatus status;
  bool badArgument = false;

  MSyntax syntax;
  syntax.addFlag("-v",    "-verbose", MSyntax::kNoArg);
  syntax.addFlag("-h",    "-help", MSyntax::kNoArg);
  syntax.addFlag("-p",    "-percent", MSyntax::kDouble);
  syntax.addFlag("-mtm",  "-maxTime", MSyntax::kDouble);
  // syntax.addFlag("-merr", "-maxError", MSyntax::kDouble);
  syntax.setObjectType(MSyntax::MObjectFormat::kSelectionList, 1, 1);
  syntax.useSelectionAsDefault(true);
  syntax.enableQuery( false );
  syntax.enableEdit( false );

  MArgDatabase argData(syntax, args, &status);

  // Help
  if (argData.isFlagSet("help"))
  {
    MGlobal::displayInfo("print help text");
    return MS::kSuccess;
  }

  // Verbose
  if (argData.isFlagSet("verbose"))
  {
    status = argData.getFlagArgument("verbose", 0, m_verbose);
  }

  // Percent
  if (argData.isFlagSet("percent"))
  {
    status = argData.getFlagArgument("percent", 0, m_percent);
  }

  // Max Time
  if (argData.isFlagSet("maxTime"))
  {
    status = argData.getFlagArgument("maxTime", 0, m_maxTime);
  }

  // Parse the selection list for selected components of the right type.
  // To simplify things, we only take the first object that we find with
  // selected components and operate on that object alone.
  //
  // All other objects are ignored and return warning messages indicating
  // this limitation.

  MSelectionList selList;
  argData.getObjects(selList);
  if (selList.length() == 0)
  {
    MGlobal::getActiveSelectionList(selList);
  }
  MItSelectionList selListIter(selList);
  selListIter.setFilter(MFn::kMesh);

  // The meshOperation node only accepts a component list input, so we build
  // a component list using MFnComponentListData.
  //
  // MIntArrays could also be passed into the node to represent the ids,
  // but are less storage efficient than component lists, since consecutive
  // components are bundled into a single entry in component lists.
  //

  bool found = false;
  bool foundMultiple = false;

  for (; !selListIter.isDone(); selListIter.next())
  {
    MDagPath dagPath;

    selListIter.getDagPath(dagPath);
    // DBG("Dag full path name: " << dagPath.fullPathName());
    dagPath.extendToShape();
//    DBG("Dag shape full path name: " << dagPath.fullPathName());

    // Check for selected components of the right type
    //
    if (dagPath.apiType() == MFn::kMesh)
    {
      if (!found)
      {
        // Ensure that this DAG path will point to the shape
        // of our object. Set the DAG path for the polyModifierCmd.
        dagPath.extendToShape();
        setMeshNode(dagPath);
        found = true;
      }
      else
      {
        // Break once we have found a multiple object holding
        // selected components, since we are not interested in how
        // many multiple objects there are, only the fact that there
        // are multiple objects.
        foundMultiple = true;
        break;
      }
    }
  }
  if (foundMultiple)
  {
    displayWarning("Found more than one object with selected components.");
    displayWarning("Only operating on first found object.");
  }
//  DBG("found: " << found);
//  DBG("foundMultiple: " << foundMultiple);

  // Initialize the polyModifierCmd node type - mesh node already set
  setModifierNodeType(vcgDecimateNode::id);

  // Allows to set the name of the node, yet to be created.
  // setModifierNodeName("someNameHere");

  if (found)
  {
    // Now, pass control over to the polyModifierCmd::doModifyPoly() method
    // to handle the operation.
//    DBG("doModifyPoly()...");
    status = doModifyPoly();

    if (status == MS::kSuccess)
    {
//      DBG("vcgDecimate command succeeded!");
//      DBG("vcgDecimate node: " << getModifierNodeName());
      setResult("vcgDecimate command succeeded!");
//      setResult("vcgDecimateNode1");
      // appendToResult("vcgDecimateNode1");
    }
    else
    {
//      DBG("vcgDecimate command failed!");
      displayError("vcgDecimate command failed!");
    }
  }
  else
  {
//    DBG("vcgDecimate command failed: Unable to find selected components");
    displayError("vcgDecimate command failed: Unable to find selected components");
    status = MS::kFailure;
  }

//  DBG("end...");
  return status;
}

MStatus vcgDecimate::redoIt()
//
//  Description:
//    Implements redo for the MEL vcgDecimate command.
//
//    This method is called when the user has undone a command of this type
//    and then redoes it.  No arguments are passed in as all of the necessary
//    information is cached by the doIt method.
//
//  Return Value:
//    MS::kSuccess - command succeeded
//    MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
//
{
  MStatus status;

  // Process the polyModifierCmd
  //
  status = redoModifyPoly();

  if (status == MS::kSuccess)
  {
    setResult("vcgDecimate command succeeded!");
  }
  else
  {
    displayError("vcgDecimate command failed!");
  }

  return status;
}

MStatus vcgDecimate::undoIt()
//
//  Description:
//    implements undo for the MEL vcgDecimate command.
//
//    This method is called to undo a previous command of this type.  The 
//    system should be returned to the exact state that it was it previous 
//    to this command being executed.  That includes the selection state.
//
//  Return Value:
//    MS::kSuccess - command succeeded
//    MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
//
{
  MStatus status;

  status = undoModifyPoly();

  if (status == MS::kSuccess)
  {
    setResult("vcgDecimate undo succeeded!");
  }
  else
  {
    setResult("vcgDecimate undo failed!");
  }

  return status;
}

MStatus vcgDecimate::initModifierNode(MObject modifierNode)
{
  MStatus status;
  MFnDependencyNode depNodeFn(modifierNode);

  MObject percentAttr = depNodeFn.attribute("percent");
  MObject maxTimeAttr = depNodeFn.attribute("maxTime");

  MPlug parentPlug(modifierNode, percentAttr);
  status = parentPlug.setValue(m_percent);

  MPlug maxTimePlug(modifierNode, maxTimeAttr);
  status = maxTimePlug.setValue(m_maxTime);


  return MS::kSuccess;
}

MStatus vcgDecimate::directModifier(MObject mesh)
{
  MStatus status;

  fFactory.setMesh(mesh);

  // Now, perform the vcgDecimate
  status = fFactory.doIt();

  return status;
}

