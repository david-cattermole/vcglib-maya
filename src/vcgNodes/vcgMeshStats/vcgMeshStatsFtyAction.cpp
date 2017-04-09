/*
 *
 */

// Local
#include <vcgNodes/vcgMeshStats/vcgMeshStatsFty.h>

// Utils
#include <other/meshStatsUtils.h>
#include <other/mayaConvertUtils.h>
#include <other/loadModelUtils.h>

// General Includes
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <maya/MFloatPointArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnTransform.h>

// Function Sets
#include <maya/MFnMesh.h>


MStatus vcgMeshStatsFty::doIt()
//
//  Description:
//    Performs the operation on the selected mesh and components
//
{
  INFO("vcgMeshStatsFty::doIt() -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
  MStatus status;
  Timer timer;
  timer.m_total.start();
  const std::string toolHeader = "VCG Mesh Stats: ";

  // Get access to the mesh's function set
  if (fMesh.isNull())
  {
    //ERR(toolHeader << "XXX Mesh Object is invalid");
     INFO("XXX Mesh Object is invalid");
    return MS::kFailure;
  }
  MFnMesh meshFn(fMesh);
  bool verbose = true;

   Mesh_t vcgMesh;
  // INFO(toolHeader << "XXX Converting Maya to VCG...");
  INFO("XXX Converting Maya to VCG...");
  timer.m_meshOpen.start();
  convertMayaToVCG(fMesh, meshFn, vcgMesh);
  timer.m_meshOpen.stop();

//  timer.m_meshOpen.start();
//  Mesh_t vcgMesh;
//  // const char *filePath = "/home/davidc/dev/mayaVcgNodes/data/sphere.obj";
//  const char *filePath = "/home/davidc/dev/mayaVcgNodes/data/gargoyle.ply";
//  int mask = vcg::tri::io::Mask::IOM_ALL;
//  int stat = loadModel(filePath, vcgMesh, mask, verbose);
//  if (stat == EXIT_FAILURE) { return MS::kFailure; }
//  timer.m_meshOpen.stop();

  // Get Mesh Statistics
  // INFO(toolHeader << "XXX Mesh Stats");
  INFO("XXX Mesh Stats");
  vcg::Point3f centreOfMass(0.0f, 0.0f, 0.0f);
  vcg::Point3f centreOfShell(0.0f, 0.0f, 0.0f);
  float mass = 0.0f;
  float area = 0.0f;
  bool waterTight = false;
  meshStats(
      vcgMesh,
      centreOfMass,
      centreOfShell,
      mass,
      area,
      waterTight,
      timer,
      verbose);

  // Set outputs.
  // INFO(toolHeader << "XXX Outputs");
  INFO("XXX Outputs");
  fCentreOfMass[0] = centreOfMass[0];
  fCentreOfMass[1] = centreOfMass[1];
  fCentreOfMass[2] = centreOfMass[2];
  fMass = mass;

  INFO("XXX Centre Of Mass: " << centreOfMass[0] << ","
                              << centreOfMass[1] << ","
                              << centreOfMass[2]);
  INFO("XXX Centre Of Shell: " << centreOfShell[0] << ","
                               << centreOfShell[1] << ","
                               << centreOfShell[2]);
  INFO("XXX Mass: " << mass);
  INFO("XXX Area: " << area);

  timer.m_total.stop();
//  timer.printResults();
  INFO("vcgMeshStatsFty::doIt() End");
  return MS::kSuccess;
}



