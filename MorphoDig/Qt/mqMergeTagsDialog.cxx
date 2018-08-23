/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqMergeTagsDialog.h"
#include "ui_mqMergeTagsDialog.h"
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
mqMergeTagsDialog::mqMergeTagsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqMergeTagsDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqMergeTagsDialog");	
	
	
	idSourceTags = 0;
	idTargetTags = 0;
	
	std::vector<std::string> myList = mqMorphoDigCore::instance()->Getmui_ActiveTagMap()->tagNames;
	for (int i = 0; i < myList.size(); i++)
	{
		QString newName = QString(myList.at(i).c_str());
		this->Ui->sourceTags->addItem(newName);
		this->Ui->targetTags->addItem(newName);

	}
	if (myList.size() > 0)
	{
		this->Ui->sourceTags->setCurrentIndex(1);
		this->Ui->targetTags->setCurrentIndex(1);

	}
	
	int activeTag = mqMorphoDigCore::instance()->Getmui_ActiveTag();
	if (activeTag<myList.size())
	{
		this->Ui->sourceTags->setCurrentIndex(activeTag);
	}
	//connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditThickness()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(slotLaunchMergeTags()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 
}




//-----------------------------------------------------------------------------
mqMergeTagsDialog::~mqMergeTagsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqMergeTagsDialog::launchMergeTags()
{
	cout << "Launch MergeTags " << endl;
	this->Ui->cancel->setDisabled(true);
	
	
		if (!this->Ui->sourceTags->currentText().isEmpty())
		{
			this->idSourceTags = this->Ui->sourceTags->currentIndex();
				cout << "source current index:" << this->idTargetTags << endl;
		}
		else
		{
			this->idSourceTags = 0;
			//cout << "Observed actor is null" << endl;
		}
		if (!this->Ui->targetTags->currentText().isEmpty())
		{
			this->idTargetTags = this->Ui->targetTags->currentIndex();
			cout << "target current index:" << this->idTargetTags << endl;
			
		}
		else
		{
			//cout << "impacted actor is null" << endl;
			this->idTargetTags = 0;
		}


		if (this->idTargetTags != this->idSourceTags)
		{
		
			mqMorphoDigCore::instance()->MergeTags(this->idSourceTags, this->idTargetTags);// to launch MergeTags
		}
		
	

}


void mqMergeTagsDialog::slotClose()
{
	
	this->close();
}


void mqMergeTagsDialog::slotLaunchMergeTags()
{
	//cout << "Set visible true!!!" << endl;
	
	this->launchMergeTags();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

