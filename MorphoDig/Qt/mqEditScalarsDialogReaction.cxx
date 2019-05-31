/*=========================================================================

   Program: MorphoDig
   Module:    mqEditScalarsDialogReaction.cxx


========================================================================*/
#include "mqEditScalarsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditScalarsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditScalarsDialogReaction::mqEditScalarsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Scalars_dialog = new mqEditScalarsDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditScalarsDialogReaction::showEditScalarsDialog(mqEditScalarsDialog *Scalars_dialog)
{
	cout << "Scalar Dialog: Try update UI" << endl;
  Scalars_dialog->UpdateUI();
  cout << "Scalar Dialog: Try show" << endl;
  Scalars_dialog->show();
  cout << "Scalar Dialog: shown" << endl;
}
