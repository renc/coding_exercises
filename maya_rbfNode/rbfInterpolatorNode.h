//
// Copyright (C) 
// 
// File: rbfInterpolatorNode.h
//
// Dependency Graph Node: rbfInterpolatorNode
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

//- Derive a new class from the default Maya node proxy class.
class rbfInterpolatorNode : public MPxNode
{
public:
	rbfInterpolatorNode() {
		//- Never do anything here which would affect Maya.
		//- Instead implement the virtual postConstructor() method.
	}
	virtual ~rbfInterpolatorNode() {}

	//- Declare the virtual compute() method.
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	//- Declare/implement the static creator method.
	//- This method exists to give Maya a way to create new objects
	//- of this type. 
	static  void*		creator() {
		return new rbfInterpolatorNode();
	}

	//- Declare the static initialize() method.
	static  MStatus		initialize();

public:
	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	static  MObject		input;		// input attribute
	static  MObject		output;		// output attribute
	static	MObject		descString; // Description string attribute describing the function of this node

	static MObject aExamples; // attr examples
	static MObject aExamplePoint;
	static MObject aExampleValue;

	static MObject aCoefficients;

	static MObject aPointX;
	static MObject aValueX; 


	//- This is a unique node ID for your new node class. It is declared
	//- static because it is common to all instance of that class.
	//-
	//- The typeid is a unique 32bit identifier that describes this node.
	//- It is used to save and retrieve nodes of this type from the binary
	//- file format. If it is not unique, it will cause file IO problems.
	static	MTypeId		id;
};

struct Example
{
	// an example is a sample data (point, value). 
	// 

	Example() : value(0.0) { point.clear(); }

	std::vector<double> point;
	double value; 
}; 

double ExampleDistance(const Example &e1, const Example &e2); 
double GaussianBasisFunction(double value, double sigma);