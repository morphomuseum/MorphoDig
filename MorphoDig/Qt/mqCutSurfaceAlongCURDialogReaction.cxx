/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqCutSurfaceAlongCURDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqCutSurfaceAlongCURDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqCutSurfaceAlongCURDialogReaction::mqCutSurfaceAlongCURDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqCutSurfaceAlongCURDialogReaction::onTriggered()
{
	
	vtkIdType num_CURN = 0;
	vtkIdType num_CURH = 0;
	
		num_CURN = mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
		num_CURH = mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	
	
		/*if (num_CURN == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Curve Node exist...");
			msgBox.exec();
			return;
		}
		if (num_CURH == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Curve Handle exist...");
			msgBox.exec();
			return;
		}*/
		if (num_CURH != num_CURN) {
			QMessageBox msgBox;
			msgBox.setText("Number of curve nodes differs from number of curve handles... impossible to export curves as landmark file!");
			msgBox.exec();
			return;
		}
		cout << "Export CUR as VER Dialog Triggered!" << endl;


		
		mqCutSurfaceAlongCURDialogReaction::showSaveCURasVERDialog();
		
	
		
}

//-----------------------------------------------------------------------------
void mqCutSurfaceAlongCURDialogReaction::showSaveCURasVERDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE CUR, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqCutSurfaceAlongCURDialog CurSurfaceAlongCUR_dialog(mqCoreUtilities::mainWidget());
  CurSurfaceAlongCUR_dialog.exec();
}
