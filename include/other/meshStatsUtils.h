/*
 *
 */

#ifndef VCG_NODES_MESH_STATS_UTILS_H
#define VCG_NODES_MESH_STATS_UTILS_H

// Utilities
#include <utilities/debugUtils.h>

// STL
#include <ctime>
#include <cmath>

// VCG
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/position.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/inertia.h>
#include <vcg/complex/algorithms/stat.h>

class Timer
{
public:
  // timers, in order of use.
  debug::TimestampBenchmark m_meshOpen;
  debug::TimestampBenchmark m_cleanup;
  debug::TimestampBenchmark m_compute;
  debug::TimestampBenchmark m_total;

  void printResults()
  {
    std::cout << "Time Info:\n" << std::endl;
    this->m_meshOpen.printInSec("Mesh Open");
    this->m_cleanup.printInSec("Clean");
    this->m_compute.printInSec("Compute");
    this->m_total.printInSec("Total");
    std::cout << std::endl;
  }
};


// The class prototypes.
class Vertex_t;

class Face_t;

class UsedTypes_t : public vcg::UsedTypes<
    vcg::Use<Vertex_t>::AsVertexType,
    vcg::Use<Face_t>::AsFaceType>
{
};

class Vertex_t : public vcg::Vertex<
    UsedTypes_t,
    vcg::vertex::Coord3f,
    vcg::vertex::Curvaturef,
    vcg::vertex::Qualityf,
    vcg::vertex::Normal3f,
    vcg::vertex::TexCoord2f,
    vcg::vertex::VFAdj,
    vcg::vertex::Mark,
    vcg::vertex::BitFlags>
{
};

class Face_t : public vcg::Face<
    UsedTypes_t,
    vcg::face::VertexRef,
    vcg::face::Normal3f,
    vcg::face::VFAdj,
    vcg::face::FFAdj,
     vcg::face::PolyInfo, // turned this off because it causes errors.
    vcg::face::BitFlags>
{
};

// the main mesh class
class Mesh_t : public vcg::tri::TriMesh<
    std::vector<Vertex_t>,
    std::vector<Face_t> >
{
};

inline
void meshStats(Mesh_t &srcMesh,
               vcg::Point3f &outCentreOfMass,
               vcg::Point3f &outCentreOfShell,
               float &outMass,
               float &outArea,
               bool &outWaterTight,
               Timer &timer,
               bool verbose = false)
{
  timer.m_cleanup.start();

  const std::string toolHeader = "VCG Mesh Stats: ";

  // Ensure the meshes are up to date.
  vcg::tri::UpdateBounding<Mesh_t>::Box(srcMesh);
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(srcMesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(srcMesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(srcMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerVertex(srcMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerFace(srcMesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerFaceNormalized(srcMesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerVertexNormalized(srcMesh);
  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(srcMesh);

  vcg::tri::Clean<Mesh_t>::RemoveUnreferencedVertex(srcMesh);
  vcg::tri::Clean<Mesh_t>::RemoveNonManifoldFace(srcMesh);
  vcg::tri::Clean<Mesh_t>::RemoveFaceOutOfRangeArea(srcMesh);
  timer.m_cleanup.stop();
  timer.m_compute.start();

  outWaterTight = vcg::tri::Clean<Mesh_t>::IsWaterTight(srcMesh);
  if (outWaterTight)
  {

    vcg::tri::Inertia<Mesh_t> inertia(srcMesh);
    inertia.Compute(srcMesh);
    outMass = inertia.Mass();
    outCentreOfMass = inertia.CenterOfMass();
  }
  else
  {
    outMass = 0.0f;
    outCentreOfMass[0] = 0.0;
    outCentreOfMass[1] = 0.0;
    outCentreOfMass[2] = 0.0;
    WRN("Mesh is not water-tight, could not calculate accurate centre of mass.");
  }

  // Thin shell barycenter
  outCentreOfShell = vcg::tri::Stat<Mesh_t>::ComputeShellBarycenter(srcMesh);
  outArea = vcg::tri::Stat<Mesh_t>::ComputeMeshArea(srcMesh);

  DBG(toolHeader << "Out Water Tight: " << outWaterTight);
  DBG(toolHeader << "Out Centre Of Mass X: " << outCentreOfMass[0]);
  DBG(toolHeader << "Out Centre Of Mass Y: " << outCentreOfMass[1]);
  DBG(toolHeader << "Out Centre Of Mass Z: " << outCentreOfMass[2]);
  DBG(toolHeader << "Out Centre Of Shell X: " << outCentreOfShell[0]);
  DBG(toolHeader << "Out Centre Of Shell Y: " << outCentreOfShell[1]);
  DBG(toolHeader << "Out Centre Of Shell Z: " << outCentreOfShell[2]);
  DBG(toolHeader << "Out Mass: " << outMass);
  DBG(toolHeader << "Out Area: " << outArea);

  timer.m_compute.stop();
}

#endif // VCG_NODES_MESH_STATS_UTILS_H
