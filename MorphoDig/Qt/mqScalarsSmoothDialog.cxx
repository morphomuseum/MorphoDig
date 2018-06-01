/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsSmoothDialog.h"
#include "ui_mqScalarsSmoothDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QProgressBar>
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
mqScalarsSmoothDialog::mqScalarsSmoothDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsSmoothDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsSmoothDialog");	
	this->Ui->localAreaLimit->setMinimum(0);
	this->Ui->localAreaLimit->setMaximum(DBL_MAX);
	this->Ui->localAreaLimit->setSingleStep(0.1);
	double localAreaLimit = mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLengthOfSelectedActors()/40;
	this->Ui->localAreaLimit->setValue(localAreaLimit);
	this->Ui->progressBar->setVisible(false);
	
	
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditSmooth()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditSmooth()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(this->Ui->localCustom, SIGNAL(pressed()), this, SLOT(slotEnableDisableCustomArea()));
	 connect(mqMorphoDigCore::instance(), SIGNAL(smoothProgression(int)), this, SLOT(slotProgressBar(int)));
}




//-----------------------------------------------------------------------------
mqScalarsSmoothDialog::~mqScalarsSmoothDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsSmoothDialog::editSmooth()
{
	cout << "Edit Smooth" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Smooth scalars";
		int mode = 3;
		//mode = 0: raw smoothing (average of direct neighbours)
		//mode = 1: smooth within local sphere of radius ~ mesh avg size / 40
		//mode = 2: smooth within local sphere of radius defined by the user
		
		if (this->Ui->localNeighbours->isChecked()) { mode = 0; }
		if (this->Ui->localAuto->isChecked()) { mode = 1; }
		if (this->Ui->localCustom->isChecked()) { mode = 2; }
		
		mqMorphoDigCore::instance()->scalarsSmooth(this->Ui->localAreaLimit->value(), mode);// to update Smooth
		
	}

}

void mqScalarsSmoothDialog::slotEnableDisableCustomArea()
{
	if (this->Ui->localCustom->isChecked())
	{
		this->Ui->localAreaLimit->setDisabled(true);
	}
	else
	{
		this->Ui->localAreaLimit->setDisabled(false);
	}

}
void mqScalarsSmoothDialog::slotProgressBar(int val)
{
	this->Ui->progressBar->setValue(val);
}


void mqScalarsSmoothDialog::slotClose()
{
	
	this->close();
}


void mqScalarsSmoothDialog::sloteditSmooth()
{
	//cout << "Set visible true!!!" << endl;
	this->Ui->progressBar->setVisible(true);
	this->editSmooth();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

