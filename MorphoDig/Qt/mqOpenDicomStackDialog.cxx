/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqOpenDicomStackDialog.h"
#include "ui_mqOpenDicomStackDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkDICOMImageReader.h>
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
mqOpenDicomStackDialog::mqOpenDicomStackDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqOpenDicomStackDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqOpenDicomStackDialog");	
	
	this->myFileName = "";
	this->inputAsStack = 1;
	this->myDimX = 0;
	this->myDimY = 0;
	this->myDimZ = 0;
	this->mySpacingX = 1;
	this->mySpacingY = 1;
	this->mySpacingZ = 1;
	this->myInput = vtkSmartPointer<vtkImageData>::New();
  this->Ui->voxelSizeX->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeY->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->dimX->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->dimY->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->dimZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
 
  this->Ui->voxelSizeX->setValue(1);
  this->Ui->voxelSizeY->setValue(1);
  this->Ui->voxelSizeZ->setValue(1);
 
  
  connect(this->Ui->voxelSizeX, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeXChanged(double))); 
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOpenDicomStack()));

}




//-----------------------------------------------------------------------------
mqOpenDicomStackDialog::~mqOpenDicomStackDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqOpenDicomStackDialog::setInputAsStack()
{
	this->inputAsStack = 1;
}
void mqOpenDicomStackDialog::setInputAs3DFile()
{
	this->inputAsStack = 0;
}
void mqOpenDicomStackDialog::OpenDicomStack()
{
	cout << "OpenDicomStack dialog" << endl;
	
		std::string action = "OpenDicomStack";
		
		if (this->Ui->voxelSizeX->value() == 0 || this->Ui->voxelSizeY->value() == 0|| this->Ui->voxelSizeZ->value() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Voxel size is 0 in x,y or z. Proceed anyway ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}
		


		
			mqMorphoDigCore::instance()->Open2DStack(this->myInput,
				this->Ui->ObjectName->text(),				
				this->Ui->voxelSizeX->value(),
				this->Ui->voxelSizeY->value(),
				this->Ui->voxelSizeZ->value(),
				this->Ui->frontToBack->isChecked()
				);
		
		
	
}

void mqOpenDicomStackDialog::set2DStackInput(vtkSmartPointer<vtkImageData> input)
{
	this->myInput = input;
}


void mqOpenDicomStackDialog::setFileName(QString fileName)
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
		vtkSmartPointer <vtkDICOMImageReader> dcmReader = vtkSmartPointer<vtkDICOMImageReader>::New();
		dcmReader->SetFileName(fileName.toLocal8Bit());
		//tiffReader->GetF
		dcmReader->Update();
		this->myInput = dcmReader->GetOutput();
		int dim[3];
		double spacing[3];
		this->myInput->GetDimensions(dim);
		this->myInput->GetSpacing(spacing);
		this->setDimensions(dim[0], dim[1], dim[2]);
		this->setDataType(this->myInput->GetScalarType());
		this->setSpacing(spacing[0], spacing[1], spacing[2]);
	}
	
}
void mqOpenDicomStackDialog::setSpacing(double spacingX, double spacingY, double spacingZ)
{

	this->mySpacingX = spacingX;
	this->mySpacingY = spacingY;
	this->mySpacingZ = spacingZ;
	this->Ui->voxelSizeX->setValue(spacingX);
	this->Ui->voxelSizeX->setValue(spacingY);
	this->Ui->voxelSizeX->setValue(spacingZ);
	
}

void mqOpenDicomStackDialog::setDimensions(int dimX, int dimY, int dimZ)
{
	this->myDimX = dimX;
	this->myDimY = dimY;
	this->myDimZ = dimZ;
	this->Ui->dimX->setValue(dimX);
	this->Ui->dimY->setValue(dimY);
	this->Ui->dimZ->setValue(dimZ);
}

void mqOpenDicomStackDialog::setDataType(int dataType)
{
	this->myDataType = dataType;
	
	if (dataType == VTK_UNSIGNED_CHAR) { this->Ui->DataType->setText("8 bits unsigned");  }
	else if (dataType  == VTK_SHORT) { this->Ui->DataType->setText("16 bits signed"); }
	else if (dataType == VTK_UNSIGNED_SHORT) { this->Ui->DataType->setText("16 bits UNsigned"); }
	else if (dataType == VTK_FLOAT) { this->Ui->DataType->setText("32 bits float"); }
	else if (dataType == VTK_DOUBLE) { this->Ui->DataType->setText("64 bits double"); }
}

void mqOpenDicomStackDialog::slotOpenDicomStack()
{
	this->OpenDicomStack();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

