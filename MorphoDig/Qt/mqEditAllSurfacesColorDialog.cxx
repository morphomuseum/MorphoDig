/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditAllSurfacesColorDialog.h"
#include "ui_mqEditAllSurfacesColorDialog.h"
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
mqEditAllSurfacesColorDialog::mqEditAllSurfacesColorDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditAllSurfacesColorDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqEditAllSurfacesColorDialog");			
	

	
	QColor myColor;

	double meshcolor[4];
	this->Ui->ActorColorButton->setShowAlphaChannel(false);
	mqMorphoDigCore::instance()->Getmui_MeshColor(meshcolor);
	myColor.setRedF(meshcolor[0]);
	cout << "meshcolor[0]" << meshcolor[0]<<endl;
	myColor.setGreenF(meshcolor[1]);
	cout << "meshcolor[1]" << meshcolor[1] << endl;
	myColor.setBlueF(meshcolor[2]);
	cout << "meshcolor[2]" << meshcolor[2] << endl;
	myColor.setAlphaF(meshcolor[3]);
	cout << "meshcolor[3]" << meshcolor[3] << endl;


	//this->Ui->ActorColorButton->setShowAlphaChannel(false);
	this->Ui->ActorColorButton->setChosenColor(myColor);

  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveAllSurfaces()));

}




//-----------------------------------------------------------------------------
mqEditAllSurfacesColorDialog::~mqEditAllSurfacesColorDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditAllSurfacesColorDialog::saveAllSurfaces()
{
	cout << "Save All Surfaces COLOR!" << endl;
	
	if (mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update all selected surfaces";
		int mCount = BEGIN_UNDO_SET(action);

		QColor myColor = this->Ui->ActorColorButton->chosenColor();
		double meshcolor[4];
		myColor.getRgbF(&meshcolor[0], &meshcolor[1], &meshcolor[2], &meshcolor[3]);
		double meshcolor2[3];
		meshcolor2[0] = meshcolor[0];
		cout << "meshcolor2[0]" << meshcolor2[0] << endl;
		meshcolor2[1] = meshcolor[1];
		cout << "meshcolor2[1]" << meshcolor2[1] << endl;

		meshcolor2[2] = meshcolor[2];
		cout << "meshcolor2[2]" << meshcolor2[2] << endl;
		mqMorphoDigCore::instance()->Setmui_MeshColorNoAlpha(meshcolor2); //do not change alpha
		
		
		END_UNDO_SET();
		mqMorphoDigCore::instance()->Render();
		
	}
}







void mqEditAllSurfacesColorDialog::slotsaveAllSurfaces()
{
	this->saveAllSurfaces();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

