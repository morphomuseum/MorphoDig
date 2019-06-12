/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqIsosurfaceDialog_h
#define _mqIsosurfaceDialog_h

#include "vtkMDVolume.h"
#include <QDialog>

namespace Ui
{
class mqIsosurfaceDialog;
}




/**
* mqIsosurfaceDialog is the dialog used by MorphoDig to create an Isosurface from the 1st selected volume
*/
class  mqIsosurfaceDialog : public QDialog
{
  Q_OBJECT

public:
  mqIsosurfaceDialog(QWidget* Parent);
  
  ~mqIsosurfaceDialog();
  void Isosurface();
  void setVolume(vtkMDVolume *vol);
  public slots:
  
  
  
  virtual void slotIsosurface();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqIsosurfaceDialog)
  Ui::mqIsosurfaceDialog* const Ui;
  vtkMDVolume *myVolume;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
