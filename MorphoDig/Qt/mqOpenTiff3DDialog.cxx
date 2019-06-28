/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqOpenTiff3DDialog.h"
#include "ui_mqOpenTiff3DDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkTIFFReader.h>
// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QString>
#include <QMessageBox>
#include <QHeaderView>


#include <sstream>

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqOpenTiff3DDialog::mqOpenTiff3DDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqOpenTiff3DDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqOpenTiff3DDialog");	
	
	this->myFileName = "";
	this->inputAsStack = 0;
	this->myDimX = 0;
	this->myDimX = 0;
	this->myInput = vtkSmartPointer<vtkImageData>::New();
  this->Ui->voxelSizeX->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeY->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->dimX->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->dimY->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->dimZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
 /*this->Ui->headerSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->requestedSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
 this->Ui->fileSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
  
  this->Ui->headerSize->setMaximum(DBL_MAX);
  this->Ui->requestedSize->setMaximum(DBL_MAX);
  this->Ui->fileSize->setMaximum(DBL_MAX);

  this->Ui->comboDataType->addItem("8 bits unsigned");
  
  this->Ui->comboDataType->addItem("16 bits signed");
  this->Ui->comboDataType->addItem("16 bits unsigned");
  this->Ui->comboDataType->addItem("32 bits float");
  this->Ui->comboDataType->addItem("64 bits float");
  this->Ui->comboDataType->setCurrentIndex(0);*/
  this->Ui->voxelSizeX->setValue(1);
  this->Ui->voxelSizeY->setValue(1);
  this->Ui->voxelSizeZ->setValue(1);
  this->mySpacingX = 1;
  this->mySpacingY = 1;
  this->mySpacingZ = 1;
 
  /*  connect(this->Ui->dimX, SIGNAL(valueChanged(int)), this, SLOT(slotDimXChanged(int)));
  connect(this->Ui->dimY, SIGNAL(valueChanged(int)), this, SLOT(slotDimYChanged(int)));
  connect(this->Ui->dimZ, SIGNAL(valueChanged(int)), this, SLOT(slotDimZChanged(int)));
  connect(this->Ui->headerSize, SIGNAL(valueChanged(double)), this, SLOT(slotHeaderSizeChanged(double)));
  //connect(this->Ui->dimY, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  //connect(this->Ui->dimZ, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  connect(this->Ui->comboDataType, SIGNAL(activated(int)), this, SLOT(slotDataTypeChanged(int)));*/

  connect(this->Ui->voxelSizeX, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeXChanged(double))); 
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOpenTiff3D()));

}




//-----------------------------------------------------------------------------
mqOpenTiff3DDialog::~mqOpenTiff3DDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqOpenTiff3DDialog::setInputAsStack()
{
	this->inputAsStack = 1;
}
void mqOpenTiff3DDialog::setInputAs3DFile()
{
	this->inputAsStack = 0;
}
void mqOpenTiff3DDialog::OpenTiff3D()
{
	cout << "OpenTiff3D dialog" << endl;
	
		std::string action = "OpenTiff3D";
		/*if (this->Ui->fileSize->value()>0 && this->Ui->requestedSize->value()!= this->Ui->fileSize->value())
		{
		QMessageBox msgBox;
		msgBox.setText("Requested size differs from file size. Proceed anyway ?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}*/
		if (this->Ui->voxelSizeX->value() == 0 || this->Ui->voxelSizeY->value() == 0|| this->Ui->voxelSizeZ->value() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Voxel size is 0 in x,y or z. Proceed anyway ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}
		/*int cType = this->Ui->comboDataType->currentIndex();
		int dataType = VTK_CHAR;
		if (cType == 0) { dataType = VTK_UNSIGNED_CHAR; }
		else if (cType == 1) { dataType = VTK_SHORT; }
		else if (cType == 2) { dataType = VTK_UNSIGNED_SHORT; }
		else if (cType == 3) { dataType = VTK_FLOAT; }
		else if (cType == 4) { dataType = VTK_DOUBLE; }*/


		if (this->inputAsStack == 0)
		{

			mqMorphoDigCore::instance()->OpenTiff3DVolume(this->myFileName,
				this->Ui->ObjectName->text(),
				/*dataType,
				this->Ui->dimX->value(),
				this->Ui->dimY->value(),
				this->Ui->dimZ->value(),
				this->Ui->headerSize->value(),*/
				this->Ui->voxelSizeX->value(),
				this->Ui->voxelSizeY->value(),
				this->Ui->voxelSizeZ->value(),
				this->Ui->frontToBack->isChecked()
				/*this->Ui->bigEndian->isChecked(),
				this->Ui->frontToBack->isChecked()*/);
		}
		else
		{
			mqMorphoDigCore::instance()->Open2DStack(this->myInput,
				this->Ui->ObjectName->text(),				
				this->Ui->voxelSizeX->value(),
				this->Ui->voxelSizeY->value(),
				this->Ui->voxelSizeZ->value(),
				this->Ui->frontToBack->isChecked()
				);
		}
		
	
}

