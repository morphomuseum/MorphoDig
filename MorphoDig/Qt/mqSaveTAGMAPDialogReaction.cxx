/*=========================================================================

   Program: MorphoDig
   Module:    mqTAGMAPDialogReaction.cxx


========================================================================*/
#include "mqSaveTAGMAPDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveTAGMAPDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveTAGMAPDialogReaction::mqSaveTAGMAPDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveTAGMAPDialogReaction::onTriggered()
{
	
		
		cout << "Save TAGMAP Dialog Triggered!" << endl;


		


		mqSaveTAGMAPDialogReaction::showSaveTAGMAPDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveTAGMAPDialogReaction::showSaveTAGMAPDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE TAGMAP, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveTAGMAPDialog SaveTAGMAP_dialog(mqCoreUtilities::mainWidget());
  SaveTAGMAP_dialog.exec();
}
