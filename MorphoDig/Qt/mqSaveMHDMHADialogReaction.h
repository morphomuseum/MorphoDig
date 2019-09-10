/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveMHDMHADialogReaction.h


========================================================================*/
#ifndef mqSaveMHDMHADialogReaction_h
#define mqSaveMHDMHADialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveMHDMHADialogReaction used to show the standard MHDMHA Save dialog for the
* application.
*/
class  mqSaveMHDMHADialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveMHDMHADialogReaction(QAction* parent, int _mode =0);

  /**
  * Shows the SaveMHDMHA dialog for the application.
  */
  static void showSaveMHDMHADialog(QString fileName, int mode);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSaveMHDMHADialogReaction)
	  int Mode;
};

#endif
