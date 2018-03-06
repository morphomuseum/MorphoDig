/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqOrientationLabelsDialog.h"
#include "ui_mqOrientationLabelsDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

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
mqOrientationLabelsDialog::mqOrientationLabelsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqOrientationLabelsDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqOrientationLabelsDialog");	
	this->Ui->zplus->setText(mqMorphoDigCore::instance()->Getmui_Z1Label());
	this->Ui->zminus->setText(mqMorphoDigCore::instance()->Getmui_Z2Label());
	this->Ui->yplus->setText(mqMorphoDigCore::instance()->Getmui_Y1Label());
	this->Ui->yminus->setText(mqMorphoDigCore::instance()->Getmui_Y2Label());
	this->Ui->xplus->setText(mqMorphoDigCore::instance()->Getmui_X1Label());
	this->Ui->xminus->setText(mqMorphoDigCore::instance()->Getmui_X2Label());

	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditOrientationLabels()));
	 connect(this->Ui->reinit, SIGNAL(clicked()), this, SLOT(slotReinitialize()));
}




//-----------------------------------------------------------------------------
mqOrientationLabelsDialog::~mqOrientationLabelsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqOrientationLabelsDialog::editOrientationLabels()
{
	mqMorphoDigCore::instance()->Setmui_Z1Label(this->Ui->zplus->text());
	mqMorphoDigCore::instance()->Setmui_Z2Label(this->Ui->zminus->text());
	mqMorphoDigCore::instance()->Setmui_Y1Label(this->Ui->yplus->text());
	mqMorphoDigCore::instance()->Setmui_Y2Label(this->Ui->yminus->text());
	mqMorphoDigCore::instance()->Setmui_X1Label(this->Ui->xplus->text());
	mqMorphoDigCore::instance()->Setmui_X2Label(this->Ui->xminus->text());
	mqMorphoDigCore::instance()->ResetOrientationHelperLabels();
}







void mqOrientationLabelsDialog::sloteditOrientationLabels()
{
	this->editOrientationLabels();
}
void mqOrientationLabelsDialog::slotReinitialize()
{

	this->Ui->zplus->setText(mqMorphoDigCore::instance()->Getmui_DefaultZ1Label());
	this->Ui->zminus->setText(mqMorphoDigCore::instance()->Getmui_DefaultZ2Label());
	this->Ui->yplus->setText(mqMorphoDigCore::instance()->Getmui_DefaultY1Label());
	this->Ui->yminus->setText(mqMorphoDigCore::instance()->Getmui_DefaultY2Label());
	this->Ui->xplus->setText(mqMorphoDigCore::instance()->Getmui_DefaultX1Label());
	this->Ui->xminus->setText(mqMorphoDigCore::instance()->Getmui_DefaultX2Label());

}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

