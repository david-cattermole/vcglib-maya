/*
 *
 */

#ifndef VCG_NODES_AUTO_ALIGN_UTILS_H
#define VCG_NODES_AUTO_ALIGN_UTILS_H

// Utilities
#include <utilities/debugUtils.h>

// STL
#include <ctime>
#include <cmath>

//// HACK: 'qDebug' is used inside 'autoalign_4pc.h', but is
//// never defined in the file or the includes. Therefore
//// we must define a dummy before we include 'autoalign_4pc.h'.
//#define qDebug(w,x,y,z)

// VCG
#include <vcg/complex/complex.h>
#include <vcg/space/point_matching.h>
#include <vcg/complex/algorithms/autoalign_4pcs.h>
#include <vcg/complex/algorithms/closest.h>
#include <vcg/complex/algorithms/point_sampling.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/position.h>
#include <vcg/math/matrix44.h>
#include <vcg/math/random_generator.h>

// RANSAC
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>
#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/inertia.h>
#include <vcg/complex/algorithms/point_sampling.h>
#include <vcg/complex/algorithms/ransac_matching.h>

class Timer
{
public:
  // timers, in order of use.
  debug::TimestampBenchmark m_meshOpen;
  debug::TimestampBenchmark m_cleanup;
  debug::TimestampBenchmark m_alignment;
  debug::TimestampBenchmark m_postCleanup;
  debug::TimestampBenchmark m_meshSave;
  debug::TimestampBenchmark m_total;

  void printResults()
  {
    std::cout << "Time Info:\n" << std::endl;
    this->m_meshOpen.printInSec("Mesh Open");
    this->m_cleanup.printInSec("Pre-Clean Up");
    this->m_alignment.printInSec("Alignment");
    this->m_postCleanup.printInSec("Post-Clean Up");
    this->m_meshSave.printInSec("Mesh Save");
    this->m_total.printInSec("Total");
    std::cout << std::endl;
  }
};

/**********************************************************
Mesh Classes for Quadric Edge collapse based simplification

For edge collpases we need verteses with:
- V->F adjacency
- per vertex incremental mark
- per vertex Normal

Moreover for using a quadric based collapse the vertex class
must have also a Quadric member Q();
Otherwise the user have to provide an helper function object
to recover the quadric.

******************************************************/

// The class prototypes.
class Vertex_t;

class Edge_t;

class Face_t;

class UsedTypes_t : public vcg::UsedTypes<
    vcg::Use<Vertex_t>::AsVertexType,
    vcg::Use<Edge_t>::AsEdgeType,
    vcg::Use<Face_t>::AsFaceType>
{
};

class Vertex_t : public vcg::Vertex<UsedTypes_t,
    vcg::vertex::Coord3f,
    vcg::vertex::Normal3f,
    vcg::vertex::TexCoord2f,
    vcg::vertex::VFAdj,
    vcg::vertex::Mark,
    vcg::vertex::BitFlags>
{
};

class Edge_t : public vcg::Edge<UsedTypes_t,
    vcg::edge::VertexRef,
//    vcg::edge::VEAdj,
//    vcg::edge::EEAdj,
    vcg::edge::BitFlags>
{
};

class Face_t : public vcg::Face<UsedTypes_t,
    vcg::face::VertexRef,
    vcg::face::FFAdj,
    vcg::face::Normal3f,
    vcg::face::VFAdj,
    vcg::face::BitFlags>
{
};

// the main mesh class
class Mesh_t : public vcg::tri::TriMesh<
    std::vector<Vertex_t>,
    std::vector<Face_t>,
    std::vector<Edge_t> >
{
};

