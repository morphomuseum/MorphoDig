/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveVTIDialogReaction.h


========================================================================*/
#ifndef mqSaveVTIDialogReaction_h
#define mqSaveVTIDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveVTIDialogReaction used to show the standard VTI Save dialog for the
* application.
*/
class  mqSaveVTIDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveVTIDialogReaction(QAction* parent);

  /**
  * Shows the SaveVTI dialog for the application.
  */
  static void showSaveVTIDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSaveVTIDialogReaction)
};

#endif
