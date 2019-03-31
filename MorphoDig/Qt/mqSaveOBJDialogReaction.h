/*=========================================================================

   Program: MorphoDig
   Module:    mqSavePLYDialogReaction.h


========================================================================*/
#ifndef mqSaveOBJDialogReaction_h
#define mqSaveOBJDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveOBJDialogReaction used to show the standard OBJ Save dialog for the
* application.
*/
class  mqSaveOBJDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveOBJDialogReaction(QAction* parent);

  /**
  * Shows the SaveOBJ dialog for the application.
  */
  static void showSaveOBJDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSaveOBJDialogReaction)
};

#endif
