/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqShrinkWrapIterativeDialog.h"
#include "ui_mqShrinkWrapIterativeDialog.h"
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
mqShrinkWrapIterativeDialog::mqShrinkWrapIterativeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqShrinkWrapIterativeDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqShrinkWrapIterativeDialog");	
	this->Ui->searchSize->setMinimum(0);
	this->Ui->searchSize->setMaximum(DBL_MAX);
	this->Ui->searchSize->setSingleStep(1);


	this->Ui->searchSize->setValue(2);
	
	double proposed_value = mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLengthOfSelectedActors() / 20;
	this->Ui->searchSize->setValue(proposed_value);

	this->Ui->progressBar->setVisible(false);
	//this->Ui->smoothNormals->setChecked(true);
	
	
	
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
	connect(this->Ui->FixedIterationNumber, SIGNAL(pressed()), this, SLOT(slotFixedIterationNumber()));
	connect(this->Ui->IterateUntilLimit, SIGNAL(pressed()), this, SLOT(slotIterateUntilLimit()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditIterativeShrinkWrap()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(this->Ui->observedObject, SIGNAL(activated(int)), this, SLOT(slotObservedObjectChanged(int)));
	 connect(mqMorphoDigCore::instance(), SIGNAL(iterativeShrinkWrapProgression(int)), this, SLOT(slotProgressBar(int)));
}




//-----------------------------------------------------------------------------
mqShrinkWrapIterativeDialog::~mqShrinkWrapIterativeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqShrinkWrapIterativeDialog::editIterativeShrinkWrap()
{
	cout << "Edit Iterative Shrink Wrap" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		std::string action = "Update Iterative Shrink Wrap";
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
		//	cout << "Call mqmorphodigcore iterative shrink wrap function" << endl;
			int mode = 0;
			if (this->Ui->FixedIterationNumber->isChecked() )
			{
				mode = 0;
			}
			else
			{
				mode = 1;
			}
			double searchSize = this->Ui->searchSize->value();
			if (searchSize <= 0) { searchSize = 1; }
			double maxStepAmplitude = searchSize;
			if (this->Ui->X->value() > 0) {
				maxStepAmplitude = searchSize / this->Ui->X->value();
			}

			double stopCriterion = 1;
			if (this->Ui->Y->value() > 0) {
				stopCriterion = searchSize / this->Ui->Y->value();
			}

			mqMorphoDigCore::instance()->ShrinkWrapIterative(this->Ui->scalarName->text(), mode, this->Ui->iterations->value(), stopCriterion, searchSize, this->Ui->angularLimit->value(), maxStepAmplitude, this->impactedActor, this->observedActor, this->Ui->smooth->isChecked());
			
		}
		
	}

}
 void mqShrinkWrapIterativeDialog::slotObservedObjectChanged(int idx)
{
	 if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	 {
		 std::string action = "Update observed object in list";
		 if (!this->Ui->observedObject->currentText().isEmpty())
		 {
			 this->observedActor = mqMorphoDigCore::instance()->getFirstActorFromName(this->Ui->observedObject->currentText());
			 double searchSize  = this->observedActor->GetXYZAvgPCLength() / 20;
			 this->Ui->searchSize->setValue(searchSize);
			 //cout << "Just set the observed actor" << endl;
		 }
	 }
 
 }

void mqShrinkWrapIterativeDialog::slotIterateUntilLimit()
{
	if (this->Ui->FixedIterationNumber->isChecked()) { this->Ui->iterations->setEnabled(false); }
	else { this->Ui->iterations->setEnabled(true); }

}
void mqShrinkWrapIterativeDialog::slotFixedIterationNumber()
{
	if (this->Ui->IterateUntilLimit->isChecked()) { this->Ui->Y->setEnabled(false); }
	else { this->Ui->Y->setEnabled(true); }

}
void mqShrinkWrapIterativeDialog::slotProgressBar(int val)
{
	this->Ui->progressBar->setValue(val);
}


void mqShrinkWrapIterativeDialog::slotClose()
{
	
	this->close();
}


void mqShrinkWrapIterativeDialog::sloteditIterativeShrinkWrap()
{
	//cout << "Set visible true!!!" << endl;
	this->Ui->progressBar->setVisible(true);
	this->editIterativeShrinkWrap();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

