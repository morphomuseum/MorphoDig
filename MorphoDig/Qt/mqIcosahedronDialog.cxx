/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqCubeDialog.h"
#include "ui_mqCubeDialog.h"
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
mqCubeDialog::mqCubeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqCubeDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqCubeDialog");	
	this->Ui->sizeX->setMinimum(0);
	this->Ui->sizeX->setMaximum(DBL_MAX);
	this->Ui->sizeY->setMinimum(0);
	this->Ui->sizeY->setMaximum(DBL_MAX);
	this->Ui->sizeZ->setMinimum(0);
	this->Ui->sizeZ->setMaximum(DBL_MAX);
	
	

	double msizeY = 10; 
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		msizeY = mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLength() / 20;
				

	}
	double msizeX = msizeY /5;
	double msizeZ = msizeY / 5;

	this->Ui->sizeX->setValue(msizeX);
	this->Ui->sizeY->setValue(msizeY);
	this->Ui->sizeZ->setValue(msizeZ);
	
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditCube()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 
	 
}



//-----------------------------------------------------------------------------
mqCubeDialog::~mqCubeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqCubeDialog::editCube()
{
	cout << "Create Cube" << endl;
	this->Ui->cancel->setDisabled(true);
	
		
		
		mqMorphoDigCore::instance()->Cube(this->Ui->cubeNumber->value(), this->Ui->sizeX->value(), this->Ui->sizeY->value(), this->Ui->sizeZ->value());
		
	
}



void mqCubeDialog::slotClose()
{
	
	this->close();
}



void mqCubeDialog::sloteditCube()
{
	//cout << "Set visible true!!!" << endl;

	this->editCube();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

