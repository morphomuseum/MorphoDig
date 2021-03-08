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
class QReleaseSliderValue;
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
  void ShowAndRefresh();
  void reject(); // override the close window event function!
 void GetFirstSelectedVolume();
  void GetFirstVolume();
  void GetNextVolume();
  void GetPrecedingVolume();
 void UpdateUI();
  int SomeThingHasChanged();
  int CurrentVolumeInCollection();
  void RefreshDialog();
  void RefreshSuggestedRange();
  void RefreshComboColorMaps();
  void LoadPreset();
  
  void CropVolume();
  /*double GetCTFMin();
  double GetCTFMax();
  void UpdateLookupTableRange();*/
  public slots:
	  virtual void slotRefreshSliceXY();
	  virtual void slotRefreshSliceXZ();
	  virtual void slotRefreshSliceYZ();

  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingVolume();
  virtual void slotGetNextVolume();
  virtual void slotsaveVolume();
  virtual void slotReinitMatrix();
  virtual void slotRefreshMatrix();
  virtual void slotapplyMatrixToAllSelectedVolumes();
  virtual void slotInterpolationToLinear(bool isChecked);
  virtual void slotScalarOpacityUnitDistance(double SOUD);
  virtual void slotSaveActorMinMaxHaveBeenChangedInWidget();
  virtual void slotRefreshSuggestedRange();
  virtual void slotAcceptSuggestedMin();
  virtual void slotAcceptSuggestedMax();
  virtual void slotRefreshUi();
  virtual void slotLoadPreset(int idx);
  virtual void slotMapper(int idx);
  virtual void slotUpdateHistogram();
  virtual void slotRefreshColorMaps();
  virtual void slotReinitializeColorMap();
  virtual void slotEditColorMapName();
  virtual void slotDeleteColorMap();
  virtual void slotdisplayROIPressed();
  

  virtual void slotEnableROIClicked(bool isChecked);
  virtual void slotEnableMASKClicked(bool isChecked);
  virtual void slotisVisibleClicked(bool isChecked);
  virtual void slotReinitializeMask();
  virtual void slotInvertMask();
  virtual void slotHardenMask();
  virtual void slotMaskWithSurface();
  virtual void slotRefreshPencilSearchSize(int newsize);
  virtual void slotPencilSearchSizeChanged(int newSize);
  virtual void slotMaskInsideRClicked(bool isChecked);
  virtual void slotMaskOutsideRClicked(bool isChecked);
  virtual void slotMaskRClicked(bool isChecked);
  
  virtual void slotUnmaskRClicked(bool isChecked);
  virtual void slotPencilMaskSphereRClicked(bool isChecked);
  virtual void slotPencilMaskTubeRClicked(bool isChecked);



  virtual void slotisVisibleXYClicked(bool isChecked);
  virtual void slotisVisibleXZClicked(bool isChecked);
  virtual void slotisVisibleYZClicked(bool isChecked);
  virtual void slotisVisibleVRClicked(bool isChecked);
  virtual void slotisVisibleCropBoxClicked(bool isChecked);
  virtual void slotcropVolumeClicked();
  virtual void slotuseImageDataBinForVRClicked(bool isChecked);
  virtual void slotSliderXYChanged(int val);
  virtual void slotSliderXZChanged(int val);
  virtual void slotSliderYZChanged(int val);
  virtual void slotSpinXYChanged(int val);
  virtual void slotSpinXZChanged(int val);
  virtual void slotSpinYZChanged(int val);

  
 /* 
 virtual void slotCurrentMinEdited();
 virtual void slotCurrentMaxEdited();
 virtual void slotShiftSlider(int shift);
  virtual void slotSliderStart();
  virtual void slotSlideMin(int slideMin);
  virtual void slotSlideMax(int slideMax);
  virtual void slotSliderStop();*/

protected:
	
private:
	mqColorOpacityEditorWidget *mColorMap;
	/*double maxShiftAmplitude;
	double slideMin;
	double slideMax;
	double ctfMin;
	double ctfMax;*/

  Q_DISABLE_COPY(mqEditVolumeDialog)
  Ui::mqEditVolumeDialog* const Ui;
  vtkSmartPointer<vtkMDVolume> Volume;
  //vtkMDVolume *Volume;
  vtkMDVolumeCollection *Volume_Coll;
  int current_coll;

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
