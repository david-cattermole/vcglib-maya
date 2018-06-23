"""
Example and test program for write file node.
"""

import os
import maya.cmds
import testUtils

NODE_TYPE = 'vcgWriteFileNode'

class WriteFileTestCase(testUtils.BaseTestCase):

    def runTest(self):
        """
        Write File node test.
        """
        dataDir = self.getDataRoot()
        self.assertTrue(self.hasNodeTypes([NODE_TYPE]))

        polyCubeNode = maya.cmds.createNode('polyCube')
        readFileNode = maya.cmds.createNode('vcgReadFileNode')
        writeFileNode = maya.cmds.createNode('vcgWriteFileNode')

        meshNode = maya.cmds.createNode('mesh')
        maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

        maya.cmds.setAttr(polyCubeNode + '.subdivisionsWidth', 9)
        maya.cmds.setAttr(polyCubeNode + '.subdivisionsHeight', 9)
        maya.cmds.setAttr(polyCubeNode + '.subdivisionsDepth', 9)
        maya.cmds.setAttr(readFileNode + '.verbose', 1)
        maya.cmds.setAttr(writeFileNode + '.verbose', 1)
        maya.cmds.setAttr(writeFileNode + '.cleanMesh', 0)

        maya.cmds.connectAttr(polyCubeNode + '.output', meshNode + '.inMesh')
        maya.cmds.connectAttr(meshNode + '.outMesh', writeFileNode + '.inMesh')

        inputPathNodeAttr = readFileNode + '.filePath'
        outputPathNodeAttr = writeFileNode + '.filePath'
        outNodeAttr = writeFileNode + '.outMesh'

        # save cube
        name = 'testPolyCube'
        outputPath = self.getTempFilePath(NODE_TYPE, name, 'obj')
        maya.cmds.setAttr(outputPathNodeAttr, outputPath, type='string')
        maya.cmds.dgeval(outNodeAttr, verbose=True)
        self.saveMayaAsciiFile(NODE_TYPE, name)

        # read / write all found .obj and .ply files.

        maya.cmds.disconnectAttr(polyCubeNode + '.output', meshNode + '.inMesh')
        maya.cmds.connectAttr(polyCubeNode + '.output', readFileNode + '.inMesh')
        maya.cmds.connectAttr(readFileNode + '.outMesh', meshNode + '.inMesh')

        inputFiles = os.listdir(dataDir) or []
        for inputFile in sorted(inputFiles):
            split = os.path.splitext(inputFile)
            name = split[0]
            if split[-1] in ['.obj', '.ply']:
                inputPath = os.path.join(dataDir, inputFile)
                maya.cmds.setAttr(inputPathNodeAttr, inputPath, type='string')

                outputPath = self.getTempFilePath(NODE_TYPE, name, 'obj')
                print 'Writing:', outputPath
                maya.cmds.setAttr(outputPathNodeAttr, outputPath, type='string')
                maya.cmds.dgeval(outNodeAttr, verbose=True)
                self.assertTrue(os.path.isfile(outputPath))

                self.saveMayaAsciiFile(NODE_TYPE, name)
        return
