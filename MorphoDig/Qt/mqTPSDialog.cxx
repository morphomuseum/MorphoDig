/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqTPSDialog.h"
#include "ui_mqTPSDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"

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
mqTPSDialog::mqTPSDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqTPSDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqTPSDialog");	
	
	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotTPS()));

}




//-----------------------------------------------------------------------------
mqTPSDialog::~mqTPSDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqTPSDialog::TPS()
{
	cout << "TPS dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "TPS selected actors";
		
		mqMorphoDigCore::instance()->addTPS(this->Ui->r->isChecked(), this->Ui->factor->value(), this->Ui->all->isChecked());
		
	}
}







void mqTPSDialog::slotTPS()
{
	this->TPS();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

