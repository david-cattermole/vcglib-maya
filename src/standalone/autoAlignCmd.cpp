/*
 *
 */

// Local
#include <other/autoAlignUtils.h>
#include <other/loadModelUtils.h>

// VCG
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>


// forward declaration.
void printUsage();
int loadModel(const char *filePath, Mesh_t &mesh, bool verbose = false);


// Main executable.
int main(int argc, char **argv)
{
  if (argc < 4)
  {
    printUsage();
  }

  // TriEdgeColQuadParam_t qparams;

  // defaults
  bool verbose = false;
  float radius = 0.001f;
  float featureRatio = 0.5f;
  int maxIterations = 1000;
  int maxFeatures = 500;

  // TODO: Parse command line options.

  Timer timer;
  timer.m_total.start();

  // Load Mesh
  Mesh_t refMesh;
  Mesh_t srcMesh;
  timer.m_meshOpen.start();

  int stat = 0;
  int mask = vcg::tri::io::Mask::IOM_ALL;
  stat = loadModel(argv[1], refMesh, mask, verbose);
  if (stat == EXIT_FAILURE) {return stat;}

  stat = loadModel(argv[2], srcMesh, mask, verbose);
  if (stat == EXIT_FAILURE) {return stat;}

  timer.m_meshOpen.stop();
  timer.m_cleanup.start();

  // Compute...
  autoAlign(
      refMesh,
      srcMesh,
      radius,
      featureRatio,
      maxIterations,
      maxFeatures,
      timer,
      verbose);

  // Export
  timer.m_meshSave.start();
  INFO("Saving Mesh: " << argv[3] << std::endl);
  vcg::tri::io::Exporter<Mesh_t>::Save(srcMesh, argv[3], mask);
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
  INFO("Done!");
  return EXIT_SUCCESS;
}


//int loadModel(const char *filePath, Mesh_t &mesh, bool verbose)
//{
//  INFO("Loading Mesh: " << filePath);
//  int err = vcg::tri::io::Importer<Mesh_t>::Open(mesh, filePath);
//  if (vcg::tri::io::Importer<Mesh_t>::ErrorCritical(err))
//  {
//    ERR("Unable to open mesh "
//            << "'" << filePath << "' "
//            << " : '" << vcg::tri::io::Importer<Mesh_t>::ErrorMsg(err) << "'");
//    return EXIT_FAILURE;
//  }
//  VRB(" Mesh vertices (vn): " << mesh.VN());
//  VRB(" Mesh faces (fn):    " << mesh.FN());
//  return EXIT_SUCCESS;
//}

void printUsage()
{
  std::cout
      << "Auto Align 0.1" << std::endl
      << __DATE__ << std::endl
      << std::endl
      << "Usage: autoalign [file in] [file out] [percent] [arg]" << std::endl
      << "Options and arguments:" << std::endl
      << "     --verbose                      Print out details of program (default no)" << std::endl
      << "     --cleanup [0|1]                Remove duplicate & unreferenced vertices (default yes)" << std::endl
      << "     --overlap [num]                Estimate percentage of overlapping points" << std::endl
      << "     --samples [num]                Number of samples" << std::endl
      << "     --radius [num]                 Distance of sampling radius " << std::endl
      << "     --delta-percent [num]          Approximation level as percentage" << std::endl
      << "     --delta [num]                  Approximation level as absolute value" << std::endl
      << "     --feet-size [num]              Number of Point around feature point" << std::endl
      << "     --score-feet [num]             Number of needed matching points" << std::endl
      << "     --angle [num]                  Maximum angle between matching points" << std::endl
      << "     --seed [num]                   Random seed value" << std::endl
      << std::endl;
  exit(-1);
}
