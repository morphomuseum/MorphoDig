/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqCreateLMKDialog.h"
#include "ui_mqCreateLMKDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
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
mqCreateLMKDialog::mqCreateLMKDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqCreateLMKDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqCreateLMKDialog");
	
	
	this->Ui->x->setDecimals(10);
	this->Ui->y->setDecimals(10);
	this->Ui->z->setDecimals(10);
	this->Ui->x->setMinimum(-DBL_MAX);
	this->Ui->y->setMinimum(-DBL_MAX);
	this->Ui->z->setMinimum(-DBL_MAX);

	this->Ui->x->setMaximum(DBL_MAX);
	this->Ui->y->setMaximum(DBL_MAX);
	this->Ui->z->setMaximum(DBL_MAX);
	
	this->Ui->Normal->setChecked(true);
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotCreateLMK()));

}




//-----------------------------------------------------------------------------
mqCreateLMKDialog::~mqCreateLMKDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqCreateLMKDialog::slotCreateLMK()
{
	double coord[3];
	coord[0] = this->Ui->x->value();
	coord[1] = this->Ui->y->value();
	coord[2] = this->Ui->z->value();
	double ori[3] = { 0,0,1 };
	int lmk_type = 0;
	if (this->Ui->Normal->isChecked()) { lmk_type = 0; }
	else if (this->Ui->Target->isChecked()) { lmk_type = 1; }
	else if (this->Ui->Node->isChecked()) { lmk_type = 2; }
	else if (this->Ui->Handle->isChecked()) { lmk_type = 3; }
	mqMorphoDigCore::instance()->CreateLandmark(coord, ori, lmk_type);
	mqMorphoDigCore::instance()->Render();
}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

