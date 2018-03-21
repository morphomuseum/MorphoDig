/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsThicknessDialog.h"
#include "ui_mqScalarsThicknessDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


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
mqScalarsThicknessDialog::mqScalarsThicknessDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsThicknessDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqScalarsThicknessDialog");	
	this->Ui->thickness->setMinimum(0);
	this->Ui->thickness->setMaximum(DBL_MAX);
	this->Ui->thickness->setSingleStep(1);
	this->Ui->thickness->setValue(2);


  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditThickness()));

}




//-----------------------------------------------------------------------------
mqScalarsThicknessDialog::~mqScalarsThicknessDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsThicknessDialog::editThickness()
{
	cout << "Edit alpha" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update thickness";
		
		mqMorphoDigCore::instance()->scalarsThickness(this->Ui->thickness->value());// to update thickness
		
	}
}







void mqScalarsThicknessDialog::sloteditThickness()
{
	this->editThickness();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

