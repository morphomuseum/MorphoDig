/*=========================================================================

Program:   Visualization Toolkit
Module:    vtkGridActor.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkGridActor.h"

#include <vtkActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkMath.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkLine.h>
#include <vtkCellData.h>
#include <vtkProperty.h>


vtkStandardNewMacro(vtkGridActor);


//----------------------------------------------------------------------------
vtkGridActor::vtkGridActor()
{
	this->OutlineMode = 0;
	this->AxisLabels = 1;
	this->GridType = 0;
	this->GridSize = 10; // by default 10 squares in each grid quadrant
	this->GridSpacing = 10; // by default 10 mm between each line

	this->GridOrigin[0] = 0;
	this->GridOrigin[1] = 0;
	this->GridOrigin[2] = 0;

	this->FirstAxisLabelText = NULL;
	this->FirstAxisLabelText2 = NULL;
	this->SecondAxisLabelText = NULL;
	this->SecondAxisLabelText2 = NULL;

	this->FirstAxisLabel = vtkBillboardTextActor3D::New();
	this->FirstAxisLabel2 = vtkBillboardTextActor3D::New();
	this->SecondAxisLabel = vtkBillboardTextActor3D::New();
	this->SecondAxisLabel2 = vtkBillboardTextActor3D::New();
	
	/*
	this->SetFirstAxisLabelText("-X");
	this->SetFirstAxisLabelText2("X");
	this->SetSecondAxisLabelText("-Y");
	this->SetSecondAxisLabelText2("Y");*/

	this->GridLines = vtkActor::New();
	// instanciate grid lines!

	this->GridOutline = vtkActor::New();


	GridLines->GetProperty()->SetOpacity(1);
	GridOutline->GetProperty()->SetLineWidth(3);
	// instanciate grid outilne!
	/*this->CreateGridLines();
	this->CreateGridOutline();
	*/


	/*




	this->FirstAxisLabel->ThreeDimensionalLeaderOff();
	this->FirstAxisLabel->LeaderOff();
	this->FirstAxisLabel->BorderOff();
	this->FirstAxisLabel->SetPosition(0, 0);

	this->FirstAxisLabel2->ThreeDimensionalLeaderOff();
	this->FirstAxisLabel2->LeaderOff();
	this->FirstAxisLabel2->BorderOff();
	this->FirstAxisLabel2->SetPosition(0, 0);

	this->SecondAxisLabel->ThreeDimensionalLeaderOff();
	this->SecondAxisLabel->LeaderOff();
	this->SecondAxisLabel->BorderOff();
	this->SecondAxisLabel->SetPosition(0, 0);

	this->SecondAxisLabel2->ThreeDimensionalLeaderOff();
	this->SecondAxisLabel2->LeaderOff();
	this->SecondAxisLabel2->BorderOff();
	this->SecondAxisLabel2->SetPosition(0, 0);
	*/
	this->UpdateProps();
}

//----------------------------------------------------------------------------
vtkGridActor::~vtkGridActor()
{

	this->GridOutline->Delete();
	this->GridLines->Delete();

	this->SetFirstAxisLabelText(NULL);
	this->SetFirstAxisLabelText2(NULL);
	this->SetSecondAxisLabelText(NULL);
	this->SetSecondAxisLabelText2(NULL);

	this->FirstAxisLabel->Delete();
	this->FirstAxisLabel2->Delete();
	this->SecondAxisLabel->Delete();
	this->SecondAxisLabel2->Delete();

}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkGridActor::ShallowCopy(vtkProp *prop)
{
	vtkGridActor *a = vtkGridActor::SafeDownCast(prop);
	if (a != NULL)
	{
		this->SetAxisLabels(a->GetAxisLabels());
		this->SetFirstAxisLabelText(a->GetFirstAxisLabelText());
		this->SetSecondAxisLabelText(a->GetSecondAxisLabelText());
		this->SetFirstAxisLabelText2(a->GetFirstAxisLabelText2());
		this->SetSecondAxisLabelText2(a->GetSecondAxisLabelText2());
		this->SetGridOrigin(a->GetGridOrigin());
		this->SetGridSpacing(a->GetGridSpacing());
		this->SetGridSize(a->GetGridSize());
		this->SetGridType(a->GetGridType());

	}

	// Now do superclass
	this->vtkProp3D::ShallowCopy(prop);
}

//----------------------------------------------------------------------------
void vtkGridActor::GetActors(vtkPropCollection *ac)
{
	ac->AddItem(this->GridOutline);
	ac->AddItem(this->GridLines);

}

