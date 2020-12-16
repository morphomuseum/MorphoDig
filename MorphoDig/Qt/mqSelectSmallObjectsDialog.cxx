/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSelectSmallObjectsDialog.h"
#include "ui_mqSelectSmallObjectsDialog.h"
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
mqSelectSmallObjectsDialog::mqSelectSmallObjectsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSelectSmallObjectsDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqSelectSmallObjectsDialog");	
	double meshcolor[4];
	
	
	this->Ui->triangles->setMinimum(2);
	this->Ui->triangles->setMaximum(20000000);
	this->Ui->triangles->setSingleStep(1000);
	this->Ui->triangles->setValue(500);
	

	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSelectSmallObjects()));

}




//-----------------------------------------------------------------------------
mqSelectSmallObjectsDialog::~mqSelectSmallObjectsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSelectSmallObjectsDialog::SelectSmallObjects()
{
	cout << "Edit alpha" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems() > 0)
	{
		std::string action = "Select small surfaces";
		
		mqMorphoDigCore::instance()->SelectSmallObjects(this->Ui->triangles->value());// to update body size!
		mqMorphoDigCore::instance()->Render();
	}
}







void mqSelectSmallObjectsDialog::slotSelectSmallObjects()
{
	this->SelectSmallObjects();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

