/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsDistanceDialog.h"
#include "ui_mqScalarsDistanceDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


// we actually do not need glew...
//#include <GL/glew.h>
#include <vtkLandmarkTransform.h>
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
mqScalarsDistanceDialog::mqScalarsDistanceDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsDistanceDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsDistanceDialog");	
	
	
	impactedActor = NULL;
	observedActor=NULL;
	this->Ui->progressBar->setVisible(false);
	this->Ui->maxDist->setMaximum(DBL_MAX);
	this->Ui->maxDist->setSingleStep(1);
	this->Ui->maxDist->setValue(2);
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
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(slotLaunchDistance()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(mqMorphoDigCore::instance(), SIGNAL(distanceProgression(int)), this, SLOT(slotProgressBar(int)));
}




//-----------------------------------------------------------------------------
mqScalarsDistanceDialog::~mqScalarsDistanceDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsDistanceDialog::launchDistance()
{
	cout << "Launch scalar distance computation " << endl;
	this->Ui->cancel->setDisabled(true);
	
	
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
			cout << "impacted actor is null" << endl;
			this->impactedActor = NULL;
		}


		if (this->observedActor != NULL && this->impactedActor != NULL && this->observedActor!= this->impactedActor)
		{
			cout << "Call mqmorphodigcore scalarsDistance" << endl;
		
			mqMorphoDigCore::instance()->scalarsDistance(this->Ui->maxDist->value(), this->Ui->avg->value(), this->Ui->scalarName->text(), this->impactedActor, this->observedActor);// to launch scalarDistance computation
		}
		
	

}


void mqScalarsDistanceDialog::slotProgressBar(int val)
{
	this->Ui->progressBar->setValue(val);
}


void mqScalarsDistanceDialog::slotClose()
{
	
	this->close();
}


void mqScalarsDistanceDialog::slotLaunchDistance()
{
	//cout << "Set visible true!!!" << endl;
	this->Ui->progressBar->setVisible(true);
	this->launchDistance();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

