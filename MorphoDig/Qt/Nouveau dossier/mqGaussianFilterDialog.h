/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqMedianFilterDialog_h
#define _mqMedianFilterDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqMedianFilterDialog;
}




/**
* mqMedianFilterDialog is the dialog used by MorphoDig to create an MedianFilter from the 1st selected volume
*/
class  mqMedianFilterDialog : public QDialog
{
  Q_OBJECT

public:
  mqMedianFilterDialog(QWidget* Parent);
  
  ~mqMedianFilterDialog();
  void MedianFilter();
  void setVolume(vtkMDVolume *vol);
  public slots:
  
  
  
  virtual void slotMedianFilter();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqMedianFilterDialog)
  Ui::mqMedianFilterDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
