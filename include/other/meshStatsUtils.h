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
  DBG(toolHeader << "1");
  vcg::tri::UpdateBounding<Mesh_t>::Box(srcMesh);
  DBG(toolHeader << "1a");
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(srcMesh);
  DBG(toolHeader << "1b");
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(srcMesh);
  DBG(toolHeader << "1c");
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(srcMesh);
  DBG(toolHeader << "1d");
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerVertex(srcMesh);
  DBG(toolHeader << "1e");
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerFace(srcMesh);
  DBG(toolHeader << "1f");
  vcg::tri::UpdateNormal<Mesh_t>::PerFaceNormalized(srcMesh);
  DBG(toolHeader << "1g");
  vcg::tri::UpdateNormal<Mesh_t>::PerVertexNormalized(srcMesh);
  DBG(toolHeader << "2");
  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(srcMesh);

  DBG(toolHeader << "3");
  vcg::tri::Clean<Mesh_t>::RemoveUnreferencedVertex(srcMesh);
  DBG(toolHeader << "3a");
  vcg::tri::Clean<Mesh_t>::RemoveNonManifoldFace(srcMesh);
  DBG(toolHeader << "3b");
  vcg::tri::Clean<Mesh_t>::RemoveFaceOutOfRangeArea(srcMesh);
  DBG(toolHeader << "3c");
  timer.m_cleanup.stop();
  timer.m_compute.start();

  DBG(toolHeader << "4");
  outWaterTight = vcg::tri::Clean<Mesh_t>::IsWaterTight(srcMesh);
  if (outWaterTight)
  {

    DBG(toolHeader << "5");
    vcg::tri::Inertia<Mesh_t> inertia(srcMesh);
    DBG(toolHeader << "5a");
    inertia.Compute(srcMesh);
    DBG(toolHeader << "5b");
    outMass = inertia.Mass();
    DBG(toolHeader << "5c");
    outCentreOfMass = inertia.CenterOfMass();
    DBG(toolHeader << "5d");
  }
  else
  {
    DBG(toolHeader << "6");
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
  DBG(toolHeader << "7");
}

#endif // VCG_NODES_MESH_STATS_UTILS_H
