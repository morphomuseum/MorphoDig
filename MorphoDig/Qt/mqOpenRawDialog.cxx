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
  
  this->Ui->headerSize->setMaximum(DBL_MAX);
  this->Ui->requestedSize->setMaximum(DBL_MAX);
  this->Ui->fileSize->setMaximum(DBL_MAX);

  this->Ui->dimX->setValue(mqMorphoDigCore::instance()->Getmui_RawDimX());
  this->Ui->dimY->setValue(mqMorphoDigCore::instance()->Getmui_RawDimY());
  this->Ui->dimZ->setValue(mqMorphoDigCore::instance()->Getmui_RawDimZ());

  this->Ui->voxelSizeX->setValue(mqMorphoDigCore::instance()->Getmui_VoxelSizeX());
  double headerSize = mqMorphoDigCore::instance()->Getmui_RawHeader();
  this->Ui->headerSize->setValue(headerSize);
  this->Ui->voxelSizeY->setValue(mqMorphoDigCore::instance()->Getmui_VoxelSizeY());
  this->Ui->voxelSizeZ->setValue(mqMorphoDigCore::instance()->Getmui_VoxelSizeZ());
  this->Ui->comboDataType->addItem("8 bits unsigned");
  
  this->Ui->comboDataType->addItem("16 bits signed");
  this->Ui->comboDataType->addItem("16 bits unsigned");
  this->Ui->comboDataType->addItem("32 bits float");
  this->Ui->comboDataType->addItem("64 bits double");
  this->Ui->comboDataType->setCurrentIndex(mqMorphoDigCore::instance()->Getmui_RawDataType());
  int backToFront= mqMorphoDigCore::instance()->Getmui_BackToFront();
  if (backToFront == 1) {
	  this->Ui->backToFront->setChecked(true);
  }
  else
  {
	  this->Ui->frontToBack->setChecked(true);
  }
  int littleEndian = mqMorphoDigCore::instance()->Getmui_RawLittleEndian();
  if (littleEndian == 1) {
	  this->Ui->littleEndian->setChecked(true);
  }
  else
  {
	  this->Ui->bigEndian->setChecked(true);
  }

  this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), headerSize);
  /*
  QObject::connect(ui.currentMin, SIGNAL(editingFinished()), this, SLOT(slotCurrentMinEdited()));
  QObject::connect(ui.currentMax, SIGNAL(editingFinished()), this, SLOT(slotCurrentMaxEdited()));
  QObject::connect(ui.plottedMin, SIGNAL(editingFinished()), this, SLOT(slotPlottedMinEdited()));
  QObject::connect(ui.plottedMax, SIGNAL(editingFinished()), this, SLOT(slotPlottedMaxEdited()));*/
  //connect(this->Ui->dimX, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  connect(this->Ui->dimX, SIGNAL(valueChanged(int)), this, SLOT(slotDimXChanged(int)));
  connect(this->Ui->dimY, SIGNAL(valueChanged(int)), this, SLOT(slotDimYChanged(int)));
  connect(this->Ui->dimZ, SIGNAL(valueChanged(int)), this, SLOT(slotDimZChanged(int)));
  
  //connect(this->Ui->dimY, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  //connect(this->Ui->dimZ, SIGNAL(editingFinished()), this, SLOT(slotRecomputeRequested()));
  connect(this->Ui->comboDataType, SIGNAL(activated(int)), this, SLOT(slotDataTypeChanged(int)));
  
  connect(this->Ui->voxelSizeX, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeXChanged(double)));
  connect(this->Ui->voxelSizeY, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeYChanged(double)));
  connect(this->Ui->voxelSizeZ, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeZChanged(double)));
  
  connect(this->Ui->bigEndian, SIGNAL(clicked(bool)), this, SLOT(slotBigEndianClicked(bool)));
  connect(this->Ui->littleEndian, SIGNAL(clicked(bool)), this, SLOT(slotLittleEndianClicked(bool)));
  connect(this->Ui->backToFront, SIGNAL(clicked(bool)), this, SLOT(slotBackToFrontClicked(bool)));
  connect(this->Ui->frontToBack, SIGNAL(clicked(bool)), this, SLOT(slotFrontToBackClicked(bool)));
  connect(this->Ui->headerSize, SIGNAL(valueChanged(double)), this, SLOT(slotHeaderSizeChanged(double)));

  
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
	
		std::string action = "OpenRaw";
		if (this->Ui->fileSize->value()>0 && this->Ui->requestedSize->value()!= this->Ui->fileSize->value())
		{
		QMessageBox msgBox;
		msgBox.setText("Requested size differs from file size. Proceed anyway ?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}
		if (this->Ui->voxelSizeX->value() == 0 || this->Ui->voxelSizeY->value() == 0|| this->Ui->voxelSizeZ->value() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Voxel size is 0 in x,y or z. Proceed anyway ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}
		int cType = this->Ui->comboDataType->currentIndex();
		int dataType = VTK_CHAR;
		if (cType == 0) { dataType = VTK_UNSIGNED_CHAR; }
		else if (cType == 1) { dataType = VTK_SHORT; }
		else if (cType == 2) { dataType = VTK_UNSIGNED_SHORT; }
		else if (cType == 3) { dataType = VTK_FLOAT; }
		else if (cType == 4) { dataType = VTK_DOUBLE; }



		mqMorphoDigCore::instance()->OpenRawVolume(this->myFileName,
			this->Ui->ObjectName->text(),
			dataType,
			this->Ui->dimX->value(),
			this->Ui->dimY->value(), 
			this->Ui->dimZ->value(),
			this->Ui->headerSize->value(),
			this->Ui->voxelSizeX->value(), 
			this->Ui->voxelSizeY->value(), 
			this->Ui->voxelSizeZ->value(), 
			this->Ui->bigEndian->isChecked(),
			this->Ui->frontToBack->isChecked());
		
		
		
	
}
void mqOpenRawDialog::slotBigEndianClicked(bool checked){

	mqMorphoDigCore::instance()->Setmui_RawLittleEndian(0);
}
void mqOpenRawDialog::slotLittleEndianClicked(bool isChecked)
{
	mqMorphoDigCore::instance()->Setmui_RawLittleEndian(1);
}
void mqOpenRawDialog::slotFrontToBackClicked(bool isChecked){

	mqMorphoDigCore::instance()->Setmui_BackToFront(0);
}
void mqOpenRawDialog::slotBackToFrontClicked(bool isChecked){

	mqMorphoDigCore::instance()->Setmui_BackToFront(1);
}
void mqOpenRawDialog::slotVoxelSizeYChanged(double newsVoxelSizeY)
{
	mqMorphoDigCore::instance()->Setmui_VoxelSizeY(newsVoxelSizeY);
}
void mqOpenRawDialog::slotVoxelSizeZChanged(double newsVoxelSizeZ)
{
	mqMorphoDigCore::instance()->Setmui_VoxelSizeZ(newsVoxelSizeZ);
}


void mqOpenRawDialog::slotVoxelSizeXChanged(double newsVoxelSizeX)
{
	this->Ui->voxelSizeY->setValue(newsVoxelSizeX);
	this->Ui->voxelSizeZ->setValue(newsVoxelSizeX);
	mqMorphoDigCore::instance()->Setmui_VoxelSizeX(newsVoxelSizeX);
	mqMorphoDigCore::instance()->Setmui_VoxelSizeY(newsVoxelSizeX);
	mqMorphoDigCore::instance()->Setmui_VoxelSizeZ(newsVoxelSizeX);
}
void mqOpenRawDialog::slotDataTypeChanged(int newDataType)
{
	mqMorphoDigCore::instance()->Setmui_RawDataType(newDataType);
	this->RecomputeRequested(newDataType, this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), this->Ui->headerSize->value());
}
void mqOpenRawDialog::slotDimXChanged(int newDimX)
{
	mqMorphoDigCore::instance()->Setmui_RawDimX(newDimX);
	this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), newDimX, this->Ui->dimY->value(), this->Ui->dimZ->value(), this->Ui->headerSize->value());
}
 void mqOpenRawDialog::slotDimYChanged(int newDimY)
 {
	 mqMorphoDigCore::instance()->Setmui_RawDimY(newDimY);
	 this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), newDimY, this->Ui->dimZ->value(), this->Ui->headerSize->value());
 }
 void mqOpenRawDialog::slotDimZChanged(int newDimZ)
 {
	 mqMorphoDigCore::instance()->Setmui_RawDimZ(newDimZ);
	 this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), this->Ui->dimY->value(), newDimZ, this->Ui->headerSize->value());
 }
 void mqOpenRawDialog::slotHeaderSizeChanged(double newHeaderSize)
 {
	 mqMorphoDigCore::instance()->Setmui_RawHeader(newHeaderSize);
	 this->RecomputeRequested(this->Ui->comboDataType->currentIndex(), this->Ui->dimX->value(), this->Ui->dimY->value(), this->Ui->dimZ->value(), newHeaderSize);
 }

void mqOpenRawDialog::RecomputeRequested(int dataType, int dimX, int dimY, int dimZ, double headerSize)
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
	


}
void mqOpenRawDialog::setFileName(QString fileName)
{
	this->myFileName = fileName;
	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());
	double fileSize = (double)fileInfo.size();
	this->Ui->fileSize->setValue(fileSize);
		
	std::string only_filename = onlyfilename.toStdString();
	std::string newname = only_filename.c_str();
	size_t nPos = newname.find_last_of(".");
	if (nPos > 0)
	{

		newname = newname.substr(0, nPos);
	}
	QString objectName = "";
	objectName = objectName + newname.c_str();
	//	double fileSize = (double)fileInfo.size();
	//	this->Ui->fileSize->setValue(fileSize);
	this->Ui->ObjectName->setText(objectName);

	
	
		
	
}



void mqOpenRawDialog::slotOpenRaw()
{
	this->OpenRaw();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

