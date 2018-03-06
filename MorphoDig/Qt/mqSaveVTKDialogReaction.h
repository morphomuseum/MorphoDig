/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveVTKDialogReaction.h


========================================================================*/
#ifndef mqSaveVTKDialogReaction_h
#define mqSaveVTKDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveVTKDialogReaction used to show the standard VTK Save dialog for the
* application.
*/
class  mqSaveVTKDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveVTKDialogReaction(QAction* parent);

  /**
  * Shows the SaveVTK dialog for the application.
  */
  static void showSaveVTKDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSaveVTKDialogReaction)
};

#endif
