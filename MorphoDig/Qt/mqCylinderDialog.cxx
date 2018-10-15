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
	
	double cylinderHeight = mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLength()/20;
	double cylinderRadius = cylinderHeight/5;

	this->Ui->cylinderRadius->setValue(cylinderRadius);
	this->Ui->cylinderHeight->setValue(cylinderHeight);
	
		
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditCylinder()));
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
	
		
		
		mqMorphoDigCore::instance()->Cylinder(this->Ui->cylinderNumber->value(), this->Ui->cylinderHeight->value(), this->Ui->cylinderRadius->value(), this->Ui->cylinderResolution->value());
		
	
}



void mqCylinderDialog::slotClose()
{
	
	this->close();
}


void mqCylinderDialog::sloteditCylinder()
{
	//cout << "Set visible true!!!" << endl;

	this->editCylinder();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

