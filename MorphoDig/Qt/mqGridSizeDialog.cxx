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
#include <vtkRenderer.h>
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
	this->Ui->hundredpxsu->setDisabled(true);
	QString myUnit = mqMorphoDigCore::instance()->Getmui_SizeUnit();

	//double HundredPxSU = mqMorphoDigCore::instance()->GetHundredPxSU();
	this->Ui->hundredpxsu->setMinimum(0.00000001);
	this->Ui->hundredpxsu->setMaximum(DBL_MAX);
	this->Ui->hundredpxsu->setSingleStep(1);
	this->Ui->hundredpxsu->setValue(100);
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
	 connect(this->Ui->hundredpxsu, SIGNAL(valueChanged(double)), this, SLOT(slotEditHundredPxSu()));
	 connect(mqMorphoDigCore::instance(), SIGNAL(zoomChanged()), this, SLOT(slotRefresh()));
	 connect(mqMorphoDigCore::instance(), SIGNAL(projectionModeChanged()), this, SLOT(slotRefresh()));

	 //connect(mqMorphoDigCore::instance(), SIGNAL(actorSelectionChanged()), this, SLOT(slotRefreshDialog()));
	
}




//-----------------------------------------------------------------------------
mqGridSizeDialog::~mqGridSizeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqGridSizeDialog::adjustParallelScale()
{
	
	if (mqMorphoDigCore::instance()->Getmui_CameraOrtho() == 1)
	{
		double currentScale = mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
		double currentHundredPxSU = mqMorphoDigCore::instance()->GetHundredPxSU();
		double newHundredPxSu = this->Ui->hundredpxsu->value();
		if (currentHundredPxSU>0 && newHundredPxSu>0)
		{
			double newScale = currentScale*newHundredPxSu / currentHundredPxSU;
			mqMorphoDigCore::instance()->getCamera()->SetParallelScale(newScale);
			// now change camera position
			mqMorphoDigCore::instance()->DollyCameraForPerspectiveMode();
			mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
			//change grid infos
			mqMorphoDigCore::instance()->SetGridInfos();
			mqMorphoDigCore::instance()->Render();
		}
		
		
		
	}

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
	this->adjustParallelScale();
	
	mqMorphoDigCore::instance()->SetGridInfos();
}




void mqGridSizeDialog::slotEditHundredPxSu()
{

	this->adjustParallelScale();
}

void mqGridSizeDialog::slotRefresh()
{
	this->Refresh();
}
void mqGridSizeDialog::slotEditGridSize()
{
	this->editGridSize();
}
void mqGridSizeDialog::Refresh()
{
	if (mqMorphoDigCore::instance()->Getmui_CameraOrtho() == 1)
	{
		if (this->Ui->hundredpxsu->isEnabled()==false)
		{
			this->Ui->hundredpxsu->setDisabled(false);
		}
		double HundredPxSU = mqMorphoDigCore::instance()->GetHundredPxSU();
		this->Ui->hundredpxsu->setValue(HundredPxSU);
	}
	else
	{
		if (this->Ui->hundredpxsu->isEnabled())
		{
			this->Ui->hundredpxsu->setDisabled(true);
		}
	}
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

