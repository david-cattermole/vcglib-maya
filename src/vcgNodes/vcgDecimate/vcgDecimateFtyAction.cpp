/*
 *
 */


// Local
#include <vcgNodes/vcgDecimate/vcgDecimateFty.h>

// Local Other
#include <other/decimateUtils.h>
#include <other/mayaConvertUtils.h>

// General Includes
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <maya/MFloatPointArray.h>

// Function Sets
#include <maya/MFnMesh.h>

// Iterators
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshEdge.h>

MStatus vcgDecimateFty::doIt()
//
//  Description:
//    Performs the operation on the selected mesh and components
//
{
//  DBG("DO IT!!!!");
  MStatus status;
  Timer timer;
  timer.m_total.start();
  const std::string toolHeader = "VCG Decimate: ";

  // Get access to the mesh's function set
  if (fMesh.isNull())
  {
    ERR("Mesh Object is invalid");
    return MS::kFailure;
  }
  MFnMesh meshFn(fMesh);

  Mesh_t tmpVcgMesh;
  INFO(toolHeader << "Converting Maya to VCG...");
  timer.m_meshOpen.start();
  convertMayaToVCG(fMesh, meshFn, tmpVcgMesh);
  timer.m_meshOpen.stop();

  bool verbose = fVerbose;
  bool cleaningFlag = true;
  double percent = fPercent / 100.0;
  double targetError = fTargetError;
  double maxDecimTime = fMaxTime;

  TriEdgeColQuadParam_t qparams;
  qparams.QualityThr = fQualityThreshold;
  qparams.QualityCheck = fUseQualityThreshold;
  qparams.NormalCheck = fUseNormalThreshold;
  qparams.NormalThrRad = vcg::math::ToRad(fNormalThreshold);
  qparams.OptimalPlacement = true;
  qparams.PreserveTopology = false;
  qparams.PreserveBoundary = fPreserveBoundary;
  qparams.BoundaryWeight = fBoundaryWeight;
  qparams.UseArea = true;
  qparams.UseVertexWeight = qparams.QualityWeight = true;

  // Decimate
  decimate(tmpVcgMesh, cleaningFlag,
           percent, targetError, maxDecimTime,
           timer, qparams, verbose);

  // Convert back.
  INFO(toolHeader << "Converting VCG to Maya...");
  timer.m_meshSave.start();
  convertVCGToMaya(fMesh, tmpVcgMesh, meshFn);
  timer.m_meshSave.stop();

  timer.m_total.stop();
  if (verbose)
  {
    timer.printResults();
  }
  else
  {
    timer.m_total.printInSec("Total");
  }
  return status;
}

