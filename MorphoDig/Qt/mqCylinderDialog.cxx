/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqCylinderDialog.h"
#include "ui_mqCylinderDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QProgressBar>
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
mqCylinderDialog::mqCylinderDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqCylinderDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqCylinderDialog");	
	this->Ui->cylinderRadius->setMinimum(0);
	this->Ui->cylinderRadius->setMaximum(DBL_MAX);
	this->Ui->cylinderHeight->setMinimum(0);
	this->Ui->cylinderHeight->setMaximum(DBL_MAX);
	this->Ui->cylinderHeight->setMinimum(0);
	

	double cylinderHeight = 10; 
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		 cylinderHeight = mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLength() / 20;

	}
	double cylinderRadius = cylinderHeight/5;

	this->Ui->cylinderRadius->setValue(cylinderRadius);
	this->Ui->cylinderRadius2->setValue(cylinderRadius);
	this->Ui->cylinderHeight->setValue(cylinderHeight);
	this->Ui->cylinderRadius2->setDisabled(true);
	this->Ui->circular_shaft->setChecked(true);
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditCylinder()));
	connect(this->Ui->cone, SIGNAL(pressed()), this, SLOT(slotConePressed()));
	connect(this->Ui->cylinder, SIGNAL(pressed()), this, SLOT(slotCylinderPressed()));
	connect(this->Ui->circular_shaft, SIGNAL(pressed()), this, SLOT(slotCircularShaftPressed()));
	connect(this->Ui->elliptical_shaft, SIGNAL(pressed()), this, SLOT(slotEllipticalShaftPressed()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditCylinder()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 
	 
}



//-----------------------------------------------------------------------------
mqCylinderDialog::~mqCylinderDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqCylinderDialog::editCylinder()
{
	cout << "Create Cylinder" << endl;
	this->Ui->cancel->setDisabled(true);
	
		
		
		mqMorphoDigCore::instance()->Cylinder(this->Ui->cylinderNumber->value(), this->Ui->cylinderHeight->value(), this->Ui->cylinderRadius->value(), this->Ui->cylinderRadius2->value(), this->Ui->cylinderResolution->value(), this->Ui->coneHeight->value(), this->Ui->cone->isChecked(), this->Ui->circular_shaft->isChecked());
		
	
}



void mqCylinderDialog::slotClose()
{
	
	this->close();
}

void mqCylinderDialog::slotConePressed()
{
	if (this->Ui->cylinder->isChecked())
	{
		this->Ui->coneHeight->setDisabled(false);
	}
}
void mqCylinderDialog::slotCylinderPressed()
{
	if (this->Ui->cone->isChecked())
	{
		this->Ui->coneHeight->setDisabled(true);
	}
}

void mqCylinderDialog::slotCircularShaftPressed()
{
	if (this->Ui->elliptical_shaft->isChecked())
	{
		this->Ui->cylinderRadius2->setDisabled(true);
	}
}
void mqCylinderDialog::slotEllipticalShaftPressed()
{
	if (this->Ui->circular_shaft->isChecked())
	{
		this->Ui->cylinderRadius2->setDisabled(false);
	}
}

void mqCylinderDialog::sloteditCylinder()
{
	//cout << "Set visible true!!!" << endl;

	this->editCylinder();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

