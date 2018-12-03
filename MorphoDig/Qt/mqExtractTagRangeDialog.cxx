/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqExtractTagRangeDialog.h"
#include "ui_mqExtractTagRangeDialog.h"
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
mqExtractTagRangeDialog::mqExtractTagRangeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqExtractTagRangeDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqExtractTagRangeDialog");
	
	

	this->Ui->min->setMinimum(0);
	this->Ui->max->setMinimum(0);
	

	this->Ui->min->setMaximum(INT_MAX);
	this->Ui->max->setMaximum(INT_MAX);
	
	QString ActiveScalar = mqMorphoDigCore::instance()->Getmui_ActiveScalars()->Name;
	this->Ui->activeScalarName->setText(ActiveScalar.toStdString().c_str());
	
	
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotExtractTagRange()));

}




//-----------------------------------------------------------------------------
mqExtractTagRangeDialog::~mqExtractTagRangeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}


void mqExtractTagRangeDialog::slotExtractTagRange()
{
	
	mqMorphoDigCore::instance()->Extract_Tag_Range(this->Ui->min->value(), this->Ui->max->value());
	mqMorphoDigCore::instance()->Render();
}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

