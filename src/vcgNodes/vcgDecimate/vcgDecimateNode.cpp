/*
 *
 */

// Local
#include <vcgNodes/vcgDecimate/vcgDecimateNode.h>

// Utils
#include <debugUtils.h>

// Function Sets
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnComponentListData.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

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
MTypeId     vcgDecimateNode::id(0x00085001); // Use a unique ID.

// Node attributes
MObject     vcgDecimateNode::inMesh;
MObject     vcgDecimateNode::outMesh;
MObject     vcgDecimateNode::aPercent;
MObject     vcgDecimateNode::aMaxTime;
MObject     vcgDecimateNode::aUseQualityThreshold;
MObject     vcgDecimateNode::aQualityThreshold;
MObject     vcgDecimateNode::aUseNormalThreshold;
MObject     vcgDecimateNode::aNormalThreshold;
MObject     vcgDecimateNode::aBoundaryWeight;
MObject     vcgDecimateNode::aPreserveBoundary;
MObject     vcgDecimateNode::aVerbose;

vcgDecimateNode::vcgDecimateNode()
{}

vcgDecimateNode::~vcgDecimateNode()
{}

MStatus vcgDecimateNode::compute(const MPlug &plug, MDataBlock &data)
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
      MObject mesh = outputData.asMesh();

      // Set the mesh object and component List on the factory
      fFactory.setMesh(mesh);

      // Set percent
      MDataHandle percentHandle = data.inputValue(aPercent, &status);
      MCheckStatus(status, "ERROR getting percent");
      fFactory.setPercent(percentHandle.asDouble());

      // Set Max Time
      MDataHandle maxTimeHandle = data.inputValue(aMaxTime, &status);
      MCheckStatus(status, "ERROR getting maxTime");
      fFactory.setMaxTime(maxTimeHandle.asDouble());

      // Set Use Quality Threshold
      MDataHandle useQualityThresholdHandle = data.inputValue(aUseQualityThreshold, &status);
      MCheckStatus(status, "ERROR getting useQualityThreshold");
      fFactory.setUseQualityThreshold(useQualityThresholdHandle.asBool());

      // Set Quality Threshold
      MDataHandle qualityThresholdHandle = data.inputValue(aQualityThreshold, &status);
      MCheckStatus(status, "ERROR getting qualityThreshold");
      fFactory.setQualityThreshold(qualityThresholdHandle.asDouble());

      // Set Use Normal Threshold
      MDataHandle useNormalThresholdHandle = data.inputValue(aUseNormalThreshold, &status);
      MCheckStatus(status, "ERROR getting useNormalThreshold");
      fFactory.setUseNormalThreshold(useNormalThresholdHandle.asBool());

      // Set Normal Threshold
      MDataHandle normalThresholdHandle = data.inputValue(aNormalThreshold, &status);
      MCheckStatus(status, "ERROR getting normalThreshold");
      fFactory.setNormalThreshold(normalThresholdHandle.asDouble());

      // Set Preserve Boundary
      MDataHandle preserveBoundaryHandle = data.inputValue(aPreserveBoundary, &status);
      MCheckStatus(status, "ERROR getting preserveBoundary");
      fFactory.setPreserveBoundary(preserveBoundaryHandle.asBool());

      // Set Boundary Weight
      MDataHandle boundaryWeightHandle = data.inputValue(aBoundaryWeight, &status);
      MCheckStatus(status, "ERROR getting boundaryWeight");
      fFactory.setBoundaryWeight(boundaryWeightHandle.asDouble());

      // Set Verbose
      MDataHandle verboseHandle = data.inputValue(aVerbose, &status);
      MCheckStatus(status, "ERROR getting verbose");
      fFactory.setVerbose(verboseHandle.asBool());

      // Now, perform the vcgDecimate
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

void *vcgDecimateNode::creator()
//
//  Description:
//    this method exists to give Maya a way to create new objects
//      of this type.
//
//  Return Value:
//    a new object of this type
//
{
  return new vcgDecimateNode();
}

MStatus vcgDecimateNode::initialize()
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
  MFnEnumAttribute enumFn;
  MFnNumericAttribute numFn;

  aPercent = numFn.create("percent", "percent",
                                           MFnNumericData::kDouble, 100.0);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = numFn.setMin(0.0);
  status = numFn.setMax(100.0);
  status = addAttribute(aPercent);
  CHECK_MSTATUS(status);

  aMaxTime = numFn.create("maxTime", "maxTime",
                                           MFnNumericData::kDouble, 3600.0);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = numFn.setMin(0.0f);
  status = addAttribute(aMaxTime);
  CHECK_MSTATUS(status);

  aUseQualityThreshold = numFn.create("useQualityThreshold",
                                                       "useQualityThreshold",
                                      MFnNumericData::kBoolean, 1);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = addAttribute(aUseQualityThreshold);
  CHECK_MSTATUS(status);

  aQualityThreshold = numFn.create("qualityThreshold",
                                   "qualityThreshold",
                                   MFnNumericData::kDouble,
                                   0.3);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = numFn.setMin(0.0);
  status = numFn.setMax(2.0);
  status = addAttribute(aQualityThreshold);
  CHECK_MSTATUS(status);

  aBoundaryWeight = numFn.create("boundaryWeight",
                                 "boundaryWeight",
                                 MFnNumericData::kDouble,
                                 0.5);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = numFn.setMin(0.0);
  status = numFn.setMax(1.0);
  status = addAttribute(aBoundaryWeight);
  CHECK_MSTATUS(status);

  aUseNormalThreshold = numFn.create("useNormalThreshold",
                                     "useNormalThreshold",
                                     MFnNumericData::kBoolean, 0);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(true);
  status = addAttribute(aUseNormalThreshold);
  CHECK_MSTATUS(status);

  aNormalThreshold = numFn.create("normalThreshold",
                                  "normalThreshold",
                                  MFnNumericData::kDouble,
                                  5.0);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(true);
  status = numFn.setMin(0.0);
  status = numFn.setMax(180.0);
  status = addAttribute(aNormalThreshold);
  CHECK_MSTATUS(status);

  aPreserveBoundary = numFn.create("preserveBoundary",
                                   "preserveBoundary",
                                   MFnNumericData::kBoolean, 0);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(true);
  status = addAttribute(aPreserveBoundary);
  CHECK_MSTATUS(status);

  aVerbose = numFn.create("verbose", "verbose",
                                           MFnNumericData::kBoolean, 0);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(false);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = addAttribute(aVerbose);
  CHECK_MSTATUS(status);

  inMesh = attrFn.create("inMesh", "im",
                         MFnMeshData::kMesh);
  attrFn.setStorable(true);  // To be stored during file-save
  status = addAttribute(inMesh);
  CHECK_MSTATUS(status);

  // Attribute is read-only because it is an output attribute
  outMesh = attrFn.create("outMesh", "om",
                                            MFnMeshData::kMesh);
  attrFn.setStorable(false);
  attrFn.setWritable(false);
  status = addAttribute(outMesh);
  CHECK_MSTATUS(status);

  // Attribute affects
  status = attributeAffects(inMesh, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aPercent, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aMaxTime, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aUseQualityThreshold, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aQualityThreshold, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aUseNormalThreshold, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aNormalThreshold, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aBoundaryWeight, outMesh);
  CHECK_MSTATUS(status);
  status = attributeAffects(aPreserveBoundary, outMesh);
  CHECK_MSTATUS(status);

  return MS::kSuccess;

}
