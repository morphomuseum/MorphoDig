/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqIcosahedronDialog.h"
#include "ui_mqIcosahedronDialog.h"
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
mqIcosahedronDialog::mqIcosahedronDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqIcosahedronDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqIcosahedronDialog");	
	this->Ui->radius->setMinimum(0);
	this->Ui->radius->setMaximum(DBL_MAX);
	
	

	double mRadius = 10; 
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		mRadius = 1*mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxLength() / 20;
				

	}

	this->Ui->radius->setValue(mRadius);
	
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditIcosahedron()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 
	 
}



//-----------------------------------------------------------------------------
mqIcosahedronDialog::~mqIcosahedronDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqIcosahedronDialog::editIcosahedron()
{
	cout << "Create Icosahedron" << endl;
	this->Ui->cancel->setDisabled(true);
	
		
		
		mqMorphoDigCore::instance()->Icosahedron(this->Ui->iscosahedronNumber->value(), this->Ui->radius->value(), this->Ui->subdivisions->value());
		
	
}



void mqIcosahedronDialog::slotClose()
{
	
	this->close();
}



void mqIcosahedronDialog::sloteditIcosahedron()
{
	//cout << "Set visible true!!!" << endl;

	this->editIcosahedron();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

