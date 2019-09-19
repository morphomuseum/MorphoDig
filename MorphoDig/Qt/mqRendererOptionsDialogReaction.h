/*=========================================================================

   Program: MorphoDig
   Module:    mqRendererOptionsDialogReaction.h


========================================================================*/
#ifndef mqRendererOptionsDialogReaction_h
#define mqRendererOptionsDialogReaction_h

#include "mqReaction.h"
#include "mqRendererOptionsDialog.h"
/**
* @ingroup Reactions
* mqRendererOptionsDialogReaction used to show the standard Renderer options
*/
class  mqRendererOptionsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqRendererOptionsDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showRendererOptionsDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();

private:
	Q_DISABLE_COPY(mqRendererOptionsDialogReaction)
	
};

#endif
