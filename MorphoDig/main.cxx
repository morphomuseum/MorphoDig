/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */
// QT includes

#include <QApplication>
#include <QSurfaceFormat>
//#include <QVTKOpenGLWidget.h>

#include <QTextCodec>
#if QT_VERSION < 0x050000

  #include <QCleanlooksStyle>
#endif
#include "mqMinimalWidget.h"
#include "MorphoDig.h"

// @@

//@@

extern int qInitResources_icons();

int main( int argc, char* argv[] )
{

  // QT Stuff
 //QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
	//QSurfaceFormat fmt = QVTKOpenGLWidget::defaultFormat();
	//fmt.setSamples(0);
	//QSurfaceFormat::setDefaultFormat(fmt);
  QApplication app( argc, argv );
  //QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
  #if QT_VERSION >= 0x050000
    QApplication::setStyle("fusion");
  #else
    QApplication::setStyle(new QCleanlooksStyle);
  #endif
  
  qInitResources_icons();
  
  //mqMinimalWidget mymin;
  //mymin.show();
  

  MorphoDig myMorphoDig;
  myMorphoDig.show();

  // Create a table with some points in it

  return app.exec();
}
/*

//Just for some tests implying vtkChartXY which was not working for a long while... 

int main( int argc, char* argv[] )
{
#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>


vtkSmartPointer<vtkTable> table =
vtkSmartPointer<vtkTable>::New();

vtkSmartPointer<vtkFloatArray> arrX =
vtkSmartPointer<vtkFloatArray>::New();
arrX->SetName("X Axis");
table->AddColumn(arrX);

vtkSmartPointer<vtkFloatArray> arrC =
vtkSmartPointer<vtkFloatArray>::New();
arrC->SetName("Cosine");
table->AddColumn(arrC);

vtkSmartPointer<vtkFloatArray> arrS =
vtkSmartPointer<vtkFloatArray>::New();
arrS->SetName("Sine");
table->AddColumn(arrS);

// Fill in the table with some example values
int numPoints = 69;
float inc = 7.5 / (numPoints - 1);
table->SetNumberOfRows(numPoints);
for (int i = 0; i < numPoints; ++i)
{
table->SetValue(i, 0, i * inc);
table->SetValue(i, 1, cos(i * inc));
table->SetValue(i, 2, sin(i * inc));
}

// Set up the view
vtkSmartPointer<vtkContextView> view =
vtkSmartPointer<vtkContextView>::New();
view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

// Add multiple line plots, setting the colors etc
vtkSmartPointer<vtkChartXY> chart =
vtkSmartPointer<vtkChartXY>::New();
view->GetScene()->AddItem(chart);
vtkPlot *line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
line->SetInput(table, 0, 1);
#else
line->SetInputData(table, 0, 1);
#endif
line->SetColor(0, 255, 0, 255);
line->SetWidth(1.0);
line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
line->SetInput(table, 0, 2);
#else
line->SetInputData(table, 0, 2);
#endif
line->SetColor(255, 0, 0, 255);
line->SetWidth(5.0);

// For dotted line, the line type can be from 2 to 5 for different dash/dot
// patterns (see enum in vtkPen containing DASH_LINE, value 2):
#ifndef WIN32
line->GetPen()->SetLineType(vtkPen::DASH_LINE);
#endif
// (ifdef-ed out on Windows because DASH_LINE does not work on Windows
//  machines with built-in Intel HD graphics card...)

//view->GetRenderWindow()->SetMultiSamples(0);

// Start interactor
view->GetInteractor()->Initialize();
view->GetInteractor()->Start();
return EXIT_SUCCESS;
}
*/