/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqExtractScalarRangeDialog.h"
#include "ui_mqExtractScalarRangeDialog.h"
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
mqExtractScalarRangeDialog::mqExtractScalarRangeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqExtractScalarRangeDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqExtractScalarRangeDialog");
	
	
	this->Ui->min->setDecimals(10);
	this->Ui->max->setDecimals(10);
	
	this->Ui->min->setMinimum(-DBL_MAX);
	this->Ui->max->setMinimum(-DBL_MAX);
	

	this->Ui->min->setMaximum(DBL_MAX);
	this->Ui->max->setMaximum(DBL_MAX);
	int removePercent = this->Ui->removePercent->value();
	QString ActiveScalar = mqMorphoDigCore::instance()->Getmui_ActiveScalars()->Name;
	this->Ui->activeScalarName->setText(ActiveScalar.toStdString().c_str());
	this->Ui->max->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMax(removePercent, 1));
	this->Ui->min->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMin(removePercent, 1));
	connect(this->Ui->removePercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedMinMax()));
	
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotExtractScalarRange()));

}




//-----------------------------------------------------------------------------
mqExtractScalarRangeDialog::~mqExtractScalarRangeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqExtractScalarRangeDialog::slotRefreshSuggestedMinMax()
{
	int removePercent = this->Ui->removePercent->value();
	this->Ui->max->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMax(removePercent, 1));
	this->Ui->min->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMin(removePercent, 1));
	//this->RefreshSuggestedMinMax();
	cout << "DIAL refreshsuggestedRange" << endl;

}
void mqExtractScalarRangeDialog::slotExtractScalarRange()
{
	
	mqMorphoDigCore::instance()->Extract_Scalar_Range(this->Ui->min->value(), this->Ui->max->value());
	mqMorphoDigCore::instance()->Render();
}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

