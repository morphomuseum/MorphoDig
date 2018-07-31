/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqTagFromRGBDialog.h"
#include "ui_mqTagFromRGBDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
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
mqTagFromRGBDialog::mqTagFromRGBDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqTagFromRGBDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqTagFromRGBDialog");	
	
	
	connect(this->Ui->exact, SIGNAL(pressed()), this, SLOT(slotExact()));
	connect(this->Ui->firstN, SIGNAL(pressed()), this, SLOT(slotFirstN()));

	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotTagFromRGB()));

}




//-----------------------------------------------------------------------------
mqTagFromRGBDialog::~mqTagFromRGBDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqTagFromRGBDialog::TagFromRGB()
{
	cout << "TagFromRGB dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "TagFromRGB for selected actors";
		
		mqMorphoDigCore::instance()->createTagsFromRGB(this->Ui->newTags->text(), this->Ui->exact->isChecked(), this->Ui->N->value());
		
	}
}






void mqTagFromRGBDialog::slotExact()
{
	this->Ui->N->setDisabled(true);
}

void mqTagFromRGBDialog::slotFirstN()
{
	this->Ui->N->setDisabled(false);
}



void mqTagFromRGBDialog::slotTagFromRGB()
{
	this->TagFromRGB();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

