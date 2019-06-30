/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqFlipSwapDialog.h"
#include "ui_mqFlipSwapDialog.h"
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
mqFlipSwapDialog::mqFlipSwapDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqFlipSwapDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqFlipSwapDialog");	
	
	this->myVolume = NULL;
  
 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotFlipX()));
	connect(this->Ui->flipX, SIGNAL(clicked()), this, SLOT(slotFlipX()));
	connect(this->Ui->flipY, SIGNAL(clicked()), this, SLOT(slotFlipY()));
	connect(this->Ui->flipZ, SIGNAL(clicked()), this, SLOT(slotFlipZ()));
	connect(this->Ui->swapXY, SIGNAL(clicked()), this, SLOT(slotSwapXY()));
	connect(this->Ui->swapXZ, SIGNAL(clicked()), this, SLOT(slotSwapXZ()));
	connect(this->Ui->swapYZ, SIGNAL(clicked()), this, SLOT(slotSwapYZ()));
	connect(this->Ui->swapXYZ, SIGNAL(clicked()), this, SLOT(slotSwapXYZ()));
}




//-----------------------------------------------------------------------------
mqFlipSwapDialog::~mqFlipSwapDialog()
{

 //depending on what is 
	
  delete this->Ui;
}



void mqFlipSwapDialog::setVolume (vtkMDVolume *vol)
{
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
	
}



void mqFlipSwapDialog::slotFlipX()
{
	cout << "Slot Flip X" << endl;
	if (this->myVolume != NULL)
	{
		this->myVolume->FlipX();
		mqMorphoDigCore::instance()->Render();
	}
	
}
void mqFlipSwapDialog::slotFlipY()
{
	if (this->myVolume != NULL)
	{
		this->myVolume->FlipY();
		mqMorphoDigCore::instance()->Render();
	}

}
void mqFlipSwapDialog::slotFlipZ()
{
	if (this->myVolume != NULL)
	{
		this->myVolume->FlipZ();
		mqMorphoDigCore::instance()->Render();
	}

}
void mqFlipSwapDialog::slotSwapXY()
{
	if (this->myVolume != NULL)
	{
		this->myVolume->SwapXY();
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}
void mqFlipSwapDialog::slotSwapXZ()
{
	if (this->myVolume != NULL)
	{
		this->myVolume->SwapXZ();
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}
void mqFlipSwapDialog::slotSwapYZ()
{
	if (this->myVolume != NULL)
	{
		this->myVolume->SwapYZ();
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}
void mqFlipSwapDialog::slotSwapXYZ()
{
	if (this->myVolume != NULL)
	{
		this->myVolume->SwapXYZ();
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

