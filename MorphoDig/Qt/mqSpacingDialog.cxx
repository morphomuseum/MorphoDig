/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSpacingDialog.h"
#include "ui_mqSpacingDialog.h"
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
mqSpacingDialog::mqSpacingDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSpacingDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqSpacingDialog");	
	this->Ui->voxelSizeX->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->voxelSizeY->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->voxelSizeZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->voxelSizeX->setValue(1);
	this->Ui->voxelSizeY->setValue(1);
	this->Ui->voxelSizeZ->setValue(1);
	this->mySpacingX = 1;
	this->mySpacingY = 1;
	this->mySpacingZ = 1;
	this->myVolume = NULL;
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSpacing()));
 connect(this->Ui->voxelSizeX, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeXChanged(double)));
	connect(this->Ui->mult10, SIGNAL(clicked()), this, SLOT(slotMult10()));
	connect(this->Ui->mult5, SIGNAL(clicked()), this, SLOT(slotMult5()));
	connect(this->Ui->mult2, SIGNAL(clicked()), this, SLOT(slotMult2()));
	connect(this->Ui->div10, SIGNAL(clicked()), this, SLOT(slotDiv10()));
	connect(this->Ui->div5, SIGNAL(clicked()), this, SLOT(slotDiv5()));
	connect(this->Ui->div2, SIGNAL(clicked()), this, SLOT(slotDiv2()));
	
}




//-----------------------------------------------------------------------------
mqSpacingDialog::~mqSpacingDialog()
{

 //depending on what is 
	
  delete this->Ui;
}


void mqSpacingDialog::setVolume (vtkMDVolume *vol)
{
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
	double spacing[3];
	vol->GetImageData()->GetSpacing(spacing);
	this->Ui->voxelSizeX->setValue(spacing[0]);
	this->Ui->voxelSizeY->setValue(spacing[1]);
	this->Ui->voxelSizeZ->setValue(spacing[2]);
}

void mqSpacingDialog::Spacing()
{
	if (this->myVolume != NULL)
	{
		cout << "Spacing dialog" << endl;

		std::string action = "Spacing";

		if (this->Ui->voxelSizeX->value() == 0 || this->Ui->voxelSizeY->value() == 0 || this->Ui->voxelSizeZ->value() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Voxel size is 0 in x,y or z. Proceed anyway ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}


		this->myVolume->ChangeSpacing(this->Ui->voxelSizeX->value() ,this->Ui->voxelSizeY->value() ,this->Ui->voxelSizeZ->value());
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}


void mqSpacingDialog::slotSpacing()
{
	this->Spacing();
}

void mqSpacingDialog::slotVoxelSizeXChanged(double newsVoxelSizeX)
{
	this->Ui->voxelSizeY->setValue(newsVoxelSizeX);
	this->Ui->voxelSizeZ->setValue(newsVoxelSizeX);
}
void mqSpacingDialog::slotMult10()
{
	double vX = this->Ui->voxelSizeX->value()*10;
	double vY = this->Ui->voxelSizeY->value()*10;
	double vZ = this->Ui->voxelSizeZ->value()*10;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->mySpacingX = vX;
	this->mySpacingY= vY;
	this->mySpacingZ = vZ;
	
}


void mqSpacingDialog::slotDiv10()
{
	double vX = this->Ui->voxelSizeX->value() / 10;
	double vY = this->Ui->voxelSizeY->value() / 10;
	double vZ = this->Ui->voxelSizeZ->value() / 10;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->mySpacingX = vX;
	this->mySpacingY = vY;
	this->mySpacingZ = vZ;

}
void mqSpacingDialog::slotMult5()
{
	double vX = this->Ui->voxelSizeX->value() * 5;
	double vY = this->Ui->voxelSizeY->value() * 5;
	double vZ = this->Ui->voxelSizeZ->value() * 5;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->mySpacingX = vX;
	this->mySpacingY = vY;
	this->mySpacingZ = vZ;

}

void mqSpacingDialog::slotDiv5()
{
	double vX = this->Ui->voxelSizeX->value() / 5;
	double vY = this->Ui->voxelSizeY->value() / 5;
	double vZ = this->Ui->voxelSizeZ->value() / 5;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->mySpacingX = vX;
	this->mySpacingY = vY;
	this->mySpacingZ = vZ;

}
void mqSpacingDialog::slotMult2()
{
	double vX = this->Ui->voxelSizeX->value() * 2;
	double vY = this->Ui->voxelSizeY->value() * 2;
	double vZ = this->Ui->voxelSizeZ->value() * 2;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->mySpacingX = vX;
	this->mySpacingY = vY;
	this->mySpacingZ = vZ;

}
void mqSpacingDialog::slotDiv2()
{
	double vX = this->Ui->voxelSizeX->value() / 2;
	double vY = this->Ui->voxelSizeY->value() / 2;
	double vZ = this->Ui->voxelSizeZ->value() / 2;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->mySpacingX = vX;
	this->mySpacingY = vY;
	this->mySpacingZ = vZ;

}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

