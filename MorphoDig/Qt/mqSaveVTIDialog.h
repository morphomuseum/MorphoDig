/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveVTIDialog_h
#define _mqSaveVTIDialog_h

#include <QDialog>
#include "vtkMDVolume.h"
namespace Ui
{
class mqSaveVTIDialog;
}




/**
* mqSaveVTIDialog is the dialog used by MorphoDig to save VTI files.
*/
class  mqSaveVTIDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveVTIDialog(QWidget* Parent, QString fileName);
  
  ~mqSaveVTIDialog();

  public slots:
  virtual void slotSaveVTIFile();
  
  

protected:
	
private:
	QString m_fileName;
	vtkMDVolume *myVolume;
  Q_DISABLE_COPY(mqSaveVTIDialog)
  Ui::mqSaveVTIDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
