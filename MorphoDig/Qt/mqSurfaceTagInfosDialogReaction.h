/*=========================================================================

   Program: MorphoDig
   Module:    mqSurfaceTagInfosDialogReaction.h


========================================================================*/
#ifndef mqSurfaceTagInfosDialogReaction_h
#define mqSurfaceTagInfosDialogReaction_h

#include "mqReaction.h"
#include "mqSurfaceTagInfosDialog.h"
/**
* @ingroup Reactions
* mqSurfaceTagInfosDialogReaction used to compute Infos (area, volume etc.) of surfaces, and possibly for each tagged region
* application.
*/
class  mqSurfaceTagInfosDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSurfaceTagInfosDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showSurfaceTagInfosDialog(mqSurfaceTagInfosDialog *FLG_dialog);
  static void showSurfaceTagInfosDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqSurfaceTagInfosDialogReaction::showSurfaceTagInfosDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqSurfaceTagInfosDialogReaction::showSurfaceTagInfosDialog(this->Infos_dialog); }

private:
	Q_DISABLE_COPY(mqSurfaceTagInfosDialogReaction)
		//mqSurfaceTagInfosDialog *Infos_dialog;
};

#endif
