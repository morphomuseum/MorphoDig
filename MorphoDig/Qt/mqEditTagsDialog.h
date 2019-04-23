/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditTagsDialog_h
#define _mqEditTagsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>



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
  //void hide();
 void RefreshDialog();
 void RefreshComboActiveTags();
 void RefreshComboTagMaps();
 void RefreshTagMapTable();
 void closeEvent(QCloseEvent *event);

 

  public slots:
  virtual void slotEditTagMapName();
  virtual void slotAddTag();
  virtual void slotRemoveTag();
  virtual void  slotReinitializeTagMap();
  virtual void slotDeleteTagMap();
  virtual void slotRefreshPencilSearchSize(int newsize);
  virtual void slotActiveTagsChanged(int idx);
  virtual void slotActiveTagMapChanged(int idx);
  virtual void slotRefreshDialog();
  virtual void slotRefreshComboTags();
  virtual void slotRemoveTags();
  virtual void slotRefreshTagMaps();
  virtual void slotActivateTagMode();
  virtual void slotPencilAll();
  virtual void slotPencilContiguous();
  virtual void slotPencilSearchSizeChanged(int newSize);
  virtual void slotPencilLimitAngleChanged(int newLimitAngle);
  virtual void slotBucketOn();
  virtual void slotPencilOn();
  virtual void slotEraseActiveTag();
  //virtual void slotDeactivateTagMode();
  virtual void slotCellChanged(int row, int column);
/*  virtual void slotCellActivated(int row, int column);  
  virtual void slotCellClicked(int row, int column);
  virtual void slotCellEntered(int row, int column);
  virtual void slotCellPressed(int row, int column);*/
  virtual void slotColorChanged();
  virtual void slotActiveTagChanged();
  virtual void slotOpacityChanged(int newopacity);
  virtual void slotSaveAsCustom();

protected:
	
private:
	void updateLabel(int row, QString newLabel);	
	void updateColor(int row, double r, double g, double b);
	void updateOpacity(int row, int newopacity);


  Q_DISABLE_COPY(mqEditTagsDialog)
  Ui::mqEditTagsDialog* const Ui;
  double activeTag;
 

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
