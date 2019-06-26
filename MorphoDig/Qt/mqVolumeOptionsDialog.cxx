/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqVolumeOptionsDialog.h"
#include "ui_mqVolumeOptionsDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkRenderer.h>
// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
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
mqVolumeOptionsDialog::mqVolumeOptionsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqVolumeOptionsDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqVolumeOptionsDialog");	
	long long int outOfCoreThreshold = mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold();
	this->Ui->outOfCoreThreshold->setValue(1000);
	this->Ui->outOfCoreThreshold->setMinimum(1000);
	//this->Ui->outOfCoreThreshold->setMaximum(LLONG_MAX);
	this->Ui->outOfCoreThreshold->setSingleStep(1000000);
	cout << "current outOfCoreThreshold =" << outOfCoreThreshold << endl;
	this->Ui->outOfCoreThreshold->setValue(outOfCoreThreshold);

	int volumeDisplaySlice = mqMorphoDigCore::instance()->Getmui_VolumeDisplaySlice();
	
	if (volumeDisplaySlice == 0) { this->Ui->VR->setChecked(true); }
	else if (volumeDisplaySlice == 1) { this->Ui->XY->setChecked(true); }
	else if (volumeDisplaySlice == 2) { this->Ui->XZ->setChecked(true); }
	else if (volumeDisplaySlice == 3) { this->Ui->YZ->setChecked(true); }
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotEditVolumeOptions()));
	 connect(this->Ui->reinit, SIGNAL(clicked()), this, SLOT(slotReinitialize()));
	 	 
	
}




//-----------------------------------------------------------------------------
mqVolumeOptionsDialog::~mqVolumeOptionsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqVolumeOptionsDialog::editVolumeOptions()
{
	
	if (this->Ui->VR->isChecked()) { mqMorphoDigCore::instance()->Setmui_VolumeDisplaySlice(0); }
	else if (this->Ui->XY->isChecked()) { mqMorphoDigCore::instance()->Setmui_VolumeDisplaySlice(1); }
	else if (this->Ui->XZ->isChecked()) { mqMorphoDigCore::instance()->Setmui_VolumeDisplaySlice(2); }
	else if (this->Ui->YZ->isChecked()) { mqMorphoDigCore::instance()->Setmui_VolumeDisplaySlice(3);
	cout << "YZ checked!!!" << endl;
	}
	
	mqMorphoDigCore::instance()->Setmui_VolumeOutOfCoreThreshold(this->Ui->outOfCoreThreshold->value());
	
}

void mqVolumeOptionsDialog::slotEditVolumeOptions()
{
	this->editVolumeOptions();
}

void mqVolumeOptionsDialog::slotReinitialize()
{

	long long int outOfCoreThreshold = mqMorphoDigCore::instance()->Getmui_DefaultVolumeOutOfCoreThreshold();

	cout << "default outOfCoreThreshold =" << outOfCoreThreshold << endl;
	this->Ui->outOfCoreThreshold->setValue(outOfCoreThreshold);
	int volumeDisplaySlice = mqMorphoDigCore::instance()->Getmui_DefaultVolumeDisplaySlice();

	if (volumeDisplaySlice == 0) { this->Ui->VR->setChecked(true); }
	else if (volumeDisplaySlice == 1) { this->Ui->XY->setChecked(true); }
	else if (volumeDisplaySlice == 2) { this->Ui->XZ->setChecked(true); }
	else if (volumeDisplaySlice == 3) { this->Ui->YZ->setChecked(true); }

}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

