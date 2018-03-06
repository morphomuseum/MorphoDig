/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqGridSizeDialog.h"
#include "ui_mqGridSizeDialog.h"
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
mqGridSizeDialog::mqGridSizeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqGridSizeDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqGridSizeDialog");	
	double GridSpacing= mqMorphoDigCore::instance()->Getmui_GridSpacing();
	
	this->Ui->gridspacing->setMinimum(0.00000001);
	this->Ui->gridspacing->setMaximum(DBL_MAX);
	this->Ui->gridspacing->setSingleStep(1);
	this->Ui->gridspacing->setValue(GridSpacing);

	QString myUnit = mqMorphoDigCore::instance()->Getmui_SizeUnit();
	QString mm("mm");
	QString cm("cm");
	QString m("m");
	QString um("um");
	QString nm("nm");
	cout << "myUnit=" << myUnit.toStdString() << endl;
	cout << "mm=" << mm.toStdString() << endl;
	if (QString::compare(myUnit, mm, Qt::CaseInsensitive) == 0) { this->Ui->mm->setChecked(true); }
	if (QString::compare(myUnit, cm, Qt::CaseInsensitive) == 0) { this->Ui->cm->setChecked(true); }
	if (QString::compare(myUnit, um, Qt::CaseInsensitive) == 0) { this->Ui->um->setChecked(true); }
	if (QString::compare(myUnit, nm, Qt::CaseInsensitive) == 0) { this->Ui->nm->setChecked(true); }
	if (QString::compare(myUnit, m, Qt::CaseInsensitive) == 0) { this->Ui->m->setChecked(true); }
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditGridSize()));
	 connect(this->Ui->reinit, SIGNAL(clicked()), this, SLOT(slotReinitialize()));
}




//-----------------------------------------------------------------------------
mqGridSizeDialog::~mqGridSizeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqGridSizeDialog::editGridSize()
{
	QString mm("mm");
	QString cm("cm");
	QString m("m");
	QString um("um");
	QString nm("nm");
	if (this->Ui->mm->isChecked()) { mqMorphoDigCore::instance()->Setmui_SizeUnit(mm); }
	else if (this->Ui->um->isChecked()) { mqMorphoDigCore::instance()->Setmui_SizeUnit(um); }
	else if (this->Ui->m->isChecked()) { mqMorphoDigCore::instance()->Setmui_SizeUnit(m); }
	else if (this->Ui->cm->isChecked()) { mqMorphoDigCore::instance()->Setmui_SizeUnit(cm); }
	else if (this->Ui->nm->isChecked()) { mqMorphoDigCore::instance()->Setmui_SizeUnit(nm); }
	mqMorphoDigCore::instance()->Setmui_GridSpacing(this->Ui->gridspacing->value());
	mqMorphoDigCore::instance()->SetGridInfos();
}







void mqGridSizeDialog::sloteditGridSize()
{
	this->editGridSize();
}
void mqGridSizeDialog::slotReinitialize()
{
	double GridSpacing = mqMorphoDigCore::instance()->Getmui_DefaultGridSpacing();
	
	this->Ui->gridspacing->setValue(GridSpacing);

	QString myUnit = mqMorphoDigCore::instance()->Getmui_DefaultSizeUnit();
	QString mm("mm");
	QString cm("cm");
	QString m("m");
	QString um("um");
	QString nm("nm");
	if (QString::compare(myUnit, mm, Qt::CaseInsensitive) == 0) { this->Ui->mm->setChecked(true); }
	if (QString::compare(myUnit, cm, Qt::CaseInsensitive) == 0) { this->Ui->cm->setChecked(true); }
	if (QString::compare(myUnit, um, Qt::CaseInsensitive) == 0) { this->Ui->um->setChecked(true); }
	if (QString::compare(myUnit, nm, Qt::CaseInsensitive) == 0) { this->Ui->nm->setChecked(true); }
	if (QString::compare(myUnit, m, Qt::CaseInsensitive) == 0) { this->Ui->m->setChecked(true); }

}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