void mqOpenTiff3DDialog::set2DStackInput(vtkSmartPointer<vtkImageData> input)
{
	this->myInput = input;
}

void mqOpenTiff3DDialog::slotVoxelSizeXChanged(double newsVoxelSizeX)
{
	this->Ui->voxelSizeY->setValue(newsVoxelSizeX);
	this->Ui->voxelSizeZ->setValue(newsVoxelSizeX);
}
/*
void mqOpenTiff3DDialog::slotDataTypeChanged(int newDataType)
{
	this->RecomputeRequested(newDataType, this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), this->Ui->headerSize->value());
}
void mqOpenTiff3DDialog::slotDimXChanged(int newDimX)
{
	this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), newDimX, this->Ui->dimY->value(), this->Ui->dimZ->value(), this->Ui->headerSize->value());
}
 void mqOpenTiff3DDialog::slotDimYChanged(int newDimY)
 {
	 this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), newDimY, this->Ui->dimZ->value(), this->Ui->headerSize->value());
 }
 void mqOpenTiff3DDialog::slotDimZChanged(int newDimZ)
 {
	 this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), this->Ui->dimY->value(), newDimZ, this->Ui->headerSize->value());
 }
 void mqOpenTiff3DDialog::slotHeaderSizeChanged(double newHeaderSize)
 {
	 this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), newHeaderSize);
 }

void mqOpenTiff3DDialog::RecomputeRequested(int dataType, int dimX, int dimY, int dimZ, double headerSize)
{
	double mult = 1;
	if (dataType == 0) { mult = 1; }
	if (dataType == 1) { mult = 2; }
	if (dataType == 2) { mult = 2; }
	if (dataType == 3) { mult = 4; }
	if (dataType == 4) { mult = 8; }

	double dimx = (double)dimX;
	double dimy = (double)dimY;
	double dimz = (double)dimZ;
	double requested = mult * dimx *dimy *dimz + headerSize;
	this->Ui->requestedSize->setValue(requested);
	


}*/
void mqOpenTiff3DDialog::setFileName(QString fileName)
{
	this->myFileName = fileName;
	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());
	std::string only_filename = onlyfilename.toStdString();
	std::string newname = only_filename.c_str();
	size_t nPos = newname.find_last_of(".");
	if (nPos > 0)
	{

		newname = newname.substr(0, nPos);
	}
	QString objectName = "";
	objectName = objectName+ newname.c_str();
//	double fileSize = (double)fileInfo.size();
//	this->Ui->fileSize->setValue(fileSize);
	this->Ui->ObjectName->setText(objectName);
	
	if (this->inputAsStack == 0)
	{
		//int that case, there is only 1 file and Data Type and Dimensions must be searched within "fileName".
		vtkSmartPointer <vtkTIFFReader> tiffReader = vtkSmartPointer<vtkTIFFReader>::New();
		tiffReader->SetFileName(fileName.toLocal8Bit());
		cout << "Spacing Flag!" << endl;
		
		tiffReader->SetSpacingSpecifiedFlag(true);

		//tiffReader->GetF
		tiffReader->Update();
		this->myInput = tiffReader->GetOutput();				
		int dim[3];
		double spacing[3];
		this->myInput->GetDimensions(dim);
		this->myInput->GetSpacing(spacing);
		cout << "found spacing:" << spacing[0] << "," << spacing[1] << "," << spacing[2] << endl;
		this->setSpacing(spacing[0], spacing[1], spacing[2]);
		this->setDimensions(dim[0], dim[1], dim[2]);
		this->setDataType(this->myInput->GetScalarType());
		
	}
	
}

void mqOpenTiff3DDialog::setDimensions(int dimX, int dimY, int dimZ)
{
	this->myDimX = dimX;
	this->myDimY = dimY;
	this->myDimZ = dimZ;
	this->Ui->dimX->setValue(dimX);
	this->Ui->dimY->setValue(dimY);
	this->Ui->dimZ->setValue(dimZ);
}
void mqOpenTiff3DDialog::setSpacing(double spacingX, double spacingY, double spacingZ)
{
	this->mySpacingX = spacingX;
	this->mySpacingY = spacingY;
	this->mySpacingZ = spacingZ;
	this->Ui->voxelSizeX->setValue(spacingX);
	this->Ui->voxelSizeY->setValue(spacingY);
	this->Ui->voxelSizeZ->setValue(spacingZ);

}
void mqOpenTiff3DDialog::setDataType(int dataType)
{
	this->myDataType = dataType;
	
	if (dataType == VTK_UNSIGNED_CHAR) { this->Ui->DataType->setText("8 bits unsigned");  }
	else if (dataType  == VTK_SHORT) { this->Ui->DataType->setText("16 bits signed"); }
	else if (dataType == VTK_UNSIGNED_SHORT) { this->Ui->DataType->setText("16 bits UNsigned"); }
	else if (dataType == VTK_FLOAT) { this->Ui->DataType->setText("32 bits float"); }
	else if (dataType == VTK_DOUBLE) { this->Ui->DataType->setText("64 bits double"); }
}

void mqOpenTiff3DDialog::slotOpenTiff3D()
{
	this->OpenTiff3D();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

