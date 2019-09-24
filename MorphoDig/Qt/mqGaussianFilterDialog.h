/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqGaussianFilterDialog_h
#define _mqGaussianFilterDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqGaussianFilterDialog;
}




/**
* mqGaussianFilterDialog is the dialog used by MorphoDig to create an GaussianFilter from the 1st selected volume
*/
class  mqGaussianFilterDialog : public QDialog
{
  Q_OBJECT

public:
  mqGaussianFilterDialog(QWidget* Parent);
  
  ~mqGaussianFilterDialog();
  void GaussianFilter();
  void setVolume(vtkMDVolume *vol);
  public slots:
  
  
  
  virtual void slotGaussianFilter();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqGaussianFilterDialog)
  Ui::mqGaussianFilterDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
