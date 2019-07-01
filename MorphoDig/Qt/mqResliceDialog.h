/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqResampleDialog_h
#define _mqResampleDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqResampleDialog;
}




/**
* mqResampleDialog is the dialog used by MorphoDig to resample the 1st selected volume
*/
class  mqResampleDialog : public QDialog
{
  Q_OBJECT

public:
  mqResampleDialog(QWidget* Parent);
  
  ~mqResampleDialog();
  void Resample();
  void disconnectDims();
  void reconnectDims();
  void disconnectRes();
  void reconnectRes();

  void setVolume(vtkMDVolume *vol);
  public slots:
  
  virtual void slotDiv10();
  virtual void slotDiv5();
  virtual void slotDiv2();
  virtual void slotMult10();
  virtual void slotMult5();
  virtual void slotMult2();
  virtual void slotVoxelSizeXChanged(double newVoxelSizeX);
  virtual void slotVoxelSizeYChanged(double newVoxelSizeY);
  virtual void slotVoxelSizeZChanged(double newVoxelSizeZ);
  virtual void slotDimXChanged(int newDimX);
  virtual void slotDimYChanged(int newDimY);
  virtual void slotDimZChanged(int newDimZ);
  virtual void slotResample();
  

protected:
	
private:
	double mySpacingX, mySpacingY, mySpacingZ;
	int myDimX, myDimY, myDimZ;
	double mySizeX, mySizeY, mySizeZ;

  Q_DISABLE_COPY(mqResampleDialog)
  Ui::mqResampleDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // 