/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSelectLandmarkRangeDialog.h"
#include "ui_mqSelectLandmarkRangeDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>

#include <QRadioButton>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

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
mqSelectLandmarkRangeDialog::mqSelectLandmarkRangeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSelectLandmarkRangeDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSelectLandmarkRangeDialog");
	
	
	// This is where we 
  //
	this->Ui->Start->setMinimum(1);
	this->Ui->Start->setMaximum(INT_MAX);
	this->Ui->End->setMinimum(1);
	this->Ui->End->setMaximum(INT_MAX);
	this->Ui->Start->setValue(1);
	this->Ui->End->setValue(1);

 this->Ui->Normal->setChecked(true);
 
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSelectLandmarkRange()));

}




//-----------------------------------------------------------------------------
mqSelectLandmarkRangeDialog::~mqSelectLandmarkRangeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSelectLandmarkRangeDialog::slotSelectLandmarkRange()
{
	
	
	int lm_type = 0;
	
	if (this->Ui->Normal->isChecked()) { lm_type = 0; }
	else if (this->Ui->Target->isChecked()) { lm_type = 1; }
	else if (this->Ui->Node->isChecked()) { lm_type = 2; }
	else if (this->Ui->Handle->isChecked()) { lm_type = 3; }
	int start_ind = std::min(this->Ui->Start->value(), this->Ui->End->value());
	int end_ind = std::max(this->Ui->Start->value(), this->Ui->End->value());
	
	

	mqMorphoDigCore::instance()->SelectLandmarkRange(start_ind, end_ind, lm_type);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

