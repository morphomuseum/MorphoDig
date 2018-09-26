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
	
	this->RefreshScalarName();
	this->Ui->oldMin->setMinimum(-DBL_MAX);
	this->Ui->oldMax->setMinimum(-DBL_MAX);	
	this->Ui->newMin->setMinimum(-DBL_MAX);
	this->Ui->newMax->setMinimum(-DBL_MAX);

	this->Ui->oldMin->setMaximum(DBL_MAX);
	this->Ui->oldMax->setMaximum(DBL_MAX);
	this->Ui->newMin->setMaximum(DBL_MAX);
	this->Ui->newMax->setMaximum(DBL_MAX);

	this->RefreshSuggestedOldMinMax();
	connect(mqMorphoDigCore::instance(), SIGNAL(activeScalarChanged()), this, SLOT(slotRefreshActiveScalars()));
	 connect(this->Ui->reinitialize_minmax, SIGNAL(pressed()), this, SLOT(slotRefreshSuggestedOldMinMax()));
	 connect(this->Ui->customMinMax, SIGNAL(pressed()), this, SLOT(slotcustomMinMax()));
	 connect(this->Ui->removePercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedOldMinMax()));

	 connect(this->Ui->customMinMax, SIGNAL(pressed()), this, SLOT(slotcustomMinMax()));
	 
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditNormalization()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 this->Ui->reinitialize_minmax->setVisible(false); // as this dialog is modal, no need to show this control for the moment!
	
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
void mqScalarsNormalizationDialog::RefreshScalarName()
{
	QString ActiveScalar = mqMorphoDigCore::instance()->Getmui_ActiveScalars()->Name;
	this->Ui->activeScalarName->setText(ActiveScalar.toStdString().c_str());
	QString SuggestedActiveScalarName = "Norm_" + ActiveScalar;
	this->Ui->scalarName->setText(SuggestedActiveScalarName.toStdString().c_str());
}
void mqScalarsNormalizationDialog::RefreshSuggestedOldMinMax()
{
	int removePercent = this->Ui->removePercent->value();
	this->Ui->oldMax->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMax(removePercent, 1));
	this->Ui->oldMin->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMin(removePercent, 1));

}

void mqScalarsNormalizationDialog::slotcustomMinMax()
{
	if (this->Ui->customMinMax->isChecked())
	{
		this->Ui->removePercent->setEnabled(true);
		this->Ui->oldMin->setEnabled(false);
		this->Ui->oldMax->setEnabled(false);
	}
	else
	{
		this->Ui->removePercent->setEnabled(false);
		this->Ui->oldMin->setEnabled(true);
		this->Ui->oldMax->setEnabled(true);

	}
}

void mqScalarsNormalizationDialog::slotRefreshSuggestedOldMinMax()
{
	this->RefreshSuggestedOldMinMax();
cout << "DIAL refreshsuggestedRange" << endl;

}

void mqScalarsNormalizationDialog::slotRefreshActiveScalars()
{
	this->RefreshScalarName();
	this->RefreshSuggestedOldMinMax();
}



void mqScalarsNormalizationDialog::slotremovePercent()
{
	this->RefreshSuggestedOldMinMax();
}

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

