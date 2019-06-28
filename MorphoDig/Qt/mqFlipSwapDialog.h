/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqFlipSwapDialog_h
#define _mqFlipSwapDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqFlipSwapDialog;
}




/**
* mqFlipSwapDialog is the dialog used by MorphoDig to flip or swap the 1st selected volume
*/
class  mqFlipSwapDialog : public QDialog
{
  Q_OBJECT

public:
  mqFlipSwapDialog(QWidget* Parent);
  
  ~mqFlipSwapDialog();
  
  void setVolume(vtkMDVolume *vol);
  public slots:
  
  virtual void slotFlipX();
  virtual void slotFlipY();
  virtual void slotFlipZ();
  virtual void slotSwapXY();
  virtual void slotSwapXZ();
  virtual void slotSwapYZ();
  virtual void slotSwapXYZ();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqFlipSwapDialog)
  Ui::mqFlipSwapDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // 