/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqDensifyDialog.h"
#include "ui_mqDensifyDialog.h"
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
mqDensifyDialog::mqDensifyDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqDensifyDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqDensifyDialog");	
	
	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotDensify()));

}




//-----------------------------------------------------------------------------
mqDensifyDialog::~mqDensifyDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqDensifyDialog::Densify()
{
	cout << "Densify dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Densify selected actors";
		
		mqMorphoDigCore::instance()->addDensify(this->Ui->subdivisions->value());
		
	}
}







void mqDensifyDialog::slotDensify()
{
	this->Densify();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