inline
void autoAlign(Mesh_t &refMesh,
               Mesh_t &srcMesh,
               float radius,       // 0.008f
               float featureRatio, // 0.5f
               int maxIterations,  // 400
               int maxFeatures,    // 500
               Timer &timer,
               bool verbose = false)
{
  timer.m_cleanup.start();

  const std::string toolHeader = "VCG AutoAlign: ";

  // Ensure the meshes are up to date.
  vcg::tri::UpdateBounding<Mesh_t>::Box(refMesh);
  vcg::tri::UpdateBounding<Mesh_t>::Box(srcMesh);
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(refMesh);
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(srcMesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(refMesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(srcMesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(refMesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(srcMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerVertex(refMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerVertex(srcMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerFace(refMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerFace(srcMesh);
  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(refMesh);
  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(srcMesh);

  INFO(toolHeader << "Cleaning...");
  vcg::tri::Clean<Mesh_t>::RemoveNonManifoldFace(refMesh);
  vcg::tri::Clean<Mesh_t>::RemoveNonManifoldFace(srcMesh);
  vcg::tri::Clean<Mesh_t>::RemoveUnreferencedVertex(refMesh);
  vcg::tri::Clean<Mesh_t>::RemoveUnreferencedVertex(srcMesh);
  vcg::tri::Clean<Mesh_t>::RemoveFaceFoldByFlip(refMesh);
  vcg::tri::Clean<Mesh_t>::RemoveFaceFoldByFlip(srcMesh);
  vcg::tri::Clean<Mesh_t>::RemoveFaceOutOfRangeArea(refMesh);
  vcg::tri::Clean<Mesh_t>::RemoveFaceOutOfRangeArea(srcMesh);
  VRB(toolHeader << "Finished Cleaning:");
  VRB(toolHeader << " REF Mesh vertices: " << refMesh.VN());
  VRB(toolHeader << " REF Mesh faces:    " << refMesh.FN());
  VRB(toolHeader << " SRC Mesh vertices: " << srcMesh.VN());
  VRB(toolHeader << " SRC Mesh faces:    " << srcMesh.FN() << std::endl);
  timer.m_cleanup.stop();
  timer.m_alignment.start();

  {
    vcg::math::MarsenneTwisterRNG rnd;

    vcg::tri::UpdateBounding<Mesh_t>::Box(refMesh);
    vcg::tri::UpdateBounding<Mesh_t>::Box(srcMesh);

    vcg::RansacFramework<Mesh_t, vcg::BaseFeatureSet<Mesh_t> > Ran;
    vcg::RansacFramework<Mesh_t, vcg::BaseFeatureSet<Mesh_t> >::Param pp;
    vcg::BaseFeatureSet<Mesh_t>::Param fpp;
    pp.samplingRadiusPerc = radius;
    pp.iterMax = maxIterations;
    pp.maxMatchingFeatureNum = maxFeatures;
    fpp.featureSampleRatio = featureRatio;
    pp.evalSize = 50;
    pp.inlierRatioThr = 0.5;
    std::vector<vcg::RansacFramework<Mesh_t, vcg::BaseFeatureSet<Mesh_t> >::Candidate> cVec;

    Ran.Init(refMesh, srcMesh, pp, fpp);
    Ran.Process_SearchEvaluateTriple(cVec, pp);

    // Apply matrix
    vcg::tri::UpdatePosition<Mesh_t>::Matrix(srcMesh, cVec[0].Tr);
  }

  timer.m_alignment.stop();
  timer.m_postCleanup.start();
  INFO(toolHeader << "Cleaning...");

  int dup_vert = vcg::tri::Clean<Mesh_t>::RemoveDuplicateVertex(srcMesh);
  VRB(toolHeader << "Removed " << dup_vert << " duplicate vertices");

  int unref_vert = vcg::tri::Clean<Mesh_t>::RemoveUnreferencedVertex(srcMesh);
  VRB(toolHeader << "Removed " << unref_vert << " unreferenced vertices");

  int dup_face = vcg::tri::Clean<Mesh_t>::RemoveDuplicateFace(srcMesh);
  VRB(toolHeader << "Removed " << dup_face << " duplicate faces");

  VRB(toolHeader << "Finished Cleaning:");
  VRB(toolHeader << " Mesh vertices:  " << srcMesh.VN());
  VRB(toolHeader << " Mesh faces:     " << srcMesh.FN());
  timer.m_postCleanup.stop();
}

#endif // VCG_NODES_AUTO_ALIGN_UTILS_H
