/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditVolumeDialog_h
#define _mqEditVolumeDialog_h

#include "vtkMDVolume.h"
#include "vtkMDVolumeCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>
#include "mqColorOpacityEditorWidget.h"
namespace Ui
{
class mqEditVolumeDialog;
}




/**
* mqEditVolumeDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditVolumeDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditVolumeDialog(QWidget* Parent);
  
  ~mqEditVolumeDialog();
  void saveVolume();
  double GetCTFMin();
  double GetCTFMax();
  void RefreshSliders();
  void GetFirstSelectedVolume();
  void GetFirstVolume();
  void GetNextVolume();
  void GetPrecedingVolume();
 void UpdateUI();
  int SomeThingHasChanged();
  int CurrentVolumeInCollection();
  void RefreshDialog();
  void MoveSliders();
  void UpdateLookupTableRange(double min, double max);
  public slots:
 virtual void slotRefreshSliders();
 virtual void slotMoveSliders();
 virtual void slotCurrentMinMaxEdited();
  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingVolume();
  virtual void slotGetNextVolume();
  virtual void slotsaveVolume();
  virtual void slotReinitMatrix();
  virtual void slotapplyMatrixToAllSelectedVolumes();
 

protected:
	
private:
	mqColorOpacityEditorWidget *mColorMap;

  Q_DISABLE_COPY(mqEditVolumeDialog)
  Ui::mqEditVolumeDialog* const Ui;
  vtkSmartPointer<vtkMDVolume> Volume;
  //vtkMDVolume *Volume;
  vtkMDVolumeCollection *Volume_Coll;
  int current_coll;

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
