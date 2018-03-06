/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSmoothDialog.h"
#include "ui_mqSmoothDialog.h"
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
mqSmoothDialog::mqSmoothDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSmoothDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqSmoothDialog");	
	
	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSmooth()));

}




//-----------------------------------------------------------------------------
mqSmoothDialog::~mqSmoothDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSmoothDialog::Smooth()
{
	cout << "Smooth dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Smooth selected actors";
		
		mqMorphoDigCore::instance()->addSmooth(this->Ui->iteration->value(), this->Ui->relaxation->value());
		
	}
}







void mqSmoothDialog::slotSmooth()
{
	this->Smooth();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

