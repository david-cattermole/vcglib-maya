"""
Example and test program for solver.
"""

import os
import maya.cmds
import testUtils

NODE_TYPE = 'vcgReadFileNode'
DIR = '/home/davidc/dev/mayaVcgNodes/data/'


def saveFile(name):
    path = '/home/davidc/dev/mayaVcgNodes/test/vcgReadFileNode_%s_export.ma'
    path = path % name
    print 'Save...', path
    maya.cmds.file(rename=path)
    maya.cmds.file(save=True,
                   type='mayaAscii',
                   constructionHistory=True,
                   force=True)
    return


def main():
    print 'Running:', __file__

    print 'Initialize scene...'
    testUtils.reinitializeScene()

    # Ensure it's loaded.
    if not testUtils.hasNodeTypes([NODE_TYPE]):
        assert False

    polyCubeNode = maya.cmds.createNode('polyCube')
    readFileNode = maya.cmds.createNode('vcgReadFileNode')
    # decimateNode = maya.cmds.createNode('vcgDecimateNode')
    meshNode = maya.cmds.createNode('mesh')
    maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

    maya.cmds.connectAttr(polyCubeNode + '.output', readFileNode + '.inMesh')
    maya.cmds.connectAttr(readFileNode + '.outMesh', meshNode + '.inMesh')
    nodeAttr = readFileNode + '.filePath'

    if os.path.isdir(DIR):
        inputFiles = os.listdir(DIR) or []
        for inputFile in sorted(inputFiles):
            split = os.path.splitext(inputFile)
            name = split[0]
            if split[-1] in ['.obj', '.ply']:
                inputPath = os.path.join(DIR, inputFile)
                maya.cmds.setAttr(nodeAttr, inputPath, type='string')
                saveFile(name)
    else:
        print 'Invalid directory!', DIR
    print '=' * 100
    return True
