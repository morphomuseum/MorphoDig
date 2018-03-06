/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqDecomposeDialog.h"
#include "ui_mqDecomposeDialog.h"
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
mqDecomposeDialog::mqDecomposeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqDecomposeDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqDecomposeDialog");	
	
	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotDecompose()));

}




//-----------------------------------------------------------------------------
mqDecomposeDialog::~mqDecomposeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqDecomposeDialog::Decompose()
{
	cout << "Decompose dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Decompose selected actors";
		//mode 0 : color = same as selected actor
		//mode 1 : color = random
		//mode 2 : color = follows tag lut

		int mode = 0; 
		if (this->Ui->same->isChecked())
		{
			mode = 0;
		}
		else if (this->Ui->random->isChecked())
		{
			mode = 1;

		}
		else if (this->Ui->tag->isChecked())
		{
			mode = 2;

		}

		mqMorphoDigCore::instance()->addDecompose(mode, this->Ui->minsize->value());
		
	}
}







void mqDecomposeDialog::slotDecompose()
{
	this->Decompose();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

