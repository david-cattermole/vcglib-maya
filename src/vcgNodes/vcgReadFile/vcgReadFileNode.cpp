/*
 *
 */

// Local
#include <vcgNodes/vcgReadFile/vcgReadFileNode.h>

// Utils
#include <utilities/debugUtils.h>

// Function Sets
#include <maya/MFnMeshData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnStringData.h>
#include <maya/MString.h>

// General Includes
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MIOStream.h>

// Macros
#define MCheckStatus(status, message)  \
  if( MStatus::kSuccess != status ) {  \
    cerr << message << "\n";           \
    return status;                     \
  }


// Unique Node TypeId
MTypeId vcgReadFileNode::id(0x0008510A); // Use a unique ID.

// Node attributes
MObject vcgReadFileNode::inMesh;
MObject vcgReadFileNode::outMesh;
MObject vcgReadFileNode::aEnable;
MObject vcgReadFileNode::aFilePath;

vcgReadFileNode::vcgReadFileNode()
{}

vcgReadFileNode::~vcgReadFileNode()
{}

MStatus vcgReadFileNode::compute(const MPlug &plug, MDataBlock &data)
//
//  Description:
//    This method computes the value of the given output plug based
//    on the values of the input attributes.
//
//  Arguments:
//    plug - the plug to compute
//    data - object that provides access to the attributes for this node
//
{
  MStatus status = MS::kSuccess;

  MDataHandle stateData = data.outputValue(state, &status);
  MCheckStatus(status, "ERROR getting state");
  INFO("vcgReadFile plug: " << plug.name());

  // Check for the HasNoEffect/PassThrough flag on the node.
  //
  // (stateData is an enumeration standard in all depend nodes)
  //
  // (0 = Normal)
  // (1 = HasNoEffect/PassThrough)
  // (2 = Blocking)
  // ...
  //
  if (stateData.asShort() == 1)
  {
    MDataHandle inputData = data.inputValue(inMesh, &status);
    MCheckStatus(status, "ERROR getting inMesh");

    MDataHandle outputData = data.outputValue(outMesh, &status);
    MCheckStatus(status, "ERROR getting outMesh");

    // Simply redirect the inMesh to the outMesh for the PassThrough effect
    outputData.set(inputData.asMesh());
  }
  else
  {
    // Check which output attribute we have been asked to
    // compute. If this node doesn't know how to compute it,
    // we must return MS::kUnknownParameter
    if (plug == outMesh)
    {
      MDataHandle inputData = data.inputValue(inMesh, &status);
      MCheckStatus(status, "ERROR getting inMesh");

      MDataHandle outputData = data.outputValue(outMesh, &status);
      MCheckStatus(status, "ERROR getting outMesh");

      // Copy the inMesh to the outMesh, so you can
      // perform operations directly on outMesh
      outputData.set(inputData.asMesh());

      // Return if the node is not enabled.
      MDataHandle enableData = data.inputValue(aEnable, &status);
      MCheckStatus(status, "ERROR getting aEnable");
      if (!enableData.asBool())
      {
        return MS::kSuccess;
      }

      // Get File Path
      MDataHandle filePathData = data.inputValue(aFilePath, &status);
      MString filePath(filePathData.asString());
      fFactory.setFilePath(filePath);

      // Get Mesh object
      MObject mesh = outputData.asMesh();

      // Set the mesh object and component List on the factory
      fFactory.setMesh(mesh);

      // Now, perform the vcgReadFile
      status = fFactory.doIt();

      // Mark the output mesh as clean
      outputData.setClean();
    }
    else
    {
      status = MS::kUnknownParameter;
    }
  }

  return status;
}

void *vcgReadFileNode::creator()
//
//  Description:
//    this method exists to give Maya a way to create new objects
//      of this type. 
//
//  Return Value:
//    a new object of this type
//
{
  return new vcgReadFileNode();
}

MStatus vcgReadFileNode::initialize()
//
//  Description:
//    This method is called to create and initialize all of the attributes
//      and attribute dependencies for this node type.  This is only called 
//    once when the node type is registered with Maya.
//
//  Return Values:
//    MS::kSuccess
//    MS::kFailure
//    
{
  MStatus status;

  MFnTypedAttribute attrFn;
  MFnNumericAttribute numFn;
  MFnTypedAttribute typedAttr;

  // File Path string data
  MFnStringData stringData;
  MObject stringObj;
  stringObj = stringData.create(&status);
  CHECK_MSTATUS(status);

  // Enable
  aEnable = numFn.create("enable", "enable",
                         MFnNumericData::kBoolean, true);
  status = numFn.setDefault(true);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(true);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = addAttribute(aEnable);
  CHECK_MSTATUS(status);

  // File Path
  aFilePath = typedAttr.create("filePath", "filePath",
                               MFnData::kString, stringObj,
                               &status);
  typedAttr.setUsedAsFilename(true);
  typedAttr.setAffectsAppearance(true);
  addAttribute(aFilePath);
  CHECK_MSTATUS(status);

  // Input Mesh
  inMesh = attrFn.create("inMesh", "im",
                         MFnMeshData::kMesh);
  attrFn.setStorable(false);
  attrFn.setHidden(true);
  status = addAttribute(inMesh);
  CHECK_MSTATUS(status);

  // Output Mesh
  // Attribute is read-only because it is an output attribute
  outMesh = attrFn.create("outMesh", "om",
                          MFnMeshData::kMesh);
  attrFn.setStorable(false);
  attrFn.setWritable(false);
  status = addAttribute(outMesh);
  CHECK_MSTATUS(status);

  // Attribute affects
  status = attributeAffects(inMesh, outMesh);
  status = attributeAffects(aFilePath, outMesh);
  status = attributeAffects(aEnable, outMesh);
  CHECK_MSTATUS(status);

  return MS::kSuccess;

}
