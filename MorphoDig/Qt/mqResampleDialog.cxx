/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqResampleDialog.h"
#include "ui_mqResampleDialog.h"
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
mqResampleDialog::mqResampleDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqResampleDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqResampleDialog");	
	this->Ui->voxelSizeX->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->voxelSizeY->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->voxelSizeZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->voxelSizeX->setValue(1);
	this->Ui->voxelSizeY->setValue(1);
	this->Ui->voxelSizeZ->setValue(1);
	this->Ui->dimX->setValue(1);
	this->Ui->dimY->setValue(1);
	this->Ui->dimZ->setValue(1);
	this->mySpacingX = 1;
	this->mySpacingY = 1;
	this->mySpacingZ = 1;
	this->myDimX = 1;
	this->myDimY = 1;
	this->myDimZ = 1;
	this->mySizeX = this->mySizeY = this->mySizeZ = 1;
	this->myVolume = NULL;
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotResample()));
 this->reconnectDims();
 this->reconnectRes();
 connect(this->Ui->mult10, SIGNAL(clicked()), this, SLOT(slotMult10()));
	connect(this->Ui->mult5, SIGNAL(clicked()), this, SLOT(slotMult5()));
	connect(this->Ui->mult2, SIGNAL(clicked()), this, SLOT(slotMult2()));
	connect(this->Ui->div10, SIGNAL(clicked()), this, SLOT(slotDiv10()));
	connect(this->Ui->div5, SIGNAL(clicked()), this, SLOT(slotDiv5()));
	connect(this->Ui->div2, SIGNAL(clicked()), this, SLOT(slotDiv2()));
	
}




//-----------------------------------------------------------------------------
mqResampleDialog::~mqResampleDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqResampleDialog::disconnectDims() {
	disconnect(this->Ui->dimX, SIGNAL(valueChanged(int)), this, SLOT(slotDimXChanged(int)));
	disconnect(this->Ui->dimY, SIGNAL(valueChanged(int)), this, SLOT(slotDimYChanged(int)));
	disconnect(this->Ui->dimZ, SIGNAL(valueChanged(int)), this, SLOT(slotDimZChanged(int)));
}
void mqResampleDialog::reconnectDims() {
	connect(this->Ui->dimX, SIGNAL(valueChanged(int)), this, SLOT(slotDimXChanged(int)));
	connect(this->Ui->dimY, SIGNAL(valueChanged(int)), this, SLOT(slotDimYChanged(int)));
	connect(this->Ui->dimZ, SIGNAL(valueChanged(int)), this, SLOT(slotDimZChanged(int)));

}
void mqResampleDialog::disconnectRes() {
	disconnect(this->Ui->voxelSizeX, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeXChanged(double)));
	disconnect(this->Ui->voxelSizeY, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeYChanged(double)));
	disconnect(this->Ui->voxelSizeZ, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeZChanged(double)));
}
void mqResampleDialog::reconnectRes() {
	connect(this->Ui->voxelSizeX, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeXChanged(double)));
	connect(this->Ui->voxelSizeY, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeYChanged(double)));
	connect(this->Ui->voxelSizeZ, SIGNAL(valueChanged(double)), this, SLOT(slotVoxelSizeZChanged(double)));
}

void mqResampleDialog::setVolume (vtkMDVolume *vol)
{
	this->disconnectDims();
	this->disconnectRes();
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
	double spacing[3];
	vol->GetImageData()->GetSpacing(spacing);
	this->mySpacingX = spacing[0];
	this->mySpacingY = spacing[1];
	this->mySpacingZ = spacing[2];
	this->Ui->voxelSizeX->setValue(spacing[0]);
	this->Ui->voxelSizeY->setValue(spacing[1]);
	this->Ui->voxelSizeZ->setValue(spacing[2]);
	int dim[3];
	vol->GetImageData()->GetDimensions(dim);
	this->myDimX = dim[0];
	this->myDimY = dim[1];
	this->myDimZ = dim[2];
	this->Ui->dimX->setValue(dim[0]);
	this->Ui->dimY->setValue(dim[1]);
	this->Ui->dimZ->setValue(dim[2]);
	if (this->mySpacingX > 0) { this->mySizeX = (double)(this->mySpacingX *(double)this->myDimX); }
	if (this->mySpacingY > 0) { this->mySizeY = (double)(this->mySpacingY *(double)this->myDimY); }
	if (this->mySpacingZ > 0) { this->mySizeZ = (double)(this->mySpacingZ *(double)this->myDimZ); }
	cout << "Sizes:" << this->mySizeX << "," << this->mySizeY << "," << this->mySizeZ << endl;
	this->reconnectDims();
	this->reconnectRes();
}

void mqResampleDialog::Resample()
{
	if (this->myVolume != NULL)
	{
		cout << "Resample dialog" << endl;

		std::string action = "Resample volume";

		if (this->Ui->voxelSizeX->value() == 0 || this->Ui->voxelSizeY->value() == 0 || this->Ui->voxelSizeZ->value() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Voxel size is 0 in x,y or z. Proceed anyway ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }
		}
		int method = 0;
		if (this->Ui->linear->isChecked()) { method = 1; }
		if (this->Ui->neighbor->isChecked()) { method = 1; }
		if (this->Ui->cubic->isChecked()) { method = 2; }
		this->myVolume->Resample(this->Ui->voxelSizeX->value() ,this->Ui->voxelSizeY->value() ,this->Ui->voxelSizeZ->value(), method);
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}


void mqResampleDialog::slotResample()
{
	this->Resample();
}

