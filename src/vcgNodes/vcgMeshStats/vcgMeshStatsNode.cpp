/*
 *
 */

// Local
#include <vcgNodes/vcgMeshStats/vcgMeshStatsNode.h>
#include <vcgNodes/vcgNodeTypeIds.h>

// Utils
#include <utilities/debugUtils.h>

// Function Sets
#include <maya/MFnMeshData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>


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
// See 'vcgNodeTypeIds.h', add a definition.
MTypeId vcgMeshStatsNode::id(VCG_MESH_STATS_NODE_ID); // Use a unique ID.

// Node attributes
MObject vcgMeshStatsNode::inMesh;
MObject vcgMeshStatsNode::outMesh;
MObject vcgMeshStatsNode::aEnable;
MObject vcgMeshStatsNode::aOutCentreOfMass;
MObject vcgMeshStatsNode::aOutMass;

vcgMeshStatsNode::vcgMeshStatsNode()
{}

vcgMeshStatsNode::~vcgMeshStatsNode()
{}

MStatus vcgMeshStatsNode::compute(const MPlug &plug, MDataBlock &data)
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
  INFO("vcgMeshStats plug: " << plug.name());

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
    if (plug == outMesh || plug == aOutCentreOfMass || plug == aOutMass)
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

      // Get Mesh object
      MObject mesh = outputData.asMesh();

      // Set the mesh object and component List on the factory
      fFactory.setMesh(mesh);

      // Now, perform the vcgMeshStats
      status = fFactory.doIt();

      // Centre Of Mass Output
      MVector centreOfMass(fFactory.getCentreOfMass());
      INFO("compute centre of mass X:" << centreOfMass[0]);
      INFO("compute centre of mass Y:" << centreOfMass[1]);
      INFO("compute centre of mass Z:" << centreOfMass[2]);
      MDataHandle centreOfMassData = data.outputValue(aOutCentreOfMass, &status);
      MCheckStatus(status, "ERROR getting aOutCentreOfMass");
      centreOfMassData.setMVector(centreOfMass);

      // Mass Output
      float mass = fFactory.getMass();
      INFO("compute mass:" << mass);
      MDataHandle massData = data.outputValue(aOutMass, &status);
      MCheckStatus(status, "ERROR getting aOutMass");
      massData.setFloat(mass);

      // Mark the output mesh as clean
      outputData.setClean();
      centreOfMassData.setClean();
      massData.setClean();
    }
//    else if
//    {
//      MDataHandle inputData = data.inputValue(inMesh, &status);
//      MCheckStatus(status, "ERROR getting inMesh");
//
//      // Return if the node is not enabled.
//      MDataHandle enableData = data.inputValue(aEnable, &status);
//      MCheckStatus(status, "ERROR getting aEnable");
//      if (!enableData.asBool())
//      {
//        return MS::kSuccess;
//      }
//
//      // Get Mesh object
//      MObject mesh = inputData.asMesh();
//
//      // Set the mesh object and component List on the factory
//      fFactory.setMesh(mesh);
//
//      // Now, perform the vcgMeshStats
//      status = fFactory.doIt();
//
//      // Centre Of Mass Output
//      MVector centreOfMass(fFactory.getCentreOfMass());
//      MDataHandle centreOfMassData = data.outputValue(aOutCentreOfMass, &status);
//      MCheckStatus(status, "ERROR getting aOutCentreOfMass");
//      centreOfMassData.setMVector(centreOfMass);
//
//      // Mass Output
//      float mass = fFactory.getMass();
//      MDataHandle massData = data.outputValue(aOutMass, &status);
//      MCheckStatus(status, "ERROR getting aOutMass");
//      massData.setFloat(mass);
//
//      // Mark the output mesh as clean
//
//    }
    else
    {
      status = MS::kUnknownParameter;
    }
  }

  return status;
}

void *vcgMeshStatsNode::creator()
//
//  Description:
//    this method exists to give Maya a way to create new objects
//      of this type. 
//
//  Return Value:
//    a new object of this type
//
{
  return new vcgMeshStatsNode();
}

MStatus vcgMeshStatsNode::initialize()
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

  aEnable = numFn.create("enable", "enable",
                         MFnNumericData::kBoolean, true, &status);
  CHECK_MSTATUS(status);
  status = numFn.setDefault(true);
  status = numFn.setStorable(true);
  status = numFn.setKeyable(true);
  status = numFn.setChannelBox(true);
  status = numFn.setHidden(false);
  status = addAttribute(aEnable);
  CHECK_MSTATUS(status);

  // Centre of Mass
  aOutCentreOfMass = numFn.createPoint("outCentreOfMass", "outCentreOfMass",
                                       &status);
  // MFnNumericData::k3Float, 0.0, &status);
  CHECK_MSTATUS(status);
  // numFn.setDefault(0.0f, 0.0f, 0.0f);
  // numFn.setKeyable(false);
  numFn.setStorable(false);
  numFn.setWritable(false);
  status = addAttribute(aOutCentreOfMass);
  CHECK_MSTATUS(status);

  // Mass
  aOutMass = numFn.create("outMass", "outMass",
                          MFnNumericData::kFloat, 0.0,
                          &status);
  CHECK_MSTATUS(status);
  numFn.setDefault(0.0f);
  numFn.setKeyable(false);
  numFn.setStorable(false);
  numFn.setWritable(false);
  status = addAttribute(aOutMass);
  CHECK_MSTATUS(status);

  // Input Mesh
  inMesh = attrFn.create("inMesh", "im", MFnMeshData::kMesh);
  attrFn.setStorable(true);  // To be stored during file-save
  status = addAttribute(inMesh);
  CHECK_MSTATUS(status);

  // Output Mesh
  // Attribute is read-only because it is an output attribute
  outMesh = attrFn.create("outMesh", "om", MFnMeshData::kMesh);
  attrFn.setStorable(false);
  attrFn.setWritable(false);
  status = addAttribute(outMesh);
  CHECK_MSTATUS(status);

  // Attribute affects
  status = attributeAffects(inMesh, outMesh);
  status = attributeAffects(aEnable, outMesh);
  status = attributeAffects(inMesh, aOutCentreOfMass);
  status = attributeAffects(aEnable, aOutCentreOfMass);
  status = attributeAffects(inMesh, aOutMass);
  status = attributeAffects(aEnable, aOutMass);
  CHECK_MSTATUS(status);

  return MS::kSuccess;

}
