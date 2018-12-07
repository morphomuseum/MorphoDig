/*=========================================================================

   Program: MorphoDig
   Module:    mqShrinkWrapDialogReaction.h


========================================================================*/
#ifndef mqShrinkWrapDialogReaction_h
#define mqShrinkWrapDialogReaction_h

#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include "mqShrinkWrapDialog.h"
#include <QMessageBox>
/**
* @ingroup Reactions
* mqShrinkWrapDialogReaction used to show the "ShrinkWrap" dialog for the
* application.
*/
class  mqShrinkWrapDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqShrinkWrapDialogReaction(QAction* parent);

  /**
  * Shows the ShrinkWrap dialog for the application.
  */
  //static void showShrinkWrapDialog(mqShrinkWrapDialog *ShrinkWrap_dialog);
  static void showShrinkWrapDialog();

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
		
		mqShrinkWrapDialogReaction::showShrinkWrapDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqShrinkWrapDialogReaction::showShrinkWrapDialog(this->ShrinkWrap_dialog); }

private:
	Q_DISABLE_COPY(mqShrinkWrapDialogReaction)
		//mqShrinkWrapDialog *ShrinkWrap_dialog;
};

#endif
