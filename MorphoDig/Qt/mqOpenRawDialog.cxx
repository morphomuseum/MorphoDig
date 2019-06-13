/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqOpenRawDialog.h"
#include "ui_mqOpenRawDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QString>

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
mqOpenRawDialog::mqOpenRawDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqOpenRawDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqOpenRawDialog");	
	
	this->myFileName = "";
  this->Ui->voxelSizeX->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeY->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->headerSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->requestedSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->fileSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
  
  this->Ui->comboDataType->addItem("8 bits unsigned");
  
  this->Ui->comboDataType->addItem("16 bits signed");
  this->Ui->comboDataType->addItem("16 bits unsigned");
  this->Ui->comboDataType->addItem("32 bits float");
  this->Ui->comboDataType->addItem("64 bits float");
  this->Ui->comboDataType->setCurrentIndex(0);
  /*
  QObject::connect(ui.currentMin, SIGNAL(editingFinished()), this, SLOT(slotCurrentMinEdited()));
  QObject::connect(ui.currentMax, SIGNAL(editingFinished()), this, SLOT(slotCurrentMaxEdited()));
  QObject::connect(ui.plottedMin, SIGNAL(editingFinished()), this, SLOT(slotPlottedMinEdited()));
  QObject::connect(ui.plottedMax, SIGNAL(editingFinished()), this, SLOT(slotPlottedMaxEdited()));*/
  connect(this->Ui->dimX, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  connect(this->Ui->dimY, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  connect(this->Ui->dimZ, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  connect(this->Ui->comboDataType, SIGNAL(activated(int)), this, SLOT(slotDataTypeChanged(int)));
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOpenRaw()));

}




//-----------------------------------------------------------------------------
mqOpenRawDialog::~mqOpenRawDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqOpenRawDialog::OpenRaw()
{
	cout << "OpenRaw dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes() ==1)
	{
		std::string action = "OpenRaw";
		
		//mqMorphoDigCore::instance()->OpenRawVolume(this->myFileName);
		
	}
}


void mqOpenRawDialog::slotDataTypeChanged(int newDataType)
{
	this->RecomputeRequested(newDataType, this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), this->Ui->headerSize->value());
}
void mqOpenRawDialog::slotRecomputeRequested()
{
	this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), this->Ui->headerSize->value());
}
void mqOpenRawDialog::RecomputeRequested(int dataType, int dimX, int dimY, int dimZ, int headerSize)
{
	int mult = 1;
	if (dataType == 0) { mult = 1; }
	if (dataType == 1) { mult = 2; }
	if (dataType == 2) { mult = 2; }
	if (dataType == 3) { mult = 4; }
	if (dataType == 4) { mult = 8; }

	long int requested = mult * dimX *dimY *dimZ + headerSize;
	this->Ui->requestedSize->setValue(requested);
	


}
void mqOpenRawDialog::setFileName(QString fileName)
{
	this->myFileName = fileName;
	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());
	this->Ui->fileSize->setValue(fileInfo.size());
	this->Ui->FileName->setText(onlyfilename);
	
		
	
}



void mqOpenRawDialog::slotOpenRaw()
{
	this->OpenRaw();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

