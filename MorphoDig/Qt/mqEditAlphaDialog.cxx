/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditAlphaDialog.h"
#include "ui_mqEditAlphaDialog.h"
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
mqEditAlphaDialog::mqEditAlphaDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditAlphaDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqEditAlphaDialog");	
	double meshcolor[4];
	mqMorphoDigCore::instance()->Getmui_MeshColor(meshcolor);
	int  alpha= (int)(meshcolor[3]*255);
	this->Ui->alpha->setMinimum(0);
	this->Ui->alpha->setMaximum(100);
	this->Ui->alpha->setSingleStep(1);
	this->Ui->alpha->setValue(alpha);

	QColor myFlagColor;
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditAlpha()));

}




//-----------------------------------------------------------------------------
mqEditAlphaDialog::~mqEditAlphaDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditAlphaDialog::editAlpha()
{
	cout << "Edit alpha" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update alpha";
		
		mqMorphoDigCore::instance()->SetSelectedActorsTransparency(this->Ui->alpha->value());// to update body size!
		
	}
}







void mqEditAlphaDialog::sloteditAlpha()
{
	this->editAlpha();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

