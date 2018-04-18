/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsCurvatureDialogReaction.cxx


========================================================================*/
#include "mqScalarsCurvatureDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScalarsCurvatureDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsCurvatureDialogReaction::mqScalarsCurvatureDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Curvature_dialog = new mqScalarsCurvatureDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsCurvatureDialogReaction::showScalarsCurvatureDialog(mqScalarsCurvatureDialog *Curvature_dialog)
void mqScalarsCurvatureDialogReaction::showScalarsCurvatureDialog()
{
	mqScalarsCurvatureDialog mqCurvature(mqCoreUtilities::mainWidget());
	mqCurvature.exec();
  
  //Curvature_dialog->show();
}
