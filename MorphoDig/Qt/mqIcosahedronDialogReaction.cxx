/*=========================================================================

   Program: MorphoDig
   Module:    mqIcosahedronDialogReaction.cxx


========================================================================*/
#include "mqIcosahedronDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqIcosahedronDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqIcosahedronDialogReaction::mqIcosahedronDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Icosahedron_dialog = new mqIcosahedronDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqIcosahedronDialogReaction::showIcosahedronDialog(mqIcosahedronDialog *Icosahedron_dialog)
void mqIcosahedronDialogReaction::showIcosahedronDialog()
{
	
		mqIcosahedronDialog mqIcosahedron(mqCoreUtilities::mainWidget());
		mqIcosahedron.exec();
	
	
  //Icosahedron_dialog->show();
}