//----------------------------------------------------------------------------
int vtkGridActor::RenderOpaqueGeometry(vtkViewport *vp)
{
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->GridOutline->RenderOpaqueGeometry(vp);
	renderedSomething += this->GridLines->RenderOpaqueGeometry(vp);


	if (this->AxisLabels)
	{
		renderedSomething += this->FirstAxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->SecondAxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->FirstAxisLabel2->RenderOpaqueGeometry(vp);
		renderedSomething += this->SecondAxisLabel2->RenderOpaqueGeometry(vp);

	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
int vtkGridActor::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->GridOutline->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->GridLines->RenderTranslucentPolygonalGeometry(vp);

	if (this->AxisLabels)
	{
		renderedSomething += this->FirstAxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->FirstAxisLabel2->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->SecondAxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->SecondAxisLabel2->RenderTranslucentPolygonalGeometry(vp);
	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int vtkGridActor::HasTranslucentPolygonalGeometry()
{
	int result = 0;

	this->UpdateProps();

	result |= this->GridOutline->HasTranslucentPolygonalGeometry();
	result |= this->GridLines->HasTranslucentPolygonalGeometry();


	if (this->AxisLabels)
	{
		result |= this->FirstAxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->SecondAxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->FirstAxisLabel2->HasTranslucentPolygonalGeometry();
		result |= this->SecondAxisLabel2->HasTranslucentPolygonalGeometry();
	}
	return result;
}

//-----------------------------------------------------------------------------
int vtkGridActor::RenderOverlay(vtkViewport *vp)
{
	int renderedSomething = 0;

	if (!this->AxisLabels)
	{
		return renderedSomething;
	}

	this->UpdateProps();

	renderedSomething += this->FirstAxisLabel->RenderOverlay(vp);
	renderedSomething += this->FirstAxisLabel2->RenderOverlay(vp);
	renderedSomething += this->SecondAxisLabel->RenderOverlay(vp);
	renderedSomething += this->SecondAxisLabel2->RenderOverlay(vp);


	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//----------------------------------------------------------------------------
void vtkGridActor::ReleaseGraphicsResources(vtkWindow *win)
{
	this->GridLines->ReleaseGraphicsResources(win);
	this->GridOutline->ReleaseGraphicsResources(win);


	this->FirstAxisLabel->ReleaseGraphicsResources(win);
	this->SecondAxisLabel->ReleaseGraphicsResources(win);
	this->FirstAxisLabel2->ReleaseGraphicsResources(win);
	this->SecondAxisLabel2->ReleaseGraphicsResources(win);

}

//----------------------------------------------------------------------------
void vtkGridActor::GetBounds(double bounds[6])
{
	double *bds = this->GetBounds();
	bounds[0] = bds[0];
	bounds[1] = bds[1];
	bounds[2] = bds[2];
	bounds[3] = bds[3];
	bounds[4] = bds[4];
	bounds[5] = bds[5];
}


void vtkGridActor::SetGridOrigin(double origin[3])
{

	if (this->GridOrigin[0] != origin[0]
		|| this->GridOrigin[1] != origin[1]
		|| this->GridOrigin[2] != origin[2]

		)
	{
		this->GridOrigin[0] = origin[0];
		this->GridOrigin[1] = origin[1];
		this->GridOrigin[2] = origin[2];


		this->Modified();
		this->UpdateProps();
	}
}
void vtkGridActor::GetGridOrigin(double origin[3])
{
	double *org = this->GetGridOrigin();

	origin[0] = org[0];
	origin[1] = org[1];
	origin[2] = org[2];

}



double *vtkGridActor::GetGridOrigin()
{
	return this->GridOrigin;
}


void vtkGridActor::CreateGridLabelText()
{
	//instantiates the 4 label text actors

	// GridType : 
	// 0 : -x x -y y
	// 1 :  -x x -z z
	// 2 : -y y -z z

	double pos1[3] = { this->GridOrigin[0] , this->GridOrigin[1], this->GridOrigin[2] };
	double pos2[3] = { this->GridOrigin[0] , this->GridOrigin[1], this->GridOrigin[2] };
	double pos3[3] = { this->GridOrigin[0] , this->GridOrigin[1], this->GridOrigin[2] };
	double pos4[3] = { this->GridOrigin[0] , this->GridOrigin[1], this->GridOrigin[2] };

	// Create six colors - one for each line
	double red[3] = { 1, 0, 0 };
	double green[3] = { 0, 1, 0 };
	double blue[3] = { 0, 0, 1 };
	double yellow[3] = { 1, 1, 0 };
	double cyan[3] = { 0, 1, 1 };
	double fuschia[3] = { 1, 0, 1 };


	vtkSmartPointer<vtkTextProperty> property1 = vtkSmartPointer<vtkTextProperty>::New();
	vtkSmartPointer<vtkTextProperty> property2 = vtkSmartPointer<vtkTextProperty>::New();
	vtkSmartPointer<vtkTextProperty> property3 = vtkSmartPointer<vtkTextProperty>::New();
	vtkSmartPointer<vtkTextProperty> property4 = vtkSmartPointer<vtkTextProperty>::New();




	//AxisLabel->
	std::string myStr1, myStr2, myStr3, myStr4;
	if (this->GridType == 0)
	{
		myStr1 = "-x";
		pos1[0] -= this->GridSize*(this->GridSpacing);
		property1->SetColor(yellow);

		//FirstAxisLabel->GetTextProperty()->SetColor(yellow);
		myStr2 = "x";//NO
		pos2[0] += this->GridSize*(this->GridSpacing);
		//FirstAxisLabel2->GetTextProperty()->SetColor(red);
		property2->SetColor(red);

		myStr3 = "-y";
		pos3[1] -= this->GridSize*(this->GridSpacing);
		
		//SecondAxisLabel->GetTextProperty()->SetColor(cyan);
		property3->SetColor(cyan);

		myStr4 = "y";
		pos4[1] += this->GridSize*(this->GridSpacing);
		property4->SetColor(green);

		//SecondAxisLabel2->GetTextProperty()->SetColor(green);
	}
	else if (this->GridType == 1)
	{
		myStr1 = "-x";
		pos1[0] -= this->GridSize*(this->GridSpacing);
		property1->SetColor(yellow);
		myStr2 = "x";//NO
		pos2[0] += this->GridSize*(this->GridSpacing);
		property2->SetColor(red);
		myStr3 = "-z";
		pos3[2] -= this->GridSize*(this->GridSpacing);
		property3->SetColor(fuschia);
		myStr4 = "z";
		pos4[2] += this->GridSize*(this->GridSpacing);
		property4->SetColor(blue);
	}
	else if (this->GridType == 2)
	{

		myStr1 = "-y";
		pos1[1] -= this->GridSize*(this->GridSpacing);
		property1->SetColor(cyan);
		myStr2 = "y"; //NO!
		pos2[1] += this->GridSize*(this->GridSpacing);
		property2->SetColor(green);
		myStr3 = "-z";
		pos3[2] -= this->GridSize*(this->GridSpacing);
		property3->SetColor(fuschia);
		myStr4 = "z";
		pos4[2] += this->GridSize*(this->GridSpacing);
		property4->SetColor(blue);


	}




	/*property1->SetFontFamilyToArial();
	property2->SetFontFamilyToArial();
	property3->SetFontFamilyToArial();
	property4->SetFontFamilyToArial();*/
	property1->SetBold(0);
	property2->SetBold(0);
	property3->SetBold(0);
	property4->SetBold(0);
	property1->SetFontSize(16);
	property2->SetFontSize(16);
	property3->SetFontSize(16);
	property4->SetFontSize(16);
	property1->SetJustificationToLeft();
	property2->SetJustificationToLeft();
	property3->SetJustificationToLeft();
	property4->SetJustificationToLeft();


	/*textActor->SetInput("");
	textActor->SetPosition(actor->GetPosition());
	textActor->GetTextProperty()->SetFontSize(12);*/
	
	
	FirstAxisLabel->SetTextProperty(property1);
	FirstAxisLabel2->SetTextProperty(property2);
	SecondAxisLabel->SetTextProperty(property3);
	SecondAxisLabel2->SetTextProperty(property4);

	/*FirstAxisLabel->SetPosition(0, 0);
	FirstAxisLabel2->SetPosition(0, 0);
	SecondAxisLabel->SetPosition(0, 0);
	SecondAxisLabel2->SetPosition(0, 0);*/
	FirstAxisLabel->SetPosition(pos1);
	FirstAxisLabel2->SetPosition(pos2);
	SecondAxisLabel->SetPosition(pos3);
	SecondAxisLabel2->SetPosition(pos4);

	/*FirstAxisLabel->SetAttachmentPoint(pos1);
	FirstAxisLabel2->SetAttachmentPoint(pos2);
	SecondAxisLabel->SetAttachmentPoint(pos3);
	SecondAxisLabel2->SetAttachmentPoint(pos4);
	*/



	/*this->FirstAxisLabel->SetHeight(0.03);
	this->FirstAxisLabel2->SetHeight(0.03);
	this->SecondAxisLabel->SetHeight(0.03);
	this->SecondAxisLabel2->SetHeight(0.03);*/



	/*this->FirstAxisLabel->BorderOff();
	this->FirstAxisLabel2->BorderOff();
	this->SecondAxisLabel->BorderOff();
	this->SecondAxisLabel2->BorderOff();


	this->FirstAxisLabel->LeaderOff();
	this->FirstAxisLabel2->LeaderOff();
	this->SecondAxisLabel->LeaderOff();
	this->SecondAxisLabel2->LeaderOff();*/


	/*this->FirstAxisLabel->SetCaption(myStr1.c_str());
	this->FirstAxisLabel2->SetCaption(myStr2.c_str());
	this->SecondAxisLabel->SetCaption(myStr3.c_str());
	this->SecondAxisLabel2->SetCaption(myStr4.c_str());*/

	this->SetFirstAxisLabelText(myStr1.c_str());
	this->SetFirstAxisLabelText2(myStr2.c_str());
	this->SetSecondAxisLabelText(myStr3.c_str());
	this->SetSecondAxisLabelText2(myStr4.c_str());

	/*this->FirstAxisLabel->SetCaption(this->FirstAxisLabelText);
	this->FirstAxisLabel2->SetCaption(this->FirstAxisLabelText2);
	this->SecondAxisLabel->SetCaption(this->SecondAxisLabelText);
	this->SecondAxisLabel2->SetCaption(this->SecondAxisLabelText2);*/
	FirstAxisLabel->SetInput(this->FirstAxisLabelText);
	FirstAxisLabel2->SetInput(this->FirstAxisLabelText2);
	SecondAxisLabel->SetInput(this->SecondAxisLabelText);
	SecondAxisLabel2->SetInput(this->SecondAxisLabelText2);




}

void vtkGridActor::CreateGridLines()
{


	// this->GridType : 0 : xy plane (z=0)
	// this->GridType : 1 : xz plane (y=0)
	// this->GridType : 2 : yz plane (x=0)

	// this->GridSize is the number of spaces between lines for each quadrant

	// this->GridSpacing : space between 2 lines in mm.


	// We need 8* (this->GridSize -1) points.
	// Then we need "only" 4 * (this->GridSize -1) lines (lines should be written only once: A=>B, not B=>A).


	vtkSmartPointer<vtkPoints> pts =
		vtkSmartPointer<vtkPoints>::New();
	// create points.
	double coord[3];
	double coord2[3];
	double coord3[3];
	for (int i = (-this->GridSize + 1); i < this->GridSize; i++)
	{
		if (i != 0)
		{
			if (this->GridType == 0)
			{
				coord[0] = i*this->GridSpacing;
				coord[1] = this->GridSize*this->GridSpacing;
				coord[2] = 0;

				coord2[0] = coord[0];
				coord2[1] = 0;
				coord2[2] = coord[2];

				coord3[0] = coord[0];
				coord3[1] = -coord[1];
				coord3[2] = coord[2];
			}
			if (this->GridType == 1)
			{
				coord[0] = i*this->GridSpacing;
				coord[1] = 0;
				coord[2] = this->GridSize*this->GridSpacing;

				coord2[0] = coord[0];
				coord2[1] = coord[1];
				coord2[2] = 0;

				coord3[0] = coord[0];
				coord3[1] = coord[1];
				coord3[2] = -coord[2];
			}
			if (this->GridType == 2)
			{
				coord[0] = 0;
				coord[1] = i*this->GridSpacing;
				coord[2] = this->GridSize*this->GridSpacing;

				coord2[0] = coord[0];
				coord2[1] = coord[1];
				coord2[2] = 0;

				coord3[0] = coord[0];
				coord3[1] = coord[1];
				coord3[2] = -coord[2];
			}
			vtkMath::Add(coord, this->GridOrigin, coord);
			vtkMath::Add(coord2, this->GridOrigin, coord2);
			vtkMath::Add(coord3, this->GridOrigin, coord3);
			//cout << "point 1:" << coord[0] << "," << coord[1] << "," << coord[2] << endl;
			//cout << "point 2:" << coord2[0] << "," << coord2[1] << "," << coord2[2] << endl;
			pts->InsertNextPoint(coord);
			pts->InsertNextPoint(coord2);
			pts->InsertNextPoint(coord3);

		}

	}
	for (int i = (-this->GridSize + 1); i < this->GridSize; i++)
	{
		if (i != 0)
		{
			if (this->GridType == 0)
			{
				coord[0] = this->GridSize*this->GridSpacing;
				coord[1] = i*this->GridSpacing;
				coord[2] = 0;

				coord2[0] = 0;
				coord2[1] = coord[1];
				coord2[2] = coord[2];


				coord3[0] = -coord[0];
				coord3[1] = coord[1];
				coord3[2] = coord[2];
			}
			if (this->GridType == 1)
			{
				coord[0] = this->GridSize*this->GridSpacing;
				coord[1] = 0;
				coord[2] = i*this->GridSpacing;

				coord2[0] = 0;
				coord2[1] = coord[1];
				coord2[2] = coord[2];

				coord3[0] = -coord[0];
				coord3[1] = coord[1];
				coord3[2] = coord[2];
			}
			if (this->GridType == 2)
			{
				coord[0] = 0;
				coord[1] = this->GridSize*this->GridSpacing;
				coord[2] = i*this->GridSpacing;

				coord2[0] = coord[0];
				coord2[1] = 0;
				coord2[2] = coord[2];

				coord3[0] = coord[0];
				coord3[1] = -coord[1];
				coord3[2] = coord[2];
			}
			vtkMath::Add(coord, this->GridOrigin, coord);
			vtkMath::Add(coord2, this->GridOrigin, coord2);
			vtkMath::Add(coord3, this->GridOrigin, coord3);
			//cout << "2point 1:" << coord[0] << "," << coord[1] << "," << coord[2] << endl;
			//cout << "2point 2:" << coord2[0] << "," << coord2[1] << "," << coord2[2] << endl;
			pts->InsertNextPoint(coord);
			pts->InsertNextPoint(coord2);
			pts->InsertNextPoint(coord3);

		}

	}
	vtkSmartPointer<vtkPolyData> linesPolyData =
		vtkSmartPointer<vtkPolyData>::New();
	// Add the points to the polydata container
	linesPolyData->SetPoints(pts);
	// We have 4* (this->GridSize -1) lines. *2

	vtkSmartPointer<vtkCellArray> lines =
		vtkSmartPointer<vtkCellArray>::New();

	for (int i = 0; i < (4 * (this->GridSize - 1)); i++)
	{
		vtkSmartPointer<vtkLine> line =
			vtkSmartPointer<vtkLine>::New();
		line->GetPointIds()->SetId(0, 3 * i);
		line->GetPointIds()->SetId(1, 3 * i + 1);
		lines->InsertNextCell(line);


	}
	for (int i = 0; i < (4 * (this->GridSize - 1)); i++)
	{
		vtkSmartPointer<vtkLine> line2 =
			vtkSmartPointer<vtkLine>::New();
		line2->GetPointIds()->SetId(0, 3 * i + 1);
		line2->GetPointIds()->SetId(1, 3 * i + 2);
		lines->InsertNextCell(line2);


	}


	linesPolyData->SetLines(lines);


	// Create six colors - one for each line
	unsigned char red[3] = { 255, 0, 0 };
	unsigned char green[3] = { 0, 255, 0 };
	unsigned char blue[3] = { 0, 0, 255 };
	unsigned char yellow[3] = { 255, 255, 0 };
	unsigned char cyan[3] = { 0, 255, 255 };
	unsigned char fuschia[3] = { 255, 0, 255 };

	// Create a vtkUnsignedCharArray container and store the colors in it
	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	colors->SetNumberOfComponents(3);
	for (int i = 0; i < (2 * (this->GridSize - 1)); i++)
	{
		if (this->GridType == 0)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(green);
#else
			colors->InsertNextTupleValue(green);
#endif

		}
		else if (this->GridType == 1)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(blue);
#else
			colors->InsertNextTupleValue(blue);
#endif				
		}
		else
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(blue);
#else
			colors->InsertNextTupleValue(blue);
#endif				
		}

	}
	for (int i = 0; i < (2 * (this->GridSize - 1)); i++)
	{
		if (this->GridType == 0)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(red);
#else
			colors->InsertNextTupleValue(red);
#endif				
		}
		else if (this->GridType == 1)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(red);
#else
			colors->InsertNextTupleValue(red);
#endif

		}
		else
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(green);
#else
			colors->InsertNextTupleValue(green);
#endif

		}
	}
	for (int i = 0; i < (2 * (this->GridSize - 1)); i++)
	{
		if (this->GridType == 0)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(cyan);
#else
			colors->InsertNextTupleValue(cyan);
#endif

		}
		else if (this->GridType == 1)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(fuschia);
