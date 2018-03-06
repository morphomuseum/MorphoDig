/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditAllFLGLengthDialog.h"
#include "ui_mqEditAllFLGLengthDialog.h"
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
mqEditAllFLGLengthDialog::mqEditAllFLGLengthDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditAllFLGLengthDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqEditAllFLGLengthDialog");			
	double flag_rendering_size = mqMorphoDigCore::instance()->Getmui_FlagRenderingSize();
	this->Ui->FlagRenderingSizeValue->setMinimum(0);
	this->Ui->FlagRenderingSizeValue->setSingleStep(1);
	this->Ui->FlagRenderingSizeValue->setValue(flag_rendering_size);



	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveAllFLG()));

}




//-----------------------------------------------------------------------------
mqEditAllFLGLengthDialog::~mqEditAllFLGLengthDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditAllFLGLengthDialog::saveAllFLG()
{
	cout << "Save All FLG!" << endl;
	
	if (mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update all selected flags";
		int mCount = BEGIN_UNDO_SET(action);
		
		double flg_rendering_size = this->Ui->FlagRenderingSizeValue->value();

		mqMorphoDigCore::instance()->UpdateAllSelectedFlagsLengths(flg_rendering_size);// to update body size!
		END_UNDO_SET();
	}
}







void mqEditAllFLGLengthDialog::slotsaveAllFLG()
{
	this->saveAllFLG();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

