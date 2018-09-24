/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsNormalizationDialogReaction.h


========================================================================*/
#ifndef mqScalarsNormalizationDialogReaction_h
#define mqScalarsNormalizationDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsNormalizationDialog.h"
/**
* @ingroup Reactions
* mqScalarsNormalizationDialogReaction used to compute Normalization scalars
* application.
*/
class  mqScalarsNormalizationDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsNormalizationDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  
  static void showScalarsNormalizationDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsNormalizationDialogReaction::showScalarsNormalizationDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsNormalizationDialogReaction::showScalarsNormalizationDialog(this->Normalization_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsNormalizationDialogReaction)
		//mqScalarsNormalizationDialog *Normalization_dialog;
};

#endif
