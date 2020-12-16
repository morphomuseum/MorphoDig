/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSelectSmallVolumesDialog.h"
#include "ui_mqSelectSmallVolumesDialog.h"
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
mqSelectSmallVolumesDialog::mqSelectSmallVolumesDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSelectSmallVolumesDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqSelectSmallVolumesDialog");	
	
	
	
	
	this->Ui->volume->setMinimum(0);

	this->Ui->volume->setMaximum(DBL_MAX);
	this->Ui->volume->setSingleStep(500);
	this->Ui->volume->setValue(1000);
	

	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSelectSmallVolumes()));

}




//-----------------------------------------------------------------------------
mqSelectSmallVolumesDialog::~mqSelectSmallVolumesDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSelectSmallVolumesDialog::SelectSmallVolumes()
{
	cout << "Edit alpha" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		std::string action = "Select small volumes";
		
		mqMorphoDigCore::instance()->SelectSmallVolumes(this->Ui->volume->value());// to update body size!
		
		mqMorphoDigCore::instance()->Render();
	}
}







void mqSelectSmallVolumesDialog::slotSelectSmallVolumes()
{
	this->SelectSmallVolumes();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

