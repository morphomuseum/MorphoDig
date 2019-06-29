/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSpacingDialog_h
#define _mqSpacingDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqSpacingDialog;
}




/**
* mqSpacingDialog is the dialog used by MorphoDig to flip or swap the 1st selected volume
*/
class  mqSpacingDialog : public QDialog
{
  Q_OBJECT

public:
  mqSpacingDialog(QWidget* Parent);
  
  ~mqSpacingDialog();
  void Spacing();
  void setVolume(vtkMDVolume *vol);
  public slots:
  
  virtual void slotDiv10();
  virtual void slotDiv5();
  virtual void slotDiv2();
  virtual void slotMult10();
  virtual void slotMult5();
  virtual void slotMult2();
  virtual void slotVoxelSizeXChanged(double newsVoxelSizeX);
  virtual void slotSpacing();
  

protected:
	
private:
	double mySpacingX, mySpacingY, mySpacingZ;

  Q_DISABLE_COPY(mqSpacingDialog)
  Ui::mqSpacingDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // 