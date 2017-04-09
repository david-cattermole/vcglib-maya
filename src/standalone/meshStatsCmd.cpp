/*
 *
 */

// Local
#include <other/meshStatsUtils.h>
#include <other/loadModelUtils.h>

// VCG
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>


// forward declaration.
void printUsage();
// int loadModel(const char *filePath, Mesh_t &mesh, bool verbose = false);


// Main executable.
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printUsage();
  }

  // defaults
  bool verbose = false;

  Timer timer;
  timer.m_total.start();

  // Load Mesh
  Mesh_t srcMesh;
  timer.m_meshOpen.start();
  int stat = 0;
  int mask = vcg::tri::io::Mask::IOM_ALL;
  stat = loadModel(argv[1], srcMesh, mask, verbose);
  if (stat == EXIT_FAILURE) {return stat;}
  timer.m_meshOpen.stop();
  timer.m_cleanup.start();

  // Compute...
  vcg::Point3f centreOfMass(0.0, 0.0, 0.0);
  vcg::Point3f centreOfShell(0.0, 0.0, 0.0);
  float mass = 0.0;
  float area = 0.0;
  bool waterTight = false;
  meshStats(
      srcMesh,
      centreOfMass,
      centreOfShell,
      mass,
      area,
      waterTight,
      timer,
      verbose);

  INFO("Centre of Mass: " << centreOfMass[0] << ","
                          << centreOfMass[1] << ","
                          << centreOfMass[2]);
  INFO("Centre of Shell: " << centreOfShell[0] << ","
                           << centreOfShell[1] << ","
                           << centreOfShell[2]);
  INFO("Area: " << area);
  INFO("Mass: " << mass);

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


void printUsage()
{
  std::cout
      << "Mesh Stats 0.1" << std::endl
      << __DATE__ << std::endl
      << std::endl
      << "Usage: meshStats [file in] [file out] [arg]" << std::endl
      << "Options and arguments:" << std::endl
      << "     --verbose                      Print out details of program (default no)" << std::endl
      << "     --cleanup [0|1]                Remove duplicate & unreferenced vertices (default yes)" << std::endl
      << std::endl;
  exit(-1);
}