void mqResampleDialog::slotVoxelSizeXChanged(double newVoxelSizeX)
{
	//size = dim*vox
	// vox has changed => dim = siz/vox
	this->disconnectDims();
	this->disconnectRes();
	this->Ui->voxelSizeY->setValue(newVoxelSizeX);
	this->Ui->voxelSizeZ->setValue(newVoxelSizeX);
	int newDimX = (int)(this->mySizeX/newVoxelSizeX );
	cout << "New dimX" << newDimX << endl;
	cout << "old dimX" << this->myDimX << endl;
	this->Ui->dimX->setValue((int)(this->mySizeX/ newVoxelSizeX));
	this->Ui->dimY->setValue((int)(this->mySizeY/ newVoxelSizeX));
	this->Ui->dimZ->setValue((int)(this->mySizeZ/newVoxelSizeX ));
	this->reconnectDims();
	this->reconnectRes();
}
void mqResampleDialog::slotVoxelSizeYChanged(double newVoxelSizeY)
{
	this->disconnectDims();
	this->Ui->dimY->setValue((int)(this->mySizeY/newVoxelSizeY ));
	this->reconnectDims();
}
void mqResampleDialog::slotVoxelSizeZChanged(double newVoxelSizeZ)
{
	this->disconnectDims();
	this->Ui->dimY->setValue((int)(this->mySizeZ/ newVoxelSizeZ));
	this->reconnectDims();
}
void mqResampleDialog::slotDimXChanged(int newDimX)
{
	//size = dim*vox
	// dim has changed => vox = size/dim
	this->disconnectRes();
	this->Ui->voxelSizeX->setValue((double)(this->mySizeX/(double)newDimX));
	this->reconnectRes();
}
void mqResampleDialog::slotDimYChanged(int newDimY)
{
	this->disconnectRes();
	this->Ui->voxelSizeY->setValue((double)(this->mySizeY / (double)newDimY));
	this->reconnectRes();
}
void mqResampleDialog::slotDimZChanged(int newDimZ)
{
	this->disconnectRes();
	this->Ui->voxelSizeZ->setValue((double)(this->mySizeZ / (double)newDimZ));
	this->reconnectRes();
}



void mqResampleDialog::slotMult10()
{
	this->disconnectRes();
	this->disconnectDims();
	double vX = this->Ui->voxelSizeX->value() * 10;
	double vY = this->Ui->voxelSizeY->value() * 10;
	double vZ = this->Ui->voxelSizeZ->value() * 10;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->Ui->dimX->setValue((int)(this->mySizeX/vX));
	this->Ui->dimY->setValue((int)(this->mySizeY/vY));
	this->Ui->dimZ->setValue((int)(this->mySizeZ/vZ));
	this->reconnectRes();
	this->reconnectDims();

}
void mqResampleDialog::slotDiv10()
{
	this->disconnectRes();
	this->disconnectDims();
	double vX = this->Ui->voxelSizeX->value() / 10;
	double vY = this->Ui->voxelSizeY->value() / 10;
	double vZ = this->Ui->voxelSizeZ->value() / 10;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->Ui->dimX->setValue((int)(this->mySizeX / vX));
	this->Ui->dimY->setValue((int)(this->mySizeY / vY));
	this->Ui->dimZ->setValue((int)(this->mySizeZ / vZ));
	this->reconnectRes();
	this->reconnectDims();
	

}
void mqResampleDialog::slotMult5()
{
	this->disconnectRes();
	this->disconnectDims();
	double vX = this->Ui->voxelSizeX->value() * 5;
	double vY = this->Ui->voxelSizeY->value() * 5;
	double vZ = this->Ui->voxelSizeZ->value() * 5;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->Ui->dimX->setValue((int)(this->mySizeX / vX));
	this->Ui->dimY->setValue((int)(this->mySizeY / vY));
	this->Ui->dimZ->setValue((int)(this->mySizeZ / vZ));
	this->reconnectRes();
	this->reconnectDims();

}

void mqResampleDialog::slotDiv5()
{
	this->disconnectRes();
	this->disconnectDims();
	double vX = this->Ui->voxelSizeX->value() / 5;
	double vY = this->Ui->voxelSizeY->value() / 5;
	double vZ = this->Ui->voxelSizeZ->value() / 5;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->Ui->dimX->setValue((int)(this->mySizeX / vX));
	this->Ui->dimY->setValue((int)(this->mySizeY / vY));
	this->Ui->dimZ->setValue((int)(this->mySizeZ / vZ));
	this->reconnectRes();
	this->reconnectDims();

}
void mqResampleDialog::slotMult2()
{
	this->disconnectRes();
	this->disconnectDims();
	double vX = this->Ui->voxelSizeX->value() * 2;
	double vY = this->Ui->voxelSizeY->value() * 2;
	double vZ = this->Ui->voxelSizeZ->value() * 2;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->Ui->dimX->setValue((int)(this->mySizeX / vX));
	this->Ui->dimY->setValue((int)(this->mySizeY / vY));
	this->Ui->dimZ->setValue((int)(this->mySizeZ / vZ));
	this->reconnectRes();
	this->reconnectDims();

}
void mqResampleDialog::slotDiv2()
{
	this->disconnectRes();
	this->disconnectDims();
	double vX = this->Ui->voxelSizeX->value() / 2;
	double vY = this->Ui->voxelSizeY->value() / 2;
	double vZ = this->Ui->voxelSizeZ->value() / 2;
	this->Ui->voxelSizeX->setValue(vX);
	this->Ui->voxelSizeY->setValue(vY);
	this->Ui->voxelSizeZ->setValue(vZ);
	this->Ui->dimX->setValue((int)(this->mySizeX / vX));
	this->Ui->dimY->setValue((int)(this->mySizeY / vY));
	this->Ui->dimZ->setValue((int)(this->mySizeZ / vZ));
	this->reconnectRes();
	this->reconnectDims();

}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

