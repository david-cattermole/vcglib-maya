/*
 *
 */

// Local
#include <vcgNodes/vcgReadFile/vcgReadFileFty.h>

// Utils
#include <other/readFileUtils.h>
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

MStatus vcgReadFileFty::doIt()
//
//  Description:
//    Performs the operation on the selected mesh and components
//
{
  INFO("vcgReadFileFty::doIt() -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
  MStatus status;
  Timer timer;
  timer.m_total.start();
  const std::string toolHeader = "VCG Read File: ";

  // Get access to the vcgMesh's function set
  if (fMesh.isNull())
  {
    INFO(toolHeader << "XXX Mesh Object is invalid");
    return MS::kFailure;
  }
  MFnMesh meshFn(fMesh);

  // Do something with 'meshFn'...
  timer.m_meshOpen.start();
  Mesh_t vcgMesh;
  const char *filePath = fFilePath.asChar();
  int mask = vcg::tri::io::Mask::IOM_ALL;
  bool verbose = true;
  int stat = loadModel(filePath, vcgMesh, mask, verbose);
  if (stat == EXIT_FAILURE) { return MS::kFailure; }
  timer.m_meshOpen.stop();

  // Convert to Maya format...
  timer.m_meshSave.start();
  convertVCGToMaya(fMesh, vcgMesh, meshFn);
  timer.m_meshSave.stop();

  timer.m_total.stop();
//  timer.printResults();
  INFO("vcgReadFileFty::doIt() End");
  return MS::kSuccess;
}



