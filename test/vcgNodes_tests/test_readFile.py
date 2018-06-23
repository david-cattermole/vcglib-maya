"""
Example and test program for read file node.
"""

import os
import maya.cmds
import testUtils

NODE_TYPE = 'vcgReadFileNode'

class ReadFileTestCase(testUtils.BaseTestCase):

    def runTest(self):
        """
        Read File node test.
        """
        dataDir = self.getDataRoot()
        self.assertTrue(self.hasNodeTypes([NODE_TYPE]))

        polyCubeNode = maya.cmds.createNode('polyCube')
        readFileNode = maya.cmds.createNode('vcgReadFileNode')
        meshNode = maya.cmds.createNode('mesh')
        maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

        maya.cmds.setAttr(readFileNode + '.verbose', 1)

        maya.cmds.connectAttr(polyCubeNode + '.output', readFileNode + '.inMesh')
        maya.cmds.connectAttr(readFileNode + '.outMesh', meshNode + '.inMesh')
        nodeAttr = readFileNode + '.filePath'

        inputFiles = os.listdir(dataDir) or []
        for inputFile in sorted(inputFiles):
            split = os.path.splitext(inputFile)
            name = split[0]
            if split[-1] in ['.obj', '.ply']:
                inputPath = os.path.join(dataDir, inputFile)
                maya.cmds.setAttr(nodeAttr, inputPath, type='string')
                self.saveMayaAsciiFile(NODE_TYPE, name)
        return