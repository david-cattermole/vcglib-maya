"""
Interface to run all tests for VCG Nodes.
"""

import unittest

try:
    import maya.standalone
    maya.standalone.initialize(name='python')
    import maya.cmds
except:
    pass


# import modules so that 'unittest' module can find them.
# from vcgNodes_tests.test_readFile import ReadFileTestCase
# from vcgNodes_tests.test_writeFile import WriteFileTestCase
from vcgNodes_tests.test_meshCutter import MeshCutterTestCase
from vcgNodes_tests.test_decimate import DecimateTestCase
from vcgNodes_tests.test_meshStats import MeshStatsTestCase

def run():
    unittest.main()

if __name__ == '__main__':
    run()
    maya.cmds.quit(force=True)
