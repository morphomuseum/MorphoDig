/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqCameraDialog.h"
#include "ui_mqCameraDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QCheckBox>
#include <QHeaderView>
#include <QFileDialog>
#include <sstream>

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
mqCameraDialog::mqCameraDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqCameraDialog())
{
  this->Ui->setupUi(this);
  this->setObjectName("mqCameraDialog");


  this->Ui->crNear->setMaximum(DBL_MAX);
  this->Ui->crNear->setMinimum(-DBL_MAX);
  this->Ui->crFar->setMaximum(DBL_MAX);
  this->Ui->crFar->setMinimum(-DBL_MAX);
  this->Ui->centerX->setMaximum(DBL_MAX);
  this->Ui->centerX->setMinimum(-DBL_MAX);
  this->Ui->centerY->setMaximum(DBL_MAX);
  this->Ui->centerY->setMinimum(-DBL_MAX);
  this->Ui->centerZ->setMaximum(DBL_MAX);
  this->Ui->centerZ->setMinimum(-DBL_MAX);

  this->Ui->focalX->setMaximum(DBL_MAX);
  this->Ui->focalX->setMinimum(-DBL_MAX);
  this->Ui->focalY->setMaximum(DBL_MAX);
  this->Ui->focalY->setMinimum(-DBL_MAX);
  this->Ui->focalZ->setMaximum(DBL_MAX);
  this->Ui->focalZ->setMinimum(-DBL_MAX);

  this->Ui->upX->setMaximum(DBL_MAX);
  this->Ui->upX->setMinimum(-DBL_MAX);
  this->Ui->upY->setMaximum(DBL_MAX);
  this->Ui->upY->setMinimum(-DBL_MAX);
  this->Ui->upZ->setMaximum(DBL_MAX);
  this->Ui->upZ->setMinimum(-DBL_MAX);

  this->RefreshDialog();

 /* connect(this->Ui->crNear, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->crFar, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->centerX, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->centerY, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->centerZ, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->focalX, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->focalY, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->focalZ, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->upX, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->upY, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));
  connect(this->Ui->upZ, SIGNAL(valueChanged(double)), this, SLOT(slotCameraParameterChanged()));*/
  connect(this->Ui->applyButton, SIGNAL(pressed()), this, SLOT(slotApplyCameraParameters()));
  connect(this->Ui->openCam, SIGNAL(pressed()), this, SLOT(slotOpenCAM()));
  connect(this->Ui->saveCam, SIGNAL(pressed()), this, SLOT(slotSaveCAM()));
  connect(this->Ui->refreshCam, SIGNAL(pressed()), this, SLOT(slotRefresh()));
  //mqMorphoDigCore::instance()->Getmui_FlagColor(flagcolor);
  


//  this->Ui->FlagColorButton->setChosenColor(myFlagColor);
 // connect(this->Ui->FlagColorButton, SIGNAL(colorChosen()), this, SLOT(slotFlagColorChanged()));
 


 // connect(this->Ui->AdjustScaleFactor, SIGNAL(valueChanged(double)), this, SLOT(slotAdjustScaleFactorChanged()));

}




//-----------------------------------------------------------------------------
mqCameraDialog::~mqCameraDialog()
{
	
	
  delete this->Ui;
}
void mqCameraDialog::RefreshDialog()
{
	double cr[2];
	double position[3];
	double focal[3];
	double up[3];

	mqMorphoDigCore::instance()->GetCAMParameters(cr, position, focal, up);
	this->Ui->crNear->setValue(cr[0]);
	this->Ui->crFar->setValue(cr[1]);
	this->Ui->centerX->setValue(position[0]);
	this->Ui->centerY->setValue(position[1]);
	this->Ui->centerZ->setValue(position[2]);
	this->Ui->focalX->setValue(focal[0]);
	this->Ui->focalY->setValue(focal[1]);
	this->Ui->focalZ->setValue(focal[2]);
	this->Ui->upX->setValue(up[0]);
	this->Ui->upY->setValue(up[1]);
	this->Ui->upZ->setValue(up[2]);

	

}
void mqCameraDialog::slotOpenCAM()
{
	QString fileName = QFileDialog::getOpenFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Load camera file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("camera file (*.cam)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMorphoDigCore::instance()->OpenCAM(fileName);
	mqMorphoDigCore::instance()->Render();
}
void mqCameraDialog::slotSaveCAM(){
	QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Save CAM file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + "camera",
		tr("CAM file (*.cam)"), NULL
		//, QFileDialog::DontConfirmOverwrite
	);


	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	//Save and applies position 
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Save camera" << endl;
	double cNear, cFar, cX, cY, cZ, fX, fY, fZ, uX, uY, uZ;
	cNear = this->Ui->crNear->value();
	cFar = this->Ui->crFar->value();
	cX = this->Ui->centerX->value();
	cY = this->Ui->centerY->value();
	cZ = this->Ui->centerZ->value();
	uX = this->Ui->upX->value();
	uY = this->Ui->upY->value();
	uZ = this->Ui->upZ->value();
	fX = this->Ui->focalX->value();
	fY = this->Ui->focalY->value();
	fZ = this->Ui->focalZ->value();
	
	mqMorphoDigCore::instance()->SaveCAM(fileName,cNear, cFar, cX, cY, cZ, fX, fY, fZ, uX, uY, uZ);

}
void mqCameraDialog::slotReinitialize()
{
}
void mqCameraDialog::slotRefresh() 
{
	cout << "Refresh" << endl;
	this->RefreshDialog();
}

void mqCameraDialog::slotApplyCameraParameters()
{
	double cNear, cFar, cX, cY, cZ, fX, fY, fZ, uX, uY, uZ;
	cNear = this->Ui->crNear->value();
	cFar = this->Ui->crFar->value();
	cX = this->Ui->centerX->value();
	cY = this->Ui->centerY->value();
	cZ = this->Ui->centerZ->value();
	uX = this->Ui->upX->value();
	uY = this->Ui->upY->value();
	uZ = this->Ui->upZ->value();
	fX = this->Ui->focalX->value();
	fY = this->Ui->focalY->value();
	fZ = this->Ui->focalZ->value();
	mqMorphoDigCore::instance()->SetCAM(cNear,cFar,cX,cY,cZ,fX,fY,fZ,uX,uY,uZ);
	mqMorphoDigCore::instance()->Render();
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

