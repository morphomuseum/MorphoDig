/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllSurfacesColorDialogReaction.h


========================================================================*/
#ifndef mqEditAllSurfacesColorDialogReaction_h
#define mqEditAllSurfacesColorDialogReaction_h

#include "mqReaction.h"
#include "mqEditAllSurfacesColorDialog.h"
/**
* @ingroup Reactions
* mqEditAllSurfacesColorDialogReaction used to edit the color of all currently opened surfaces
* application.
*/
class  mqEditAllSurfacesColorDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditAllSurfacesColorDialogReaction(QAction* parent);

  /**
  * Shows the Surfaces dialog for the application.
  */
  static void showEditAllSurfacesDialog(mqEditAllSurfacesColorDialog *Surfaces_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditAllSurfacesColorDialogReaction::showEditAllSurfacesDialog(this->Surfaces_dialog); }

private:
	Q_DISABLE_COPY(mqEditAllSurfacesColorDialogReaction)
		mqEditAllSurfacesColorDialog *Surfaces_dialog;
};

#endif
