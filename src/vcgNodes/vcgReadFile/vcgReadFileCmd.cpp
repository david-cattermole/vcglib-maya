/*
 *
 */

#include <vcgNodes/vcgReadFile/vcgReadFileCmd.h>
#include <vcgNodes/vcgReadFile/vcgReadFileNode.h>

// Utils
#include <debugUtils.h>

// Function Sets
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
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
#define MCheckStatus(status, message)  \
  if( MS::kSuccess != status ) {    \
    cerr << message << "\n";    \
    return status;          \
  }


vcgReadFile::vcgReadFile()
//
//  Description:
//    vcgReadFile constructor
//
{
//  m_percent = 100.0;
//  m_maxTime = 3600.0;
}

vcgReadFile::~vcgReadFile()
//
//  Description:
//    vcgReadFile destructor
//
{}

void *vcgReadFile::creator()
//
//  Description:
//    this method exists to give Maya a way to create new objects
//      of this type. 
//
//  Return Value:
//    a new object of this type
//
{
  return new vcgReadFile();
}

bool vcgReadFile::isUndoable() const
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


MStatus vcgReadFile::doIt(const MArgList &args)
//
//  Description:
//    implements the MEL vcgReadFile command.
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
  syntax.addFlag("-h", "-help", MSyntax::kNoArg);
  syntax.setObjectType(MSyntax::MObjectFormat::kSelectionList, 1, 1);
  syntax.useSelectionAsDefault(true);
  syntax.enableQuery(false);
  syntax.enableEdit(false);

  MArgDatabase argData(syntax, args, &status);

  // Help
  if (argData.isFlagSet("help"))
  {
    MGlobal::displayInfo("print help text");
    return MS::kSuccess;
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
    dagPath.extendToShape();

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

  // Initialize the polyModifierCmd node type - mesh node already set
  setModifierNodeType(vcgReadFileNode::id);

  // Allows to set the name of the node, yet to be created.
  // setModifierNodeName("someNameHere");

  if (found)
  {
    // Now, pass control over to the polyModifierCmd::doModifyPoly() method
    // to handle the operation.
    status = doModifyPoly();

    if (status == MS::kSuccess)
    {
      setResult("vcgReadFile command succeeded!");
    }
    else
    {
      displayError("vcgReadFile command failed!");
    }
  }
  else
  {
    displayError("vcgReadFile command failed: Unable to find selected components");
    status = MS::kFailure;
  }

  return status;
}

MStatus vcgReadFile::redoIt()
//
//  Description:
//    Implements redo for the MEL vcgReadFile command.
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
    setResult("vcgReadFile command succeeded!");
  }
  else
  {
    displayError("vcgReadFile command failed!");
  }

  return status;
}

MStatus vcgReadFile::undoIt()
//
//  Description:
//    implements undo for the MEL vcgReadFile command.
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
    setResult("vcgReadFile undo succeeded!");
  }
  else
  {
    setResult("vcgReadFile undo failed!");
  }

  return status;
}

MStatus vcgReadFile::initModifierNode(MObject modifierNode)
{
  MStatus status;
  MFnDependencyNode depNodeFn(modifierNode);

  return MS::kSuccess;
}

MStatus vcgReadFile::directModifier(MObject mesh)
{
  MStatus status;

  fFactory.setMesh(mesh);

  // Now, perform the vcgReadFile
  status = fFactory.doIt();

  return status;
}

