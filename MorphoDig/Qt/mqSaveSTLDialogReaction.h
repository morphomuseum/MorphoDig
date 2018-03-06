/*=========================================================================

   Program: MorphoDig
   Module:    mqSavePLYDialogReaction.h


========================================================================*/
#ifndef mqSaveSTLDialogReaction_h
#define mqSaveSTLDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveSTLDialogReaction used to show the standard STL Save dialog for the
* application.
*/
class  mqSaveSTLDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveSTLDialogReaction(QAction* parent);

  /**
  * Shows the SaveSTL dialog for the application.
  */
  static void showSaveSTLDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSaveSTLDialogReaction)
};

#endif
