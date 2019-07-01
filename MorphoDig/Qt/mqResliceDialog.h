/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqResliceDialog_h
#define _mqResliceDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqResliceDialog;
}




/**
* mqResliceDialog is the dialog used by MorphoDig to Reslice the 1st selected volume
*/
class  mqResliceDialog : public QDialog
{
  Q_OBJECT

public:
  mqResliceDialog(QWidget* Parent);
  
  ~mqResliceDialog();
  void Reslice();


  void setVolume(vtkMDVolume *vol);
  public slots:
  

  virtual void slotReslice();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqResliceDialog)
  Ui::mqResliceDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // 