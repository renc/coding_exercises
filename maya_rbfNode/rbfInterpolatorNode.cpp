//
// Copyright (C) 
// 
// File: rbfInterpolatorNode.cpp
//
// Dependency Graph Node: rbfInterpolatorNode
//
// Author: Maya Plug-in Wizard 2.0
//

#include "rbfInterpolatorNode.h"
#include <maya/MGlobal.h>
#include <maya/MFnCompoundAttribute.h> //
#include "matrix.h"

//- Assigning a unique node ID to your new node class.
//- Ask ADN or Autodesk product support to reserve IDs for your company. You can
//- reserve ID by block of 64, 128, 256, or 512 consecutive ID.
//-
//- 0x00001 is a temporary ID for reserved for development. Never use that ID in a
//- production environment.
/*static*/ MTypeId rbfInterpolatorNode::id( 0x00001 );

//- Instantiate the static attributes of your node class.
/*static*/ MObject rbfInterpolatorNode::input;        
/*static*/ MObject rbfInterpolatorNode::output;   
/*static*/ MObject rbfInterpolatorNode::descString;		 

MObject rbfInterpolatorNode::aExamples;
MObject rbfInterpolatorNode::aExamplePoint;
MObject rbfInterpolatorNode::aExampleValue;
MObject rbfInterpolatorNode::aCoefficients;
MObject rbfInterpolatorNode::aPointX;
MObject rbfInterpolatorNode::aValueX; 

//- The initialize method is called to create and initialize all of the 
//- attributes and attribute dependencies for this node type. This is 
//- only called once when the node type is registered with Maya.
//- Return Values: MS::kSuccess / MS::kFailure
//-
MStatus rbfInterpolatorNode::initialize()
{
	//- Initialize a float input attribute using the MFnNumericAttribute
	//- class. Make that attribute definition saved into Maya file (setStorable),
	//- and selectable in the channel box (setKeyable).

	//- Create a generic attribute using MFnNumericAttribute
	MFnNumericAttribute nAttr;
	input = nAttr.create( "input", "in", MFnNumericData::kFloat, 0.0 );
	//- Attribute will be written to files when this type of node is stored
 	nAttr.setStorable(true);
	//- Attribute is keyable and will show up in the channel box
 	nAttr.setKeyable(true);

	//- Initialize a float output attribute using the MFnNumericAttribute
	//- class. Make that attribute definition not saved into Maya file.
	output = nAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	//- Attribute will not be written to files when this type of node is stored
	nAttr.setStorable(false);
	
	//- Initialize a string output attribute using the MFnTypedAttribute
	//- class.In order to specify the attribute default value, you will need to 
	//- use the MFnStringData class, before creating the attribute itself.

	//- Create a string attribute using MFnTypedAttribute
	MFnTypedAttribute typedAttr;
	//- Use MFnStringData to implement default value of this attribute
	MFnStringData fnStringData;
	MString defaultString("description string for current node");
	MObject defaultStringObj = fnStringData.create(defaultString);
	descString = typedAttr.create("descString", "dStr", MFnData::kString,defaultStringObj);

	//- Now add the attribute to your node definition using the addAttribute()
	//- method.
	//- Add the attributes we have created to the node
	addAttribute( input );	
	addAttribute( output );
	addAttribute(descString);
	
	//- Finally tell Maya how the information should flow through your node.
	//- This will also tell Maya how the dirty flag is propagated in your node
	//- and ultimately in the Maya DG. To do this, use the attributeAffects()
	//- method to link your node' attributes together.

	//- Set up a dependency between the input and the output. This will cause
	//- the output to be marked dirty when the input changes. The output will
	//- then be recomputed the next time the value of the output is requested.
	attributeAffects( input, output );

	
	MFnCompoundAttribute cAttr;
	MFnTypedAttribute tAttr; tAttr;
  
	// renc: i hope to use doubleArray which lenght can be changed. however donot how to connect
	//aExamplePoint = tAttr.create("examplePoint", "examplePoint", MFnData::kDoubleArray);
	aExamplePoint = nAttr.create("examplePoint", "examplePoint", MFnNumericData::k3Double);
	aExampleValue = nAttr.create("exampleValue", "exampleValue", MFnNumericData::kDouble, 0.0);
	aExamples = cAttr.create("examples", "examples");
	cAttr.setArray(true); 
	cAttr.addChild( aExamplePoint );
	cAttr.addChild( aExampleValue ); 
	
	aCoefficients = tAttr.create("coefficient", "coefficient", MFnData::kDoubleArray);
	tAttr.setArray(true);
	
	//aPointX = tAttr.create("pointX", "pointX", MFnData::kDoubleArray);
	aPointX = nAttr.create("pointX", "pointX", MFnNumericData::k3Double);
	tAttr.setArray(true);
	aValueX = nAttr.create("valueX", "valueX", MFnNumericData::kDouble, 0.0);

	addAttribute( aExamples );
	addAttribute( aExamplePoint );
	addAttribute( aExampleValue );
	addAttribute( aCoefficients );
	addAttribute( aPointX ); 
	addAttribute( aValueX ); 

	attributeAffects( aExamples, aValueX );
	attributeAffects( aExamplePoint, aValueX );
	attributeAffects( aExampleValue, aValueX );
	attributeAffects( aPointX, aValueX );

	//- Return success to Maya
	return MS::kSuccess;
}

// copied 
Matrix PseudoInverse(const Matrix &a, double epsilon = std::numeric_limits<double>::epsilon()) {
  Eigen::JacobiSVD<Matrix> svd(a ,Eigen::ComputeThinU | Eigen::ComputeThinV);
  double tolerance = epsilon * std::max(a.cols(), a.rows()) *svd.singularValues().array().abs()(0);
  return svd.matrixV() * (svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0).matrix().asDiagonal() * svd.matrixU().adjoint();
}


//- This method computes the value of the given output plug based
//- on the values of the input attributes.
//- Arguments:
//- 	plug - the plug to compute
//- 	data - object that provides access to the attributes for this node
MStatus rbfInterpolatorNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus returnStatus;
	MStatus status; 

	//- Check which output attribute we have been asked to compute. If this 
	//- node doesn't know how to compute it, you must return MS::kUnknownParameter.

	if( plug == output )
	{
		//- Get a handle to the input attribute that we will need for the
		//- computation. If the value is being supplied via a connection 
		//- in the dependency graph, then this call will cause all upstream  
		//- connections to be evaluated so that the correct value is supplied.
		MDataHandle inputData = data.inputValue( input, &returnStatus );

		//- Read the input value from the handle.
		float result = inputData.asFloat();

		//- Get a handle to the output attribute. This is similar to the
		//- "inputValue" call above except that no dependency graph 
		//- computation will be done as a result of this call.

		//- Get a handle on the aOutput attribute
		MDataHandle outputHandle = data.outputValue( rbfInterpolatorNode::output );

		//- Set the new output value to the handle.
		outputHandle.set( result * 2 );

		//- Mark the destination plug as being clean. This will prevent the
		//- dependency graph from repeating this calculation until an input 
		//- attribute of this node which affects this output attribute changes.

		//- Tell Maya the plug is now clean
		data.setClean(plug);

		//- Return success to Maya
		return MS::kSuccess;
	}

	if (plug == aValueX)
	{
		printf("::compute() aValueX.\n");
	
		// Get Input Examples
		MArrayDataHandle hExamples = data.inputArrayValue(aExamples); 
		unsigned int numExamples = hExamples.elementCount(); 
		
		std::vector<Example> examples;
		examples.resize(numExamples); 
		std::vector<int> exampleIndices;
		exampleIndices.resize(numExamples); 

		MDataHandle hExample, hExamplePoint, hExampleValue; 
		for (unsigned int i = 0; i < numExamples; ++i)
		{
			status = hExamples.jumpToArrayElement(i);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			exampleIndices[i] = hExamples.elementIndex(); 

			hExample = hExamples.inputValue(&status); 
			CHECK_MSTATUS_AND_RETURN_IT(status);

			hExamplePoint = hExample.child(aExamplePoint);
			//MObject oDoubleData; 
			//oDoubleData = hExamplePoint.data();
			//MFnDoubleArrayData fnDoubleDataTrans(oDoubleData, &status);
			//CHECK_MSTATUS_AND_RETURN_IT(status);
			//examples[i].point.resize(fnDoubleDataTrans.length());
			//for (unsigned int j = 0; j < fnDoubleDataTrans.length(); ++j)
			//{
			//	examples[i].point[j] = fnDoubleDataTrans[j];
			//} 
			double3& pointData = hExamplePoint.asDouble3();
			examples[i].point.resize(3);
			examples[i].point[0] = pointData[0];examples[i].point[1] = pointData[1];
			examples[i].point[2] = pointData[2];
			
			hExampleValue = hExample.child(aExampleValue);
			examples[i].value = hExampleValue.asDouble();
		} 
		
		printf("\n");
		printf("Example input: \n ");
		for (unsigned int i = 0; i < examples.size(); ++i)
		{
			printf("element Index %d: (%.2f, %.2f, %.2f), %.2f", exampleIndices[i],
				examples[i].point[0], examples[i].point[1], examples[i].point[2], 
				examples[i].value);
		} 
		printf("Count %d. \n", examples.size());
		
		// Generate the matrix
		unsigned int rows = examples.size(); 
		unsigned int cols = 1 + examples.size() ;
		Matrix m(rows, cols);
		m.setZero();
		// Add intercept params
		for (unsigned int i = 0; i < rows; ++i)
			m(i, 0) = 1.0;
		
		double maxValue = 0; 
		for (unsigned int i = 0; i < rows; ++i)
		{
			for (unsigned int j = 0; j < rows; ++j)
			{
				double distance = ExampleDistance(examples[i], examples[j]);
				if (maxValue < distance)
					maxValue = distance;
				m(i, 1+j) = distance; 
			} 
		} 
		if (maxValue > 0)
		{
			// Normalize the scalar distance 
			for (unsigned int i = 0; i < rows; ++i)
			{
				for (unsigned int j = 0; j < rows; ++j)
				{
					double distance = m(i, 1+j) / maxValue; 
					m(i, 1+j) = GaussianBasisFunction(distance, 1); 
				} 
			}
		}
		
		Matrix regularization(cols, cols);
		regularization.setZero();
		for (unsigned int i = 1; i < cols; ++i) {
			regularization(i, i) = 0.01; //lambda_;
		}
		Matrix tm = m.transpose();
		Matrix mat = PseudoInverse(tm*m + regularization)*tm;
		std::vector<Vector> thetas;
		for (unsigned int i = 0; i < rows; ++i ) {
			Vector b(rows);
			b.setZero();
			b[i] = 1.0;
			Vector theta = mat * b;
			thetas.push_back(theta);
		}

		{
			// b = w*m;
			Vector b(rows);
			for (unsigned int i = 0; i < rows; ++i)
				b[i] = examples[i].value;
			Vector w = mat * b; 

			// Get input sample X.
			MDataHandle hPointX = data.inputValue(aPointX);
			double3 &pointX = hPointX.asDouble3();
 
			Vector P(cols); 
			int ii = 0; 
			P[0] = 1.0; // Intercept value is always 1
			ii = 1; 
			for (unsigned int i = 0; i < rows; ++i)
			{
				double distance = 0;
				distance += pow( pointX[0] - examples[i].point[0], 2);
				distance += pow( pointX[1] - examples[i].point[1], 2);
				distance += pow( pointX[2] - examples[i].point[2], 2);
				distance = sqrt( distance ) / (maxValue > 0 ? maxValue : 1);
				P[ii++] = GaussianBasisFunction(distance, 1); 
			} 
			assert( w.size() == P.size() );
			double valueResult = w.dot(P); 

			MDataHandle outputHandle = data.outputValue( rbfInterpolatorNode::aValueX ); 
			outputHandle.set( valueResult ); 
			data.setClean(plug);
		}
		
		return MS::kSuccess;
	} 

	//- Tell Maya that we do not know how to handle this plug, but let give a chance
	//- to our parent class to evaluate it.
	return MS::kUnknownParameter;
}



double ExampleDistance(const Example &e1, const Example &e2)
{
	// Euclidean distance 
	double dis = 0.0;
	unsigned int pointSize = e1.point.size() < e2.point.size() ? e1.point.size() : e2.point.size(); 
	for (unsigned int i = 0; i < pointSize; ++i)
	{
		double d = e1.point[i] - e2.point[i];
		dis += d * d;
	} 
	return sqrt( dis ); 
} 

double GaussianBasisFunction(double value, double sigma)
{
	const double FALLOFF_SCALE = 0.707;  // Gives approximate value of 1.0 denominator in the gaussian rbf.
    sigma *= FALLOFF_SCALE;
	if (sigma < 1e-5)
		sigma = 1e-5;
    return exp(-(value*value)/(2.0*sigma*sigma));
} 