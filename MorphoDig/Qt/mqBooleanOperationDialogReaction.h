/*=========================================================================

   Program: MorphoDig
   Module:    mqBooleanOperationDialogReaction.h


========================================================================*/
#ifndef mqBooleanOperationDialogReaction_h
#define mqBooleanOperationDialogReaction_h

#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include "mqBooleanOperationDialog.h"
#include <QMessageBox>
/**
* @ingroup Reactions
* mqBooleanOperationDialogReaction used to show the "BooleanOperation" dialog for the
* application.
*/
class  mqBooleanOperationDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqBooleanOperationDialogReaction(QAction* parent);

  /**
  * Shows the BooleanOperation dialog for the application.
  */
  //static void showBooleanOperationDialog(mqBooleanOperationDialog *BooleanOperation_dialog);
  static void showBooleanOperationDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() {
		vtkIdType num_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
		if (num_meshes < 2) {
			QMessageBox msgBox;
			msgBox.setText("At least two surface objects must be opened to use this function.");
			msgBox.exec();
			return;
		}
		
		mqBooleanOperationDialogReaction::showBooleanOperationDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqBooleanOperationDialogReaction::showBooleanOperationDialog(this->BooleanOperation_dialog); }

private:
	Q_DISABLE_COPY(mqBooleanOperationDialogReaction)
		//mqBooleanOperationDialog *BooleanOperation_dialog;
};

#endif
