/*=========================================================================

  Program:   Visualization Toolkit
  Module:    MorphoDig.h
  Language:  C++

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef MorphoDig_H
#define MorphoDig_H
#include "ui_MorphoDig.h"
#include "mqMorphoDigCore.h"

#if VTK_MAJOR_VERSION<8
#include <QVTKWidget.h>
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION<2
#include <QVTKOpenGLWidget.h>
#else
#include <QVTKOpenGLNativeWidget.h>
//#include <QVTKWidget.h>
#endif


#include <vtkSmartPointer.h>    // Required for smart pointer internal ivars.
#include <vtkRenderedAreaPicker.h>   
#include <vtkAreaPicker.h>   

#include <vtkRenderer.h>
#include <vtkImageData.h>

#include <vtkStructuredGrid.h>
#include <vtkBillboardTextActor3D.h>
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>


// Forward Qt class declarations
class Ui_MorphoDig;

// Forward VTK class declarations
class vtkQtTableView;


class MorphoDig : public QMainWindow, private Ui::MorphoDig {

  Q_OBJECT

public:
	explicit MorphoDig(QWidget *parent = 0);

 
  // Constructor/Destructor
  //MorphoDig();
  ~MorphoDig();
  
  mqMorphoDigCore* MorphoDigCore;
  void dragEnterEvent(QDragEnterEvent *e);
  void dropEvent(QDropEvent *e);
  //vtkSmartPointer<vtkRenderedAreaPicker> AreaPicker;
 vtkSmartPointer<vtkAreaPicker> AreaPicker;
 void ambientLightToWhite();
  
  
  
public slots:
  //static  MorphoDig* Instance;
 // static MorphoDig* instance();
 // static int getTestInt();
  
  
  
  
  virtual void slotExit();
  


protected:

protected slots:
	
private:
	//
#if VTK_MAJOR_VERSION<8
	QVTKWidget *qvtkWidget2;
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	QVTKOpenGLWidget *qvtkWidget2;
#else
	QVTKOpenGLNativeWidget *qvtkWidget2;
	//QVTKWidget *qvtkWidget2;
#endif
	
	
	
	
	void saveSettings();
	
	vtkSmartPointer<vtkBoxWidget> _boxWidget;
  vtkSmartPointer<vtkQtTableView>         TableView;
   //Ui_MorphoDig *ui;
};

#endif // MorphoDig_H