#else
			colors->InsertNextTupleValue(fuschia);
#endif


		}
		else
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(fuschia);
#else
			colors->InsertNextTupleValue(fuschia);
#endif


		}
	}


	for (int i = 0; i < (2 * (this->GridSize - 1)); i++)
	{
		if (this->GridType == 0)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(yellow);
#else
			colors->InsertNextTupleValue(yellow);
#endif


		}
		else if (this->GridType == 1)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(yellow);
#else
			colors->InsertNextTupleValue(yellow);
#endif

		}
		else
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			colors->InsertNextTypedTuple(cyan);
#else
			colors->InsertNextTupleValue(cyan);
#endif


		}

	}




	linesPolyData->GetCellData()->SetScalars(colors);

	// Setup the visualization pipeline
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);


	this->GridLines->SetMapper(mapper);







}

void vtkGridActor::CreateGridOutline()
{

	//OutlineMode : 0 => orange
	//OutlineMode : 1 => cyan

	// GridType : 0 : xy this->GridType (z=0)
	// GridType : 1 : xz this->GridType (y=0)
	// GridType : 2 : yz this->GridType (x=0)
	// this->GridSize is the number of spaces between lines for each quadrant

	// this->GridSpacing : space between 2 lines in mm.



	vtkSmartPointer<vtkPoints> pts =
		vtkSmartPointer<vtkPoints>::New();
	// create points.

	// we need 8 points + origin = 9 points
	// we need to draw 12 bold lines.
	// create points.
	double coord[3];
	double coord2[3];
	double coord3[3];
	double coord4[3];
	double coord5[3];
	double coord6[3];
	double coord7[3];
	double coord8[3];
	double coord9[3];

	if (this->GridType == 0)
	{
		coord[0] = -this->GridSize*this->GridSpacing;
		coord[1] = this->GridSize*this->GridSpacing;
		coord[2] = 0;

		coord2[0] = -this->GridSize*this->GridSpacing;
		coord2[1] = 0;
		coord2[2] = 0;

		coord3[0] = -this->GridSize*this->GridSpacing;
		coord3[1] = -this->GridSize*this->GridSpacing;
		coord3[2] = 0;


		coord4[0] = 0;
		coord4[1] = this->GridSize*this->GridSpacing;
		coord4[2] = 0;

		coord5[0] = 0;
		coord5[1] = 0;
		coord5[2] = 0;

		coord6[0] = 0;
		coord6[1] = -this->GridSize*this->GridSpacing;
		coord6[2] = 0;

		coord7[0] = this->GridSize*this->GridSpacing;
		coord7[1] = this->GridSize*this->GridSpacing;
		coord7[2] = 0;

		coord8[0] = this->GridSize*this->GridSpacing;
		coord8[1] = 0;
		coord8[2] = 0;

		coord9[0] = this->GridSize*this->GridSpacing;
		coord9[1] = -this->GridSize*this->GridSpacing;
		coord9[2] = 0;

	}
	else if (this->GridType == 1)
	{
		coord[0] = -this->GridSize*this->GridSpacing;
		coord[1] = 0;
		coord[2] = this->GridSize*this->GridSpacing;

		coord2[0] = -this->GridSize*this->GridSpacing;
		coord2[1] = 0;
		coord2[2] = 0;

		coord3[0] = -this->GridSize*this->GridSpacing;
		coord3[1] = 0;
		coord3[2] = -this->GridSize*this->GridSpacing;


		coord4[0] = 0;
		coord4[1] = 0;
		coord4[2] = this->GridSize*this->GridSpacing;

		coord5[0] = 0;
		coord5[1] = 0;
		coord5[2] = 0;

		coord6[0] = 0;
		coord6[1] = 0;
		coord6[2] = -this->GridSize*this->GridSpacing;

		coord7[0] = this->GridSize*this->GridSpacing;
		coord7[1] = 0;
		coord7[2] = this->GridSize*this->GridSpacing;

		coord8[0] = this->GridSize*this->GridSpacing;
		coord8[1] = 0;
		coord8[2] = 0;

		coord9[0] = this->GridSize*this->GridSpacing;
		coord9[1] = 0;
		coord9[2] = -this->GridSize*this->GridSpacing;

	}
	else
	{
		coord[0] = 0;
		coord[1] = -this->GridSize*this->GridSpacing;
		coord[2] = this->GridSize*this->GridSpacing;

		coord2[0] = 0;
		coord2[1] = -this->GridSize*this->GridSpacing;
		coord2[2] = 0;

		coord3[0] = 0;
		coord3[1] = -this->GridSize*this->GridSpacing;
		coord3[2] = -this->GridSize*this->GridSpacing;


		coord4[0] = 0;
		coord4[1] = 0;
		coord4[2] = this->GridSize*this->GridSpacing;

		coord5[0] = 0;
		coord5[1] = 0;
		coord5[2] = 0;

		coord6[0] = 0;
		coord6[1] = 0;
		coord6[2] = -this->GridSize*this->GridSpacing;

		coord7[0] = 0;
		coord7[1] = this->GridSize*this->GridSpacing;
		coord7[2] = this->GridSize*this->GridSpacing;

		coord8[0] = 0;
		coord8[1] = this->GridSize*this->GridSpacing;
		coord8[2] = 0;

		coord9[0] = 0;
		coord9[1] = this->GridSize*this->GridSpacing;
		coord9[2] = -this->GridSize*this->GridSpacing;
	}

	vtkMath::Add(coord, this->GridOrigin, coord);
	vtkMath::Add(coord2, this->GridOrigin, coord2);
	vtkMath::Add(coord3, this->GridOrigin, coord3);
	vtkMath::Add(coord4, this->GridOrigin, coord4);
	vtkMath::Add(coord5, this->GridOrigin, coord5);
	vtkMath::Add(coord6, this->GridOrigin, coord6);
	vtkMath::Add(coord7, this->GridOrigin, coord7);
	vtkMath::Add(coord8, this->GridOrigin, coord8);
	vtkMath::Add(coord9, this->GridOrigin, coord9);

	pts->InsertNextPoint(coord);
	pts->InsertNextPoint(coord2);
	pts->InsertNextPoint(coord3);
	pts->InsertNextPoint(coord4);
	pts->InsertNextPoint(coord5);
	pts->InsertNextPoint(coord6);
	pts->InsertNextPoint(coord7);
	pts->InsertNextPoint(coord8);
	pts->InsertNextPoint(coord9);

	vtkSmartPointer<vtkPolyData> linesPolyData =
		vtkSmartPointer<vtkPolyData>::New();
	// Add the points to the polydata container
	linesPolyData->SetPoints(pts);
	// We have 4* (this->GridSize -1) lines. *2

	vtkSmartPointer<vtkCellArray> lines =
		vtkSmartPointer<vtkCellArray>::New();


	vtkSmartPointer<vtkLine> line1 =
		vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId(0, 0);
	line1->GetPointIds()->SetId(1, 1);
	lines->InsertNextCell(line1);

	vtkSmartPointer<vtkLine> line2 =
		vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId(0, 1);
	line2->GetPointIds()->SetId(1, 2);
	lines->InsertNextCell(line2);

	vtkSmartPointer<vtkLine> line3 =
		vtkSmartPointer<vtkLine>::New();
	line3->GetPointIds()->SetId(0, 3);
	line3->GetPointIds()->SetId(1, 4);
	lines->InsertNextCell(line3);

	vtkSmartPointer<vtkLine> line4 =
		vtkSmartPointer<vtkLine>::New();
	line4->GetPointIds()->SetId(0, 4);
	line4->GetPointIds()->SetId(1, 5);
	lines->InsertNextCell(line4);

	vtkSmartPointer<vtkLine> line5 =
		vtkSmartPointer<vtkLine>::New();
	line5->GetPointIds()->SetId(0, 6);
	line5->GetPointIds()->SetId(1, 7);
	lines->InsertNextCell(line5);

	vtkSmartPointer<vtkLine> line6 =
		vtkSmartPointer<vtkLine>::New();
	line6->GetPointIds()->SetId(0, 7);
	line6->GetPointIds()->SetId(1, 8);
	lines->InsertNextCell(line6);

	vtkSmartPointer<vtkLine> line7 =
		vtkSmartPointer<vtkLine>::New();
	line7->GetPointIds()->SetId(0, 6);
	line7->GetPointIds()->SetId(1, 3);
	lines->InsertNextCell(line7);

	vtkSmartPointer<vtkLine> line8 =
		vtkSmartPointer<vtkLine>::New();
	line8->GetPointIds()->SetId(0, 3);
	line8->GetPointIds()->SetId(1, 0);
	lines->InsertNextCell(line8);

	vtkSmartPointer<vtkLine> line9 =
		vtkSmartPointer<vtkLine>::New();
	line9->GetPointIds()->SetId(0, 7);
	line9->GetPointIds()->SetId(1, 4);
	lines->InsertNextCell(line9);

	vtkSmartPointer<vtkLine> line10 =
		vtkSmartPointer<vtkLine>::New();
	line10->GetPointIds()->SetId(0, 4);
	line10->GetPointIds()->SetId(1, 1);
	lines->InsertNextCell(line10);

	vtkSmartPointer<vtkLine> line11 =
		vtkSmartPointer<vtkLine>::New();
	line11->GetPointIds()->SetId(0, 8);
	line11->GetPointIds()->SetId(1, 5);
	lines->InsertNextCell(line11);

	vtkSmartPointer<vtkLine> line12 =
		vtkSmartPointer<vtkLine>::New();
	line12->GetPointIds()->SetId(0, 5);
	line12->GetPointIds()->SetId(1, 2);
	lines->InsertNextCell(line12);



	linesPolyData->SetLines(lines);


	// Create six colors - one for each line
	/*unsigned char red[3] = { 255, 0, 0 };
	unsigned char green[3] = { 0, 255, 0 };
	unsigned char blue[3] = { 0, 0, 255 };
	unsigned char yellow[3] = { 255, 255, 0 };
	unsigned char cyan[3] = { 0, 255, 255 };
	unsigned char fuschia[3] = { 255, 0, 255 };*/
	unsigned char outline_color[3] = { 125, 125, 125 };
	if (this->OutlineMode == 0)
	{
		
		outline_color[0] = 0;
		outline_color[1] = 127;
		outline_color[2] = 255;
		
	}
	else
	{
		
		outline_color[0] = 255;
		outline_color[1] = 127;
		outline_color[2] = 0;

	}
	
	

	// Create a vtkUnsignedCharArray container and store the colors in it
	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	colors->SetNumberOfComponents(3);

	//1
	if (this->GridType == 0)
	{
		for (int i = 0; i < 3; i++)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			/*colors->InsertNextTypedTuple(green);
			colors->InsertNextTypedTuple(cyan);*/
			colors->InsertNextTypedTuple(outline_color);
			colors->InsertNextTypedTuple(outline_color);
#else
			//colors->InsertNextTupleValue(green);
			//colors->InsertNextTupleValue(cyan);
			colors->InsertNextTupleValue(outline_color);
			colors->InsertNextTupleValue(outline_color);
#endif
		}
		for (int i = 0; i < 3; i++)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			/*colors->InsertNextTypedTuple(red);
			colors->InsertNextTypedTuple(yellow);*/
			colors->InsertNextTypedTuple(outline_color);
			colors->InsertNextTypedTuple(outline_color);
#else
			/*colors->InsertNextTupleValue(red);
			colors->InsertNextTupleValue(yellow);*/
			colors->InsertNextTupleValue(outline_color);
			colors->InsertNextTupleValue(outline_color);
#endif

		}
	}
	else if (this->GridType == 1)
	{
		for (int i = 0; i < 3; i++)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			/*colors->InsertNextTypedTuple(blue);
			colors->InsertNextTypedTuple(fuschia);*/
			colors->InsertNextTypedTuple(outline_color);
			colors->InsertNextTypedTuple(outline_color);
#else
			/*colors->InsertNextTupleValue(blue);
			colors->InsertNextTupleValue(fuschia);*/
			colors->InsertNextTupleValue(outline_color);
			colors->InsertNextTupleValue(outline_color);
#endif

		}
		for (int i = 0; i < 3; i++)
		{
			for (int i = 0; i < 3; i++)
			{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
				/*colors->InsertNextTypedTuple(red);
				colors->InsertNextTypedTuple(yellow);*/
				colors->InsertNextTypedTuple(outline_color);
				colors->InsertNextTypedTuple(outline_color);
#else
				/*colors->InsertNextTupleValue(red);
				colors->InsertNextTupleValue(yellow);*/
				colors->InsertNextTupleValue(outline_color);
				colors->InsertNextTupleValue(outline_color);
#endif

			}

		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			/*colors->InsertNextTypedTuple(blue);
			colors->InsertNextTypedTuple(fuschia);*/
			colors->InsertNextTypedTuple(outline_color);
			colors->InsertNextTypedTuple(outline_color);
#else
			/*colors->InsertNextTupleValue(blue);
			colors->InsertNextTupleValue(fuschia);*/
			colors->InsertNextTupleValue(outline_color);
			colors->InsertNextTupleValue(outline_color);
#endif
		}
		for (int i = 0; i < 3; i++)
		{
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1
			/*colors->InsertNextTypedTuple(green);
			colors->InsertNextTypedTuple(cyan);*/
			colors->InsertNextTypedTuple(outline_color);
			colors->InsertNextTypedTuple(outline_color);
#else
			/*colors->InsertNextTupleValue(green);
			colors->InsertNextTupleValue(cyan);*/
			colors->InsertNextTupleValue(outline_color);
			colors->InsertNextTupleValue(outline_color);
#endif

		}
	}


	linesPolyData->GetCellData()->SetScalars(colors);

	// Setup the visualization pipeline
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);

	this->GridOutline->SetMapper(mapper);





}
//----------------------------------------------------------------------------
// Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax).
double *vtkGridActor::GetBounds()
{

	
	if (this->GridType == 0) // z=0
	{
		this->Bounds[0] = this->GridOrigin[0]-this->GridSize * this->GridSpacing;
		this->Bounds[1] = this->GridOrigin[0]+this->GridSize * this->GridSpacing;
		this->Bounds[2] = this->GridOrigin[1] -this->GridSize * this->GridSpacing;
		this->Bounds[3] = this->GridOrigin[1]+this->GridSize * this->GridSpacing;
		this->Bounds[4] = this->GridOrigin[2];
		this->Bounds[5] = this->GridOrigin[2];
	}
	else if (this->GridType == 1) //y=0
	{
		this->Bounds[0] = this->GridOrigin[0] -this->GridSize * this->GridSpacing;
		this->Bounds[1] = this->GridOrigin[0]+this->GridSize * this->GridSpacing;
		this->Bounds[2] = this->GridOrigin[1];
		this->Bounds[3] = this->GridOrigin[1];
		this->Bounds[4] = this->GridOrigin[2] -this->GridSize * this->GridSpacing;
		this->Bounds[5] = this->GridOrigin[2]+this->GridSize * this->GridSpacing;
	}
	else // x=0
	{
		this->Bounds[0] = this->GridOrigin[0];
		this->Bounds[1] = this->GridOrigin[0];
		this->Bounds[2] = this->GridOrigin[1] -this->GridSize * this->GridSpacing;
		this->Bounds[3] = this->GridOrigin[1]+this->GridSize * this->GridSpacing;
		this->Bounds[4] = this->GridOrigin[2] -this->GridSize * this->GridSpacing;
		this->Bounds[5] = this->GridOrigin[2]+this->GridSize * this->GridSpacing;
	}

	return this->Bounds;
}

//----------------------------------------------------------------------------
#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1

vtkMTimeType vtkGridActor::GetMTime()
{
	vtkMTimeType mTime = this->Superclass::GetMTime();
	return mTime;
}

vtkMTimeType vtkGridActor::GetRedrawMTime()
{
	vtkMTimeType mTime = this->GetMTime();
	return mTime;
}
#else
unsigned long int vtkGridActor::GetMTime()
{
	unsigned long mTime = this->Superclass::GetMTime();
	return mTime;
}



//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
unsigned long int vtkGridActor::GetRedrawMTime()
{
	unsigned long mTime = this->GetMTime();
	return mTime;
}

#endif


//----------------------------------------------------------------------------
void vtkGridActor::SetGridType(int type)
{
	if (this->GridType != type && type >= 0 && type <= 2)
	{
		this->GridType = type;
		this->Modified();
		this->UpdateProps();
	}


}

void vtkGridActor::SetGridSize(double size)
{
	if (this->GridSize != size && size>0)
	{
		this->GridSize = size;
		this->Modified();
		this->UpdateProps();
	}

}


void vtkGridActor::SetGridSpacing(double spacing)
{
	if (this->GridSpacing != spacing && spacing>0)
	{
		this->GridSpacing = spacing;
		this->Modified();
		this->UpdateProps();
	}

}

