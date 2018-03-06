/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditAllFLGColorDialog.h"
#include "ui_mqEditAllFLGColorDialog.h"
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
mqEditAllFLGColorDialog::mqEditAllFLGColorDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditAllFLGColorDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqEditAllFLGColorDialog");			
	

	QColor myFlagColor;

	double flagcolor[4];

	this->Ui->FlagColorButton->setShowAlphaChannel(false);
	mqMorphoDigCore::instance()->Getmui_FlagColor(flagcolor);

	myFlagColor.setRedF(flagcolor[0]);
	myFlagColor.setGreenF(flagcolor[1]);
	myFlagColor.setBlueF(flagcolor[2]);


	this->Ui->FlagColorButton->setChosenColor(myFlagColor);

  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveAllFLG()));

}




//-----------------------------------------------------------------------------
mqEditAllFLGColorDialog::~mqEditAllFLGColorDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditAllFLGColorDialog::saveAllFLG()
{
	cout << "Save All FLG COLOR!" << endl;
	
	if (mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "Update all selected flags";
		int mCount = BEGIN_UNDO_SET(action);
		QColor myFlagColor = this->Ui->FlagColorButton->chosenColor();
		double flagcolor[4];
		myFlagColor.getRgbF(&flagcolor[0], &flagcolor[1], &flagcolor[2], &flagcolor[3]);

	

		mqMorphoDigCore::instance()->UpdateAllSelectedFlagsColors(flagcolor);// to update body size!
		END_UNDO_SET();
	}
}







void mqEditAllFLGColorDialog::slotsaveAllFLG()
{
	this->saveAllFLG();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

