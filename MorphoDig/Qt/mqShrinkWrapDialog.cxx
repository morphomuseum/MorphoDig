/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqShrinkWrapDialog.h"
#include "ui_mqShrinkWrapDialog.h"
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
mqShrinkWrapDialog::mqShrinkWrapDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqShrinkWrapDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqShrinkWrapDialog");	

	
	
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
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 
}




//-----------------------------------------------------------------------------
mqShrinkWrapDialog::~mqShrinkWrapDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqShrinkWrapDialog::editShrinkWrap()
{
	cout << "Edit ShrinkWrap" << endl;
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
			mqMorphoDigCore::instance()->ShrinkWrap(this->Ui->iteration->value(), this->Ui->relaxation->value(), this->impactedActor, this->observedActor);// to launch the process
		}
		
	}

}





void mqShrinkWrapDialog::slotClose()
{
	
	this->close();
}


void mqShrinkWrapDialog::sloteditShrinkWrap()
{
	//cout << "Set visible true!!!" << endl;
	this->editShrinkWrap();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

