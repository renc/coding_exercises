//
// Copyright (C)  
// 
// File: pluginMain.cpp
//
// Author: Maya Plug-in Wizard 2.0
//

#include <maya/MFnPlugin.h>
#include "rbfInterpolatorNode.h"
#include "ConvertToDoubleArrayNode.h"

MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "2009", "Any");

	// Add plug-in feature registration here
	//

	//- Register your node here, so Maya can use it and recognize it
	//- while reading/saving a file which has instance of this node type.
	status = plugin.registerNode( "rbfInterpolatorNode", rbfInterpolatorNode::id, rbfInterpolatorNode::creator, rbfInterpolatorNode::initialize );
	
	status = plugin.registerNode( "ConvertToDoubleArrayNode", ConvertToDoubleArrayNode::id, ConvertToDoubleArrayNode::creator, ConvertToDoubleArrayNode::initialize );

	if (!status) {
		status.perror("registerNode");
		return status;
	}
	return status;
}

MStatus uninitializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	// Add plug-in feature deregistration here
	//

	//- Unregister your node here, so Maya stops using it.
	status = plugin.deregisterNode( rbfInterpolatorNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	status = plugin.deregisterNode( ConvertToDoubleArrayNode::id );

	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
