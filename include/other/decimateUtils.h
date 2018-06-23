/*
 *
 */

#ifndef VCG_NODES_DECIMATE_UTILS_H
#define VCG_NODES_DECIMATE_UTILS_H

// Utilities
#include <utilities/debugUtils.h>

// STL
#include <ctime>

// VCG
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/clustering.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/update/normal.h>

// uncomment the line below to turn on experimental texture coordinate decimation.
// #define WITH_TEXTURE_COORDS
#ifdef WITH_TEXTURE_COORDS

#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric_tex.h>

#else

#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>

#endif

class Timer
{
public:
  // timers, in order of use.
  debug::TimestampBenchmark m_meshOpen;
  debug::TimestampBenchmark m_cleanup;
  debug::TimestampBenchmark m_bbox;
  debug::TimestampBenchmark m_curvature;
  debug::TimestampBenchmark m_deciInit;
  debug::TimestampBenchmark m_decimation;
  debug::TimestampBenchmark m_postCleanup;
  debug::TimestampBenchmark m_meshSave;
  debug::TimestampBenchmark m_total;

  void printResults()
  {
    std::cout << "Time Info:\n" << std::endl;
    this->m_meshOpen.printInSec("Mesh Open");
    this->m_cleanup.printInSec("Pre-Clean Up");
    this->m_bbox.printInSec("B-Box");
    this->m_curvature.printInSec("Curvature");
    this->m_deciInit.printInSec("Deci Init");
    this->m_decimation.printInSec("Decimation");
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

class Face_t;

class UsedTypes_t : public vcg::UsedTypes<
    vcg::Use<Vertex_t>::AsVertexType,
    vcg::Use<Face_t>::AsFaceType>
{
};

class Vertex_t : public vcg::Vertex<UsedTypes_t,
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
public:
  vcg::math::Quadric<double> &Qd()
  {
    return q;
  }

private:
  vcg::math::Quadric<double> q;
};

typedef BasicVertexPair<Vertex_t> VertexPair_t;

class Face_t : public vcg::Face<UsedTypes_t,
    vcg::face::Normal3f,
    vcg::face::PolyInfo,
    vcg::face::VertexRef,
    vcg::face::WedgeTexCoord2f,
    vcg::face::VFAdj,
    vcg::face::FFAdj,
    vcg::face::BitFlags>
{
};

// the main mesh class
class Mesh_t : public vcg::tri::TriMesh<
    std::vector<Vertex_t>,
    std::vector<Face_t> >
{
};

#ifdef WITH_TEXTURE_COORDS

class TriEdgeCollapseTex_t : public TriEdgeCollapseQuadricTex<
    Mesh_t,
    VertexPair_t,
    TriEdgeCollapseTex_t,
    vcg::tri::QuadricTexHelper<Mesh_t> >
{
public:
  typedef TriEdgeCollapseQuadricTex<Mesh_t,
      VertexPair_t,
      TriEdgeCollapseTex_t,
      vcg::tri::QuadricTexHelper<Mesh_t> > TECQ;

  inline TriEdgeCollapseTex_t(const VertexPair_t &p,
                              int i,
                              vcg::BaseParameterClass *pp) :
      TECQ(p, i, pp)
  {}
};

typedef vcg::tri::TriEdgeCollapseQuadricTexParameter TriEdgeColQuadParam_t;

#else

class TriEdgeCollapse_t : public vcg::tri::TriEdgeCollapseQuadric<
    Mesh_t,
    VertexPair_t,
    TriEdgeCollapse_t,
    QInfoStandard<Vertex_t> >
{
public:
  typedef vcg::tri::TriEdgeCollapseQuadric<Mesh_t, VertexPair_t, TriEdgeCollapse_t, QInfoStandard<Vertex_t> > TECQ;

  inline
  TriEdgeCollapse_t(const VertexPair_t &p,
                    int i,
                    vcg::BaseParameterClass *pp) : TECQ(p, i, pp)
  {}
};

typedef vcg::tri::TriEdgeCollapseQuadricParameter TriEdgeColQuadParam_t;

#endif

inline
void decimate(Mesh_t &mesh,
              bool cleaningFlag,
              double percentSize,
              double targetError,
              double maxDecimTime,
              Timer &timer,
              TriEdgeColQuadParam_t qparams,
              bool verbose = false)
{
  const std::string toolHeader = "VCG Decimate: ";

  timer.m_cleanup.start();

  int meshFaceNum = mesh.FN();
  float pre_area = vcg::tri::Stat<Mesh_t>::ComputeMeshArea(mesh);

  // Ensure the mesh is up to date.
  vcg::tri::UpdateBounding<Mesh_t>::Box(mesh);
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(mesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(mesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(mesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerVertexNormalizedPerFace(mesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerFace(mesh);


  if (cleaningFlag)
  {
    INFO(toolHeader << "Cleaning...");
    int manf_face = vcg::tri::Clean<Mesh_t>::RemoveNonManifoldFace(mesh);
    VRB(toolHeader << "Removed " << manf_face << " folded faces");

    int manf_flip = vcg::tri::Clean<Mesh_t>::RemoveFaceFoldByFlip(mesh);
    VRB(toolHeader << "Removed " << manf_flip << " flipped faces");

    VRB(toolHeader << "Finished Cleaning:");
    VRB(toolHeader << " Mesh vertices: " << mesh.VN());
    VRB(toolHeader << " Mesh faces:    " << mesh.FN() << std::endl);
  }
  timer.m_cleanup.stop();
  timer.m_bbox.start();

  vcg::tri::UpdateBounding<Mesh_t>::Box(mesh);
  timer.m_bbox.stop();
  timer.m_curvature.start();

  // Compute Curvature and Decimate
  int finalSize = (int) (meshFaceNum * percentSize);
  if (finalSize < meshFaceNum)
  {
    // Compute Curvature
    // TODO: Don't just compute curvature, we could also compute occlusion using ray-tracing.
    INFO(toolHeader << "Computing Curvature...");

    int manifold_edges_ff = vcg::tri::Clean<Mesh_t>::CountNonManifoldEdgeFF(mesh);
    int manifold_vertex_ff = vcg::tri::Clean<Mesh_t>::CountNonManifoldVertexFF(mesh);
    // VRB(toolHeader << "manifold_edges_ff = " << manifold_edges_ff);
    // VRB(toolHeader << "manifold_vertex_ff = " << manifold_vertex_ff);
    if (manifold_edges_ff > 0)
    {
      WRN("Mesh has some non 2-manifold faces, Curvature computation requires manifoldness");
    }
    else
    {
      vcg::tri::Allocator<Mesh_t>::CompactEveryVector(mesh);
      vcg::tri::UpdateCurvature<Mesh_t>::MeanAndGaussian(mesh);

      // Computed ABS Curvature
      vcg::tri::UpdateQuality<Mesh_t>::VertexFromAbsoluteCurvature(mesh);

      // TODO: Don't compute a histogram, it's slow.
      // Just compute the min and max, then normalize the quality using:
      // "vcg::tri::Stat<Mesh_t>::ComputePerVertexQualityMinMax(mesh, &min, &max)"
      // and...
      // "vcg::tri::UpdateQuality<Mesh_t>::VertexNormalize(mesh, min, max)
      vcg::Histogramf H;
      vcg::tri::Stat<Mesh_t>::ComputePerVertexQualityHistogram(mesh, H);
      VRB(toolHeader << "Curvature Range:");
      VRB(toolHeader << " Min:             " << H.MinV());
      VRB(toolHeader << " Max:             " << H.MaxV());
      VRB(toolHeader << " 10th Percentile: " << H.Percentile(0.1f));
      VRB(toolHeader << " 50th Percentile: " << H.Percentile(0.5f));
      VRB(toolHeader << " 90th Percentile: " << H.Percentile(0.9f) << std::endl);
    }

    // compute final face count
    finalSize = (int) (mesh.fn * percentSize);
    INFO(toolHeader << "Decimating...");
    VRB(toolHeader << "Target face number: " << finalSize);

    // decimator initialisation
    timer.m_curvature.stop();
    timer.m_deciInit.start();

    vcg::LocalOptimization<Mesh_t> DeciSession(mesh, &qparams);
#ifdef WITH_TEXTURE_COORDS
    vcg::math::Quadric<double> QZero;
    QZero.SetZero();
    QuadricTexHelper<Mesh_t>::QuadricTemp TD3(mesh.vert, QZero);
    QuadricTexHelper<Mesh_t>::TDp3() = &TD3;

    std::vector<std::pair<vcg::TexCoord2<float>, vcg::Quadric5<double> > > qv;

    QuadricTexHelper<Mesh_t>::Quadric5Temp TD(mesh.vert, qv);
    QuadricTexHelper<Mesh_t>::TDp() = &TD;

    DeciSession.Init<TriEdgeCollapseTex_t>();
#else
    DeciSession.Init<TriEdgeCollapse_t>();
#endif

    VRB(toolHeader << "Initial Heap Size " << DeciSession.h.size());

    timer.m_deciInit.stop();
    timer.m_decimation.start();

    // decimation loop
    std::clock_t before_time = clock();
    double updateTime = 1.0; // 1 seconds
    int before_faces = int(mesh.FN());
    DeciSession.SetTargetSimplices(finalSize);
    DeciSession.SetTimeBudget(0.5f);
    if (targetError < std::numeric_limits<float>::max())
    {
      DeciSession.SetTargetMetric(targetError);
    }
    float percent = 100.0f;
    while (DeciSession.DoOptimization() &&
           (mesh.FN() > finalSize) &&
           (DeciSession.currMetric < targetError) &&
           (((clock() - before_time) / (double) CLOCKS_PER_SEC) < maxDecimTime))
    {
      if (((clock() - before_time) / (double) CLOCKS_PER_SEC) > updateTime)
      {
        percent = (float(mesh.FN()) / float(before_faces)) * 100.0f;
        INFO(toolHeader
                 << "Progress: "
                 << percent
                 << "\tMesh Faces: " << mesh.FN());
        before_time = clock();
      }
    }
    percent = (float(mesh.FN()) / float(before_faces)) * 100.0f;
    INFO(toolHeader
             << "Progress: "
             << percent
             << "\tMesh Faces: " << mesh.FN());

    // Print decimation status
    float post_area = vcg::tri::Stat<Mesh_t>::ComputeMeshArea(mesh);
    INFO(toolHeader << "Decimation finished!");
    VRB(toolHeader << " Mesh vertices:  " << mesh.VN());
    VRB(toolHeader << " Mesh faces:     " << mesh.FN());
    VRB(toolHeader << " Mesh error:     " << DeciSession.currMetric);
    VRB(toolHeader << " Mesh area:      " << post_area);
    VRB(toolHeader << " Mesh area diff: " << std::fabs(pre_area - post_area) << std::endl);
    timer.m_decimation.stop();
  }
  else
  {
    timer.m_curvature.stop();
    timer.m_deciInit.stop();
    timer.m_decimation.stop();
    WRN("Skipping Decimation, percentage given will do nothing");
  }

  timer.m_postCleanup.start();
  if (cleaningFlag)
  {
    INFO(toolHeader << "Cleaning...");

    int dup_vert = vcg::tri::Clean<Mesh_t>::RemoveDuplicateVertex(mesh);
    VRB(toolHeader << "Removed " << dup_vert << " duplicate vertices");

    int unref_vert = vcg::tri::Clean<Mesh_t>::RemoveUnreferencedVertex(mesh);
    VRB(toolHeader << "Removed " << unref_vert << " unreferenced vertices");

    int dup_face = vcg::tri::Clean<Mesh_t>::RemoveDuplicateFace(mesh);
    VRB(toolHeader << "Removed " << dup_face << " duplicate faces");

    float post_clean_area = vcg::tri::Stat<Mesh_t>::ComputeMeshArea(mesh);
    VRB(toolHeader << "Finished Cleaning:");
    VRB(toolHeader << " Mesh vertices:  " << mesh.VN());
    VRB(toolHeader << " Mesh faces:     " << mesh.FN());
    VRB(toolHeader << " Mesh area:      " << post_clean_area);
    VRB(toolHeader << " Mesh area diff: " << std::fabs(pre_area - post_clean_area));
  }
  timer.m_postCleanup.stop();
}

#endif // VCG_NODES_DECIMATE_UTILS_H
