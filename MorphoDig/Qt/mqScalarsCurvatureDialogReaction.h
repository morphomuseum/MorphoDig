/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsCurvatureDialogReaction.h


========================================================================*/
#ifndef mqScalarsCurvatureDialogReaction_h
#define mqScalarsCurvatureDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsCurvatureDialog.h"
/**
* @ingroup Reactions
* mqScalarsCurvatureDialogReaction used to compute curvature scalars
* application.
*/
class  mqScalarsCurvatureDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsCurvatureDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsCurvatureDialog(mqScalarsCurvatureDialog *FLG_dialog);
  static void showScalarsCurvatureDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsCurvatureDialogReaction::showScalarsCurvatureDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsCurvatureDialogReaction::showScalarsCurvatureDialog(this->Curvature_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsCurvatureDialogReaction)
		//mqScalarsCurvatureDialog *Curvature_dialog;
};

#endif
