/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqBooleanOperationDialog.h"
#include "ui_mqBooleanOperationDialog.h"
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
mqBooleanOperationDialog::mqBooleanOperationDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqBooleanOperationDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqBooleanOperationDialog");	

	
	
	actorA = NULL;
	actorB=NULL;
	std::vector<std::string> myList = mqMorphoDigCore::instance()->getActorNames();
	for (int i = 0; i < myList.size(); i++)
	{
		QString newName = QString(myList.at(i).c_str());
		this->Ui->surfaceB->addItem(newName);
		this->Ui->surfaceA->addItem(newName);

	}
	if (myList.size() > 0)
	{
		this->Ui->surfaceB->setCurrentIndex(0);
		this->Ui->surfaceA->setCurrentIndex(0);

	}
	if (myList.size() > 1)
	{
		this->Ui->surfaceB->setCurrentIndex(1);
		

	}
	connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditBooleanOperation()));
	 
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 
}




//-----------------------------------------------------------------------------
mqBooleanOperationDialog::~mqBooleanOperationDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqBooleanOperationDialog::editBooleanOperation()
{
	cout << "Edit BooleanOperation" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		std::string action = "Update ThicknessBetween";
		if (!this->Ui->surfaceB->currentText().isEmpty())
		{
			this->actorB = mqMorphoDigCore::instance()->getFirstActorFromName(this->Ui->surfaceB->currentText());
			//cout << "Just set the observed actor" << endl;
		}
		else
		{
			this->actorB = NULL;
			//cout << "Observed actor is null" << endl;
		}
		if (!this->Ui->surfaceA->currentText().isEmpty())
		{
			this->actorA = mqMorphoDigCore::instance()->getFirstActorFromName(this->Ui->surfaceA->currentText());
			//cout << "Just set the impacted actor" << endl;
		}
		else
		{
			//cout << "impacted actor is null" << endl;
			this->actorA = NULL;
		}
		if (this->actorB != NULL && this->actorA != NULL)
		{
		//	cout << "Call mphodigcore between function" << endl;
			int mode = 0; //0 difference // 1 union // 2 intersection

			if (this->Ui->difference_2->isChecked()) { mode = 0; }
			if (this->Ui->union_2->isChecked()) { mode = 1; }
			if (this->Ui->intersection_2->isChecked()) { mode = 2; }
			mqMorphoDigCore::instance()->BooleanOperation(this->actorA, this->actorB, mode);// to launch the process
		}
		
	}

}





void mqBooleanOperationDialog::slotClose()
{
	
	this->close();
}


void mqBooleanOperationDialog::sloteditBooleanOperation()
{
	//cout << "Set visible true!!!" << endl;
	this->editBooleanOperation();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

