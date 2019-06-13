/*=========================================================================

   Program: MorphoDig
   Module:    mqOpenRawDialogReaction.h


========================================================================*/
#ifndef mqOpenRawDialogReaction_h
#define mqOpenRawDialogReaction_h

#include "mqReaction.h"
#include "mqOpenRawDialog.h"
#include "mqMorphoDigCore.h"

#include "vtkMDVolume.h"
#include <QMessageBox>
#include <QFileDialog>
/**
* @ingroup Reactions
* mqOpenRawDialogReaction used ton open a raw data volume 
*/
class  mqOpenRawDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqOpenRawDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showOpenRawDialog(QString  fileName);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() {
	  QString fileName = QFileDialog::getOpenFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		  tr("Open raw data"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		  tr("raw data (*.raw);; other (*.*)"));

	  cout << fileName.toStdString();
	  if (fileName.isEmpty()) return;
	 
	  mqOpenRawDialogReaction::showOpenRawDialog(fileName); 
  
  }

private:
	Q_DISABLE_COPY(mqOpenRawDialogReaction)
		//mqOpenRawDialog *OpenRaw_dialog;
	
};

#endif
