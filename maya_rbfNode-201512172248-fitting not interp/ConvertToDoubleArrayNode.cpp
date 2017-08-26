//
// Copyright (C) 
// 
// File: ConvertToDoubleArrayNode.cpp
//
// Dependency Graph Node: ConvertToDoubleArrayNode
//
// Author: Maya Plug-in Wizard 2.0
//

#include "ConvertToDoubleArrayNode.h"
#include <maya/MGlobal.h>
#include <maya/MFnCompoundAttribute.h> //
#include "matrix.h"

//- Assigning a unique node ID to your new node class.
//- Ask ADN or Autodesk product support to reserve IDs for your company. You can
//- reserve ID by block of 64, 128, 256, or 512 consecutive ID.
//-
//- 0x00001 is a temporary ID for reserved for development. Never use that ID in a
//- production environment.
/*static*/ MTypeId ConvertToDoubleArrayNode::id( 0x00002 );

//- Instantiate the static attributes of your node class.
MObject ConvertToDoubleArrayNode::aIn3Double;  
MObject ConvertToDoubleArrayNode::aInDoublePackage;
MObject ConvertToDoubleArrayNode::aInDoubleChild;
MObject ConvertToDoubleArrayNode::aInDoubleArray;
MObject ConvertToDoubleArrayNode::aOutDoubleArray;  

//- The initialize method is called to create and initialize all of the 
//- attributes and attribute dependencies for this node type. This is 
//- only called once when the node type is registered with Maya.
//- Return Values: MS::kSuccess / MS::kFailure
//-
MStatus ConvertToDoubleArrayNode::initialize()
{
	//- Initialize a float input attribute using the MFnNumericAttribute
	//- class. Make that attribute definition saved into Maya file (setStorable),
	//- and selectable in the channel box (setKeyable).

	//- Create a generic attribute using MFnNumericAttribute
	MFnNumericAttribute nAttr;
	MFnCompoundAttribute cAttr;
	MFnTypedAttribute tAttr; tAttr;
  
	aIn3Double = nAttr.create( "in3double", "in3double", MFnNumericData::k3Double);
	//- Attribute will be written to files when this type of node is stored
 	nAttr.setStorable(true);
	nAttr.setStorable(false);
	
	aInDoubleArray = tAttr.create("inDoubleArray", "inDoubleArray", MFnData::kDoubleArray);
	
	aInDoubleChild = nAttr.create( "child", "child", MFnNumericData::kDouble, 0.0);
	nAttr.setArray(true);
	aInDoublePackage = cAttr.create("inDoublePackage", "idp");
	cAttr.addChild( aInDoubleChild ); 
	
	aOutDoubleArray = tAttr.create("outDoubleArray", "outDoubleArray", MFnData::kDoubleArray);
	
	addAttribute(aIn3Double);
	addAttribute(aInDoubleArray);
	addAttribute(aInDoublePackage);
	addAttribute(aOutDoubleArray); 
	
	//- Finally tell Maya how the information should flow through your node.
	//- This will also tell Maya how the dirty flag is propagated in your node
	//- and ultimately in the Maya DG. To do this, use the attributeAffects()
	//- method to link your node' attributes together.

	//- Set up a dependency between the input and the output. This will cause
	//- the output to be marked dirty when the input changes. The output will
	//- then be recomputed the next time the value of the output is requested.
	attributeAffects( aIn3Double, aOutDoubleArray );
	attributeAffects( aInDoubleArray, aOutDoubleArray );
	attributeAffects( aInDoublePackage, aOutDoubleArray );
	
	  
	//- Return success to Maya
	return MS::kSuccess;
}
 

//- This method computes the value of the given output plug based
//- on the values of the input attributes.
//- Arguments:
//- 	plug - the plug to compute
//- 	data - object that provides access to the attributes for this node
MStatus ConvertToDoubleArrayNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus returnStatus;
	MStatus status; 

	//- Check which output attribute we have been asked to compute. If this 
	//- node doesn't know how to compute it, you must return MS::kUnknownParameter.

	if( plug == aOutDoubleArray )
	{
		//- Get a handle to the input attribute that we will need for the
		//- computation. If the value is being supplied via a connection 
		//- in the dependency graph, then this call will cause all upstream  
		//- connections to be evaluated so that the correct value is supplied.
		MDataHandle inputData = data.inputValue( aIn3Double, &status );

		//- Read the input value from the handle.
		double3 & dataIn3Double = inputData.asDouble3();
				
		MDoubleArray tmpDoubleArray;
		tmpDoubleArray.setLength(3);
		tmpDoubleArray.set(dataIn3Double[0], 0);
		tmpDoubleArray.set(dataIn3Double[1], 1);
		tmpDoubleArray.set(dataIn3Double[2], 2);
				
		MFnDoubleArrayData fnDoubleDataTrans;
		MObject oDoubleData = fnDoubleDataTrans.create(tmpDoubleArray, &status); 
		CHECK_MSTATUS_AND_RETURN_IT(status);

		//- Get a handle to the output attribute. This is similar to the
		//- "inputValue" call above except that no dependency graph 
		//- computation will be done as a result of this call.

		//- Get a handle on the aOutput attribute
		MDataHandle outputHandle = data.outputValue( ConvertToDoubleArrayNode::aOutDoubleArray );
		outputHandle.set(oDoubleData);

		//- Mark the destination plug as being clean. This will prevent the
		//- dependency graph from repeating this calculation until an input 
		//- attribute of this node which affects this output attribute changes.

		//- Tell Maya the plug is now clean
		data.setClean(plug);

		//- Return success to Maya
		return MS::kSuccess;
	} 

	//- Tell Maya that we do not know how to handle this plug, but let give a chance
	//- to our parent class to evaluate it.
	return MS::kUnknownParameter;
} 