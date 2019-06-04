/*=========================================================================

   Program: MorphoDig
   Module:    mqMinimalDialogReaction.h


========================================================================*/
#ifndef mqMinimalDialogReaction_h
#define mqMinimalDialogReaction_h

#include "mqReaction.h"
#include "mqMinimalDialog.h"
#include "mqMorphoDigCore.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqMinimalDialogReaction used to open the MinimalDialog window: used to test troubleshootings with vtkQtOpenGL widgets... (used in edit scalar windows...) selected surfaces
*/
class  mqMinimalDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqMinimalDialogReaction(QAction* parent);

  /**
  * Shows the Miniam dialog for the application.
  */
  static void showMinimalDialog(mqMinimalDialog *Minimal_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { 
	 
	
	  mqMinimalDialogReaction::showMinimalDialog(this->Minimal_dialog); }

private:
	Q_DISABLE_COPY(mqMinimalDialogReaction)
		mqMinimalDialog *Minimal_dialog;
};

#endif
