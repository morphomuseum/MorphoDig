/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveMHDMHADialog_h
#define _mqSaveMHDMHADialog_h

#include <QDialog>
#include "vtkMDVolume.h"
namespace Ui
{
class mqSaveMHDMHADialog;
}




/**
* mqSaveMHDMHADialog is the dialog used by MorphoDig to save MHDMHA files.
*/
class  mqSaveMHDMHADialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveMHDMHADialog(QWidget* Parent, QString fileName, int _mode=0);
  
  ~mqSaveMHDMHADialog();

  public slots:
  virtual void slotSaveMHDMHAFile();
  
  

protected:
	
private:
	QString m_fileName;
	vtkMDVolume *myVolume;
  Q_DISABLE_COPY(mqSaveMHDMHADialog)
  Ui::mqSaveMHDMHADialog* const Ui;
  int Mode;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
