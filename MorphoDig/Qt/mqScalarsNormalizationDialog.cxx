/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsNormalizationDialog.h"
#include "ui_mqScalarsNormalizationDialog.h"
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
mqScalarsNormalizationDialog::mqScalarsNormalizationDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsNormalizationDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsNormalizationDialog");	
	
	
  
	 connect(this->Ui->reinitialize_minmax, SIGNAL(pressed()), this, SLOT(slotReinitializeMinMax()));
	 connect(this->Ui->customMinMax, SIGNAL(pressed()), this, SLOT(slotcustomMinMax()));
	 connect(this->Ui->removePercent, SIGNAL(pressed()), this, SLOT(slotremovePercent()));

	 
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditNormalization()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	
	
}




//-----------------------------------------------------------------------------
mqScalarsNormalizationDialog::~mqScalarsNormalizationDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsNormalizationDialog::editNormalization()
{
	cout << "Edit Normalization" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update Normalization";
		int curvType = 0;
		mqMorphoDigCore::instance()->scalarsNormalization(this->Ui->scalarName->text(), this->Ui->oldMin->value(), this->Ui->oldMax->value(), this->Ui->newMin->value(), this->Ui->newMax->value(), this->Ui->allowMin->isChecked(), this->Ui->allowMax->isChecked(), this->Ui->flip->isChecked());
				
	}

}

void mqScalarsNormalizationDialog::slotReinitializeMinMax()
{}
void mqScalarsNormalizationDialog::slotcustomMinMax()
{}
void mqScalarsNormalizationDialog::slotremovePercent()
{}

void mqScalarsNormalizationDialog::slotClose()
{
	
	this->close();
}


void mqScalarsNormalizationDialog::sloteditNormalization()
{
	//cout << "Set visible true!!!" << endl;
	this->editNormalization();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

