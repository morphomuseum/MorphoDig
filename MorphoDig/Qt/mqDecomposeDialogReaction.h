/*=========================================================================

   Program: MorphoDig
   Module:    mqDecomposeDialogReaction.h


========================================================================*/
#ifndef mqDecomposeDialogReaction_h
#define mqDecomposeDialogReaction_h

#include "mqReaction.h"
#include "mqDecomposeDialog.h"
#include "mqMorphoDigCore.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqDecomposeDialogReaction used to Decompose selected surfaces
*/
class  mqDecomposeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqDecomposeDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showDecomposeDialog(mqDecomposeDialog *Decompose_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() {
	  vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	  if (num_selected_meshes == 0) {
		  QMessageBox msgBox;
		  msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		  msgBox.exec();
		  return;
	  }
	  mqDecomposeDialogReaction::showDecomposeDialog(this->Decompose_dialog); }

private:
	Q_DISABLE_COPY(mqDecomposeDialogReaction)
		mqDecomposeDialog *Decompose_dialog;
};

#endif
