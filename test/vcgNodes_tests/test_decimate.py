"""
Example and test program for solver.
"""

import maya.cmds

import testUtils

NODE_TYPE = 'vcgDecimateNode'

def main():
    print 'Running:', __file__

    print 'Initialize scene...'
    testUtils.reinitializeScene()

    # Ensure it's loaded.
    if not testUtils.hasNodeTypes([NODE_TYPE]):
        assert False

    sph = maya.cmds.polySphere()

    node = maya.cmds.vcgDecimate(sph[0], percent=20)
    print 'node:', node
    if not node:
        maya.cmds.error('vcgDecimate failed to give a node name')
        return False
    node = ['vcgDecimateNode1']
    maya.cmds.select(sph[0])

    newTransform = maya.cmds.createNode('transform')
    newMesh = maya.cmds.createNode('mesh', parent=newTransform)
    maya.cmds.connectAttr(node[0]+'.outMesh', newMesh+'.inMesh')
    maya.cmds.setAttr(newTransform+'.translateX', 2.0)
    maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

    # maya.cmds.viewFit()

    # Export
    print 'Exporting...'
    path = "/home/davidc/dev/mayaVcgNodes/test/mayaObjExport.ma"
    maya.cmds.select(sph[0], replace=True)
    print maya.cmds.file(path,
                         exportSelected=True,
                         type='mayaAscii',
                         constructionHistory=False,
                         force=True)
    print '=' * 100
    return True
