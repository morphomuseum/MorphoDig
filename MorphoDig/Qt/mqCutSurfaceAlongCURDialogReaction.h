/*=========================================================================

   Program: MorphoDig
   Module:    mqCutSurfaceAlongCURDialogReaction.h


========================================================================*/
#ifndef mqCutSurfaceAlongCURDialogReaction_h
#define mqCutSurfaceAlongCURDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqCutSurfaceAlongCURDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqCutSurfaceAlongCURDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveCURasVERDialog();

	

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqCutSurfaceAlongCURDialogReaction)

};

#endif
