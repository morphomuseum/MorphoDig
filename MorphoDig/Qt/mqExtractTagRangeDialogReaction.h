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
	  ActiveArray *myActiveArray = mqMorphoDigCore::instance()->Getmui_ActiveArray();
	  int ok = 0;
	  if ((myActiveArray->DataType == VTK_INT || myActiveArray->DataType == VTK_UNSIGNED_INT) && myActiveArray->NumComp == 1)
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
	  vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	  if (num_selected_meshes == 0) {
		  QMessageBox msgBox;
		  msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		  msgBox.exec();
		  return;
	  }

	  mqExtractTagRangeDialogReaction::showExtractTagRangeDialog(); }

private:
	Q_DISABLE_COPY(mqExtractTagRangeDialogReaction)
		//mqExtractTagRangeDialog *etr_dialog;
};

#endif
