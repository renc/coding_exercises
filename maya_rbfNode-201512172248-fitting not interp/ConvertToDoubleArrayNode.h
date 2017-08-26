//
// Copyright (C) 
// 
// File: ConvertToDoubleArrayNode.h
//
// Dependency Graph Node: ConvertToDoubleArrayNode
//
// Author: Maya Plug-in Wizard 2.0
//
#pragma once

//- Include Maya necessary headers for our class
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnStringData.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <vector> 

// ConvertToDoubleArrayNode
// 
// A helper node to conver the input to output as doubleArray data type. 
// Input: int/int2/int3, ...  

//- Derive a new class from the default Maya node proxy class.
class ConvertToDoubleArrayNode : public MPxNode
{
public:
	ConvertToDoubleArrayNode() {
		//- Never do anything here which would affect Maya.
		//- Instead implement the virtual postConstructor() method.
	}
	virtual ~ConvertToDoubleArrayNode() {}

	//- Declare the virtual compute() method.
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	//- Declare/implement the static creator method.
	//- This method exists to give Maya a way to create new objects
	//- of this type. 
	static  void*		creator() {
		return new ConvertToDoubleArrayNode();
	}

	//- Declare the static initialize() method.
	static  MStatus		initialize();

public:
	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	 
	static MObject aIn3Double;  
	static MObject aInDoublePackage; static MObject aInDoubleChild;
	static MObject aInDoubleArray;
	static MObject aOutDoubleArray; 

	//static MObject aInDataType; // to choose 3double, or double array, or ..., as input. 

	//- This is a unique node ID for your new node class. It is declared
	//- static because it is common to all instance of that class.
	//-
	//- The typeid is a unique 32bit identifier that describes this node.
	//- It is used to save and retrieve nodes of this type from the binary
	//- file format. If it is not unique, it will cause file IO problems.
	static	MTypeId		id;
};

