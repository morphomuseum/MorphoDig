/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsVolumeDialog.h"
#include "ui_mqScalarsVolumeDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


// we actually do not need glew...
//#include <GL/glew.h>
//#include <vtkCurvatures.h>
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
mqScalarsVolumeDialog::mqScalarsVolumeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsVolumeDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsVolumeDialog");	
	
	
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditVolume()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditVolume()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(this->Ui->globalVolume, SIGNAL(pressed()), this, SLOT(slotglobalVolume()));
	 connect(this->Ui->decomposeVolume, SIGNAL(pressed()), this, SLOT(slotdecomposeVolume()));
	 connect(this->Ui->computeVolume, SIGNAL(pressed()), this, SLOT(slotcomputeVolume()));
	 connect(this->Ui->computeArea, SIGNAL(pressed()), this, SLOT(slotcomputeArea()));
}




//-----------------------------------------------------------------------------
mqScalarsVolumeDialog::~mqScalarsVolumeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsVolumeDialog::editVolume()
{
	cout << "Compute Volume Scalar" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update Scalar";
		int volType = 0;
		if (this->Ui->globalVolume->isChecked())
		{
			if (this->Ui->computeVolume->isChecked())
			{
				volType = 0;
			}
			else
			{
				volType = 2;
			}
		}
		else if (this->Ui->decomposeVolume->isChecked())
		{
			if (this->Ui->computeVolume->isChecked())
			{
				volType = 1;
			}
			else
			{
				volType = 3;
			}
		}
		int minSize = 10;
		if (this->Ui->minsize->value() > 3)
		{
			minSize = this->Ui->minsize->value();
		}
		mqMorphoDigCore::instance()->scalarsAreaVolume(volType, minSize , this->Ui->scalarName->text());// to compute curvatures
		
	}

}

void mqScalarsVolumeDialog::slotglobalVolume()
{
	if (this->Ui->computeVolume->isChecked())
	{
		this->Ui->scalarName->setText("Volume_global");
	}
	else		
	{
			this->Ui->scalarName->setText("Area_global");
	}
	this->Ui->minsize->setDisabled(true);
}
void mqScalarsVolumeDialog::slotdecomposeVolume()
{
	if (this->Ui->computeVolume->isChecked())
	{
		this->Ui->scalarName->setText("Volume_regions");
	}
	else
	{
		this->Ui->scalarName->setText("Area_regions");
	}
	this->Ui->minsize->setDisabled(false);

}
void mqScalarsVolumeDialog::slotcomputeVolume()
{
	if (this->Ui->globalVolume->isChecked())
	{
		this->Ui->scalarName->setText("Volume_global");
	}
	else
	{
		this->Ui->scalarName->setText("Volume_regions");
	}

}
void mqScalarsVolumeDialog::slotcomputeArea()
{
	if (this->Ui->globalVolume->isChecked())
	{
		this->Ui->scalarName->setText("Area_global");
	}
	else
	{
		this->Ui->scalarName->setText("Area_regions");
	}

}



void mqScalarsVolumeDialog::slotClose()
{
	
	this->close();
}


void mqScalarsVolumeDialog::sloteditVolume()
{
	//cout << "Set visible true!!!" << endl;
	this->editVolume();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

