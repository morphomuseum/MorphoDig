/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditTagsDialog_h
#define _mqEditTagsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>
#include "mqColorOpacityEditorWidget.h"


namespace Ui
{
class mqEditTagsDialog;
}
class SignalBlocker {
public:
	SignalBlocker(QObject *o) : object(o), alreadyBlocked(object->signalsBlocked()) {
		if (!alreadyBlocked) {
			object->blockSignals(true);
		}
	}
	~SignalBlocker() {
		if (!alreadyBlocked) {
			object->blockSignals(false);
		}
	}

private:
	QObject *object;
	bool alreadyBlocked;
};



/**
* mqEditTagsDialog is the dialog used by MorphoDig to edit Tags.
*/
class  mqEditTagsDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditTagsDialog(QWidget* Parent);
  
  ~mqEditTagsDialog();
 
  void UpdateUI();
  
 void RefreshDialog();
 void RefreshComboActiveTags();
 void RefreshComboTagMaps();
 void RefreshTagMapTable();
 
 

  public slots:
  virtual void slotEditTagMapName();
  virtual void  slotReinitializeTagMap();
  virtual void slotDeleteTagMap();
  virtual void slotActiveTagsChanged(int idx);
  virtual void slotActiveTagMapChanged(int idx);
  virtual void slotRefreshDialog();
  virtual void slotRefreshComboTags();
  virtual void slotRemoveTags();
  virtual void slotRefreshTagMaps();
  virtual void slotCellChanged(int row, int column);
/*  virtual void slotCellActivated(int row, int column);  
  virtual void slotCellClicked(int row, int column);
  virtual void slotCellEntered(int row, int column);
  virtual void slotCellPressed(int row, int column);*/
  virtual void slotColorChanged();
  virtual void slotActiveTagChanged();
  virtual void slotAlphaChanged(int newalpha);
  virtual void slotSaveAsCustom();

protected:
	
private:
	void updateLabel(int row, QString newLabel);	
	void updateColor(int row, double r, double g, double b);
	void updateAlpha(int row, int newalpha);


  Q_DISABLE_COPY(mqEditTagsDialog)
  Ui::mqEditTagsDialog* const Ui;
  double activeTag;
 

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
