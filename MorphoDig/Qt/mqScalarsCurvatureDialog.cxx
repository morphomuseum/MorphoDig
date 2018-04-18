/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsCurvatureDialog.h"
#include "ui_mqScalarsCurvatureDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


// we actually do not need glew...
//#include <GL/glew.h>
#include <vtkCurvatures.h>
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
mqScalarsCurvatureDialog::mqScalarsCurvatureDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsCurvatureDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsCurvatureDialog");	
	
	
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditCurvature()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditCurvature()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	
}




//-----------------------------------------------------------------------------
mqScalarsCurvatureDialog::~mqScalarsCurvatureDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsCurvatureDialog::editCurvature()
{
	cout << "Edit Curvature" << endl;
	this->Ui->cancel->setDisabled(true);
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update Curvature";
		int curvType = 0;
		if (this->Ui->meanCurvature->isChecked())
		{
			curvType = VTK_CURVATURE_MEAN;
		}
		else if (this->Ui->gaussianCurvature->isChecked())
		{
			curvType = VTK_CURVATURE_GAUSS;
		}
		else if (this->Ui->maximalCurvature->isChecked())
		{
			curvType = VTK_CURVATURE_MAXIMUM;
		}
		if (this->Ui->minimalCurvature->isChecked())
		{
			curvType = VTK_CURVATURE_MINIMUM;
		}
		mqMorphoDigCore::instance()->scalarsCurvature(curvType, this->Ui->scalarName->text());// to compute curvatures
		
	}

}


void mqScalarsCurvatureDialog::slotClose()
{
	
	this->close();
}


void mqScalarsCurvatureDialog::sloteditCurvature()
{
	//cout << "Set visible true!!!" << endl;
	this->editCurvature();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

