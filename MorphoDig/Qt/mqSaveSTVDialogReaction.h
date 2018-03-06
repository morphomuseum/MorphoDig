/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveSTVDialogReaction.h


========================================================================*/
#ifndef mqSaveSTVDialogReaction_h
#define mqSaveSTVDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveSTVDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveSTVDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveSTVDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveSTVDialogReaction)

};

#endif
