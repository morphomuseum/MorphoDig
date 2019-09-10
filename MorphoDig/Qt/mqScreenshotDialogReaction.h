/*=========================================================================

   Program: MorphoDig
   Module:    mqScreenshotDialogReaction.h


========================================================================*/
#ifndef mqScreenshotDialogReaction_h
#define mqScreenshotDialogReaction_h

#include "mqReaction.h"
#include "mqScreenshotDialog.h"
#include "mqMorphoDigCore.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqScreenshotDialogReaction used to take screenshots
*/
class  mqScreenshotDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScreenshotDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showScreenshotDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();

private:
	Q_DISABLE_COPY(mqScreenshotDialogReaction)
		
};

#endif
