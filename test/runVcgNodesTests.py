"""
Interface to run all tests for rspSolver.
"""


try:
    import maya.standalone
    maya.standalone.initialize(name='python')
except:
    pass

import sys
from vcgNodes_tests import test_meshCutter
from vcgNodes_tests import test_decimate
from vcgNodes_tests import test_meshStats
from vcgNodes_tests import test_readFile

def run():
    # test_readFile.main()
    test_meshStats.main()
    # test_meshCutter.main()
    # test_decimate.main()
    return

    
if __name__ == '__main__':
    run()
    maya.cmds.quit()