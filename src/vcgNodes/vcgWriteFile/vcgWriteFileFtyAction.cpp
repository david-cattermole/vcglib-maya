/*
 *
 */

// Local
#include <vcgNodes/vcgWriteFile/vcgWriteFileFty.h>

// Utils
#include <other/writeFileUtils.h>
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

MStatus vcgWriteFileFty::doIt()
//
//  Description:
//    Performs the operation on the selected mesh and components
//
{
  INFO("vcgWriteFileFty::doIt() -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
  MStatus status;
  Timer timer;
  timer.m_total.start();
  const std::string toolHeader = "VCG Write File: ";

  // Get access to the vcgMesh's function set
  if (fMesh.isNull())
  {
    INFO(toolHeader << "XXX Mesh Object is invalid");
    return MS::kFailure;
  }
  MFnMesh meshFn(fMesh);
  bool verbose = fVerbose;
  bool cleanMesh = fCleanMesh;

  // Convert to VCG format.
  Mesh_t vcgMesh;
  timer.m_meshSave.start();
  convertMayaToVCG(fMesh, meshFn, vcgMesh);
  timer.m_meshSave.stop();

  // Clean up
  timer.m_cleanup.start();
  int meshFaceNum = vcgMesh.FN();
  float pre_area = vcg::tri::Stat<Mesh_t>::ComputeMeshArea(vcgMesh);

  // Ensure the vcgMesh is up to date.
  vcg::tri::UpdateBounding<Mesh_t>::Box(vcgMesh);
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(vcgMesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(vcgMesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(vcgMesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerVertexNormalizedPerFace(vcgMesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerFace(vcgMesh);

  if (cleanMesh)
  {
    INFO(toolHeader << "Cleaning...");
    int manf_face = vcg::tri::Clean<Mesh_t>::RemoveNonManifoldFace(vcgMesh);
    VRB(toolHeader << "Removed " << manf_face << " folded faces");

    int manf_flip = vcg::tri::Clean<Mesh_t>::RemoveFaceFoldByFlip(vcgMesh);
    VRB(toolHeader << "Removed " << manf_flip << " flipped faces");

    VRB(toolHeader << "Finished Cleaning:");
    VRB(toolHeader << " Mesh vertices: " << vcgMesh.VN());
    VRB(toolHeader << " Mesh faces:    " << vcgMesh.FN() << std::endl);
  }
  timer.m_cleanup.stop();

  // Export
  const char *filePath = fFilePath.asChar();
  INFO("Saving Mesh: " << filePath << std::endl);
  int mask = vcg::tri::io::Mask::IOM_ALL;
  timer.m_meshSave.start();
  vcg::tri::io::Exporter<Mesh_t>::Save(vcgMesh, filePath, mask);
  timer.m_meshSave.stop();

  timer.m_total.stop();
//  timer.printResults();
  INFO("vcgWriteFileFty::doIt() End");
  return MS::kSuccess;
}