//----------------------------------------------------------------------------
void vtkGridActor::UpdateProps()
{
	//Recreates the 4 text actors and the GridOutline and GridLines actors
	this->CreateGridLabelText();
	this->CreateGridLines();
	this->CreateGridOutline();
}


//----------------------------------------------------------------------------
void vtkGridActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "GridType: ";
	if (this->GridType)
	{
		os << this->GridType << endl;
	}
	else
	{
		os << "(no Grid Type)" << endl;
	}

	os << indent << "Grid origin: ";
	if (this->GridOrigin[0])
	{
		os << this->GridOrigin[0] << "," << this->GridOrigin[1] << "," << this->GridOrigin[2] << endl;
	}
	else
	{
		os << "(no Grid Origin)" << endl;
	}

	os << indent << "GridSize: ";
	if (this->GridSize)
	{
		os << this->GridSize << endl;
	}
	else
	{
		os << "(no Grid Size)" << endl;
	}

	os << indent << "GridSpacing: ";
	if (this->GridSpacing)
	{
		os << this->GridSpacing << endl;
	}
	else
	{
		os << "(no Grid Spacing)" << endl;
	}

	os << indent << "FirstAxisLabelText: " << (this->FirstAxisLabelText ?
		this->FirstAxisLabelText : "(none)")
		<< endl;
	os << indent << "FirstAxisLabelText2: " << (this->FirstAxisLabelText2 ?
		this->FirstAxisLabelText2 : "(none)")
		<< endl;

	os << indent << "SecondAxisLabelText: " << (this->SecondAxisLabelText ?
		this->SecondAxisLabelText : "(none)")
		<< endl;

	os << indent << "SecondAxisLabelText2: " << (this->SecondAxisLabelText2 ?
		this->SecondAxisLabelText2 : "(none)")
		<< endl;

	os << indent << "AxisLabels: " << (this->AxisLabels ? "On\n" : "Off\n");


}
