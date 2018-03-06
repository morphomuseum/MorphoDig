/*=========================================================================

   Program: MorphoDig
   Module:    mqSavePLYDialogReaction.h


========================================================================*/
#ifndef mqSavePLYDialogReaction_h
#define mqSavePLYDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSavePLYDialogReaction used to show the standard PLY Save dialog for the
* application.
*/
class  mqSavePLYDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSavePLYDialogReaction(QAction* parent);

  /**
  * Shows the SavePLY dialog for the application.
  */
  static void showSavePLYDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSavePLYDialogReaction)
};

#endif
