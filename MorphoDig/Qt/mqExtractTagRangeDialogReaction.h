/*=========================================================================

   Program: MorphoDig
   Module:    mqExtractTagRangeDialogReaction.h


========================================================================*/
#ifndef mqExtractTagRangeDialogReaction_h
#define mqExtractTagRangeDialogReaction_h

#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include "mqExtractTagRangeDialog.h"
#include <QMessageBox>
/**
* @ingroup Reactions
* mqExtractTagRangeDialogReaction used to show the standard Color dialog for the
* application.
*/
class  mqExtractTagRangeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqExtractTagRangeDialogReaction(QAction* parent);

  /**
  * Shows the Color dialog for the application.
  */
  static void showExtractTagRangeDialog();

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() {
	  ActiveScalars *myActiveScalars = mqMorphoDigCore::instance()->Getmui_ActiveScalars();
	  int ok = 0;
	  if ((myActiveScalars->DataType == VTK_INT || myActiveScalars->DataType == VTK_UNSIGNED_INT) && myActiveScalars->NumComp == 1)
	  {
		  ok = 1;
	  }
	  if (ok == 0)
	  {
		  QMessageBox msgBox;
		  msgBox.setText("A Tag array must be active to use this option.");
		  msgBox.exec();
		  return;
	  }

	  mqExtractTagRangeDialogReaction::showExtractTagRangeDialog(); }

private:
	Q_DISABLE_COPY(mqExtractTagRangeDialogReaction)
		//mqExtractTagRangeDialog *etr_dialog;
};

#endif
