/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsThicknessBetweenDialog.h"
#include "ui_mqScalarsThicknessBetweenDialog.h"
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
mqScalarsThicknessBetweenDialog::mqScalarsThicknessBetweenDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsThicknessBetweenDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsThicknessBetweenDialog");	
	this->Ui->thickness->setMinimum(0);
	this->Ui->thickness->setMaximum(DBL_MAX);
	this->Ui->thickness->setSingleStep(1);
	this->Ui->thickness->setValue(2);
	this->Ui->progressBar->setVisible(false);
	this->Ui->smoothNormals->setChecked(true);
	
	this->Ui->avg->setMinimum(1);
	this->Ui->avg->setValue(5);
	this->Ui->avg->setMaximum(1000);
	this->Ui->avg->setSingleStep(1);
	
	impactedActor = NULL;
	observedActor=NULL;
	std::vector<std::string> myList = mqMorphoDigCore::instance()->getActorNames();
	for (int i = 0; i < myList.size(); i++)
	{
		QString newName = QString(myList.at(i).c_str());
		this->Ui->observedObject->addItem(newName);
		this->Ui->impactedObject->addItem(newName);

	}
	if (myList.size() > 0)
	{
		this->Ui->observedObject->setCurrentIndex(0);
		this->Ui->impactedObject->setCurrentIndex(0);

	}
	if (myList.size() > 1)
	{
		this->Ui->observedObject->setCurrentIndex(1);
		

	}
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditThickness()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditThickness()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(mqMorphoDigCore::instance(), SIGNAL(thicknessProgression(int)), this, SLOT(slotProgressBar(int)));
}




//-----------------------------------------------------------------------------
mqScalarsThicknessBetweenDialog::~mqScalarsThicknessBetweenDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsThicknessBetweenDialog::editThickness()
{
	cout << "Edit ThicknessBetween" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update ThicknessBetween";
		if (!this->Ui->observedObject->currentText().isEmpty())
		{
			this->observedActor = mqMorphoDigCore::instance()->getFirstActorFromName(this->Ui->observedObject->currentText());
			//cout << "Just set the observed actor" << endl;
		}
		else
		{
			this->observedActor = NULL;
			//cout << "Observed actor is null" << endl;
		}
		if (!this->Ui->impactedObject->currentText().isEmpty())
		{
			this->impactedActor = mqMorphoDigCore::instance()->getFirstActorFromName(this->Ui->impactedObject->currentText());
			//cout << "Just set the impacted actor" << endl;
		}
		else
		{
			//cout << "impacted actor is null" << endl;
			this->impactedActor = NULL;
		}
		if (this->observedActor != NULL && this->impactedActor != NULL)
		{
		//	cout << "Call mphodigcore between function" << endl;
			mqMorphoDigCore::instance()->scalarsThicknessBetween(this->Ui->thickness->value(), this->Ui->smoothNormals->isChecked(), this->Ui->avg->value(), this->Ui->scalarName->text(), this->impactedActor, this->observedActor, this->Ui->angularLimit->value(), this->Ui->invertObservedNormals->isChecked());// to update thickness
		}
		
	}

}

void mqScalarsThicknessBetweenDialog::slotProgressBar(int val)
{
	this->Ui->progressBar->setValue(val);
}


void mqScalarsThicknessBetweenDialog::slotClose()
{
	
	this->close();
}


void mqScalarsThicknessBetweenDialog::sloteditThickness()
{
	//cout << "Set visible true!!!" << endl;
	this->Ui->progressBar->setVisible(true);
	this->editThickness();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

