/*
 *
 */

#ifndef VCG_NODES_READ_FILE_UTILS_H
#define VCG_NODES_READ_FILE_UTILS_H

// Utilities
#include <utilities/debugUtils.h>

// STL
#include <ctime>

// VCG
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/update/normal.h>

class Timer
{
public:
  // timers, in order of use.
  debug::TimestampBenchmark m_meshOpen;
  debug::TimestampBenchmark m_cleanup;
  debug::TimestampBenchmark m_meshSave;
  debug::TimestampBenchmark m_total;

  void printResults()
  {
    std::cout << "Time Info:\n" << std::endl;
    this->m_meshOpen.printInSec("Mesh Open");
    this->m_cleanup.printInSec("Mesh Clean");
    this->m_meshSave.printInSec("Mesh Save");
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
    vcg::vertex::Curvaturef,
    vcg::vertex::Qualityf,
    vcg::vertex::Coord3f,
    vcg::vertex::Color4b,
    vcg::vertex::Normal3f,
    vcg::vertex::TexCoord2f,
    vcg::vertex::VFAdj,
    vcg::vertex::Mark,
    vcg::vertex::BitFlags>
{
};

class Face_t : public vcg::Face<
    UsedTypes_t,
    vcg::face::Normal3f,
    vcg::face::VertexRef,
    vcg::face::WedgeTexCoord2f,
    vcg::face::VFAdj,
    vcg::face::FFAdj,
    vcg::face::PolyInfo,
    vcg::face::BitFlags>
{
};

// the main mesh class
class Mesh_t : public vcg::tri::TriMesh<
    std::vector<Vertex_t>,
    std::vector<Face_t> >
{
};


#endif // VCG_NODES_READ_FILE_UTILS_H
