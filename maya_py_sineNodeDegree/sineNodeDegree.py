#-
# ==========================================================================
# Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
# rights reserved.
#
# The coded instructions, statements, computer programs, and/or related 
# material (collectively the "Data") in these files contain unpublished 
# information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
# licensors, which is protected by U.S. and Canadian federal copyright 
# law and by international treaties.
#
# The Data is provided for use exclusively by You. You have the right 
# to use, modify, and incorporate this Data into other products for 
# purposes authorized by the Autodesk software license agreement, 
# without fee.
#
# The copyright notices in the Software and this entire statement, 
# including the above license grant, this restriction and the 
# following disclaimer, must be included in all copies of the 
# Software, in whole or in part, and all derivative works of 
# the Software, unless such copies or derivative works are solely 
# in the form of machine-executable object code generated by a 
# source language processor.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
# AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
# WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
# NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
# PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
# TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
# BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
# DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
# AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
# OR PROBABILITY OF SUCH DAMAGES.
#
# ==========================================================================
#+

# http://me.autodesk.jp/wam/maya/docs/Maya2009/API/sine_node_8py-example.html 
# change unit from Radians to Degrees

# import maya
# maya.cmds.loadPlugin("sineNodeDegree.py")
# maya.cmds.createNode("spSineNodeDegree")
# maya.cmds.getAttr("spSineNodeDegree1.output")

import math, sys

import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx

kPluginNodeTypeName = "spSineNodeDegree"
# unique type identify for custom node, 
# 0x00000000 to 0x0007FFFF are reserved by Autodesk.   
sineNodeId = OpenMaya.MTypeId(0x87001)

# Node definition
class sineNode(OpenMayaMPx.MPxNode): 
        # class variables
        input = OpenMaya.MObject()
        output = OpenMaya.MObject()
        def __init__(self):
                OpenMayaMPx.MPxNode.__init__(self)
				
		# the brains of any dependency node, called
		# when data are requested for a particular plug. 
        def compute(self,plug,dataBlock):
				# route execution based on which plug is requested.
                if ( plug == sineNode.output ): 
						# to retrieve the values of input attributes. 
                        dataHandle = dataBlock.inputValue( sineNode.input )
                        # same as dataHandle = OpenMaya.MDataHandle( dataBlock.inputValue( sineNode.input ) ) 
						
                        inputFloat = dataHandle.asFloat()
                        result = math.sin( inputFloat / 180 * 3.1415926 ) 
						
                        outputHandle = dataBlock.outputValue( sineNode.output )
                        outputHandle.setFloat( result )
                        dataBlock.setClean( plug ) 

                return OpenMaya.kUnknownParameter

# creator
def nodeCreator():
        return OpenMayaMPx.asMPxPtr( sineNode() )

# initializer
def nodeInitializer():
        # input
        nAttr = OpenMaya.MFnNumericAttribute()
        sineNode.input = nAttr.create( "input", "in", OpenMaya.MFnNumericData.kFloat, 0.0 )
        nAttr.setStorable(1)
        # output
        nAttr = OpenMaya.MFnNumericAttribute()
        sineNode.output = nAttr.create( "output", "out", OpenMaya.MFnNumericData.kFloat, 0.0 )
        nAttr.setStorable(1)
        nAttr.setWritable(1)
        # add attributes
        sineNode.addAttribute( sineNode.input )
        sineNode.addAttribute( sineNode.output )
        sineNode.attributeAffects( sineNode.input, sineNode.output )
        
# initialize the script plug-in
def initializePlugin(mobject):
        mplugin = OpenMayaMPx.MFnPlugin(mobject)
        try:
                mplugin.registerNode( kPluginNodeTypeName, sineNodeId, nodeCreator, nodeInitializer )
        except:
                sys.stderr.write( "Failed to register node: %s" % kPluginNodeTypeName )
                raise

# uninitialize the script plug-in
def uninitializePlugin(mobject):
        mplugin = OpenMayaMPx.MFnPlugin(mobject)
        try:
                mplugin.deregisterNode( sineNodeId )
        except:
                sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeTypeName )
                raise