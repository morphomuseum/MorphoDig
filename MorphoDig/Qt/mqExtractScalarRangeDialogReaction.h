/*=========================================================================

   Program: MorphoDig
   Module:    mqExtractScalarRangeDialogReaction.h


========================================================================*/
#ifndef mqExtractScalarRangeDialogReaction_h
#define mqExtractScalarRangeDialogReaction_h

#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include "mqExtractScalarRangeDialog.h"
#include <QMessageBox>
/**
* @ingroup Reactions
* mqExtractScalarRangeDialogReaction used to show the standard Color dialog for the
* application.
*/
class  mqExtractScalarRangeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqExtractScalarRangeDialogReaction(QAction* parent);

  /**
  * Shows the Color dialog for the application.
  */
  static void showExtractScalarRangeDialog();

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { 
	 
	  // checks wether active array is scalar array... 	
	  ActiveScalars *myActiveScalars = mqMorphoDigCore::instance()->Getmui_ActiveScalars();
	  int ok = 0;
	  if ((myActiveScalars->DataType == VTK_FLOAT || myActiveScalars->DataType == VTK_DOUBLE) && myActiveScalars->NumComp == 1)
	  {
		  ok = 1;
	  }
	  if (ok == 0)
	  {
		  QMessageBox msgBox;
		  msgBox.setText("A Scalar array must be active to use this option.");
		  msgBox.exec();
		  return;
	  }
	  mqExtractScalarRangeDialogReaction::showExtractScalarRangeDialog();
	 // mqExtractScalarRangeDialogReaction::showExtractScalarRangeDialog(this->esr_dialog); 
  }

private:
	Q_DISABLE_COPY(mqExtractScalarRangeDialogReaction)
	//	mqExtractScalarRangeDialog *esr_dialog;
};

#endif
