/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsComplexityDialog.h"
#include "ui_mqScalarsComplexityDialog.h"
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
mqScalarsComplexityDialog::mqScalarsComplexityDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsComplexityDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsComplexityDialog");	
	this->Ui->localAreaLimit->setMinimum(0);
	this->Ui->localAreaLimit->setMaximum(DBL_MAX);
	this->Ui->localAreaLimit->setSingleStep(0.1);
	double localAreaLimit = mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLengthOfSelectedActors() / 20;
	this->Ui->localAreaLimit->setValue(localAreaLimit);
	this->Ui->progressBar->setVisible(false);
	
	
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditComplexity()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditComplexity()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(this->Ui->customLocalAreaLimit, SIGNAL(pressed()), this, SLOT(slotEnableDisableCustomArea()));
	 connect(mqMorphoDigCore::instance(), SIGNAL(complexityProgression(int)), this, SLOT(slotProgressBar(int)));
}




//-----------------------------------------------------------------------------
mqScalarsComplexityDialog::~mqScalarsComplexityDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsComplexityDialog::editComplexity()
{
	cout << "Edit Complexity" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update Complexity";
		
		mqMorphoDigCore::instance()->scalarsComplexity(this->Ui->localAreaLimit->value(), this->Ui->customLocalAreaLimit->isChecked(), this->Ui->scalarName->text(), this->Ui->convexHullArea->isChecked());// to update Complexity
		
	}

}

void mqScalarsComplexityDialog::slotEnableDisableCustomArea()
{
	if (this->Ui->customLocalAreaLimit->isChecked())
	{
		this->Ui->localAreaLimit->setDisabled(true);
	}
	else
	{
		this->Ui->localAreaLimit->setDisabled(false);
	}

}
void mqScalarsComplexityDialog::slotProgressBar(int val)
{
	this->Ui->progressBar->setValue(val);
}


void mqScalarsComplexityDialog::slotClose()
{
	
	this->close();
}


void mqScalarsComplexityDialog::sloteditComplexity()
{
	//cout << "Set visible true!!!" << endl;
	this->Ui->progressBar->setVisible(true);
	this->editComplexity();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

