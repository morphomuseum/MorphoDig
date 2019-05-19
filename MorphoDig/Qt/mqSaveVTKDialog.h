/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveVTKDialog_h
#define _mqSaveVTKDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveVTKDialog;
}




/**
* mqSaveVTKDialog is the dialog used by MorphoDig to save VTK files.
*/
class  mqSaveVTKDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveVTKDialog(QWidget* Parent, QString fileName);
  
  ~mqSaveVTKDialog();

  public slots:
  virtual void slotSaveVTKFile();
  virtual void slotVTKVTPClicked();
  

protected:
	
private:
	QString m_fileName;
  Q_DISABLE_COPY(mqSaveVTKDialog)
  Ui::mqSaveVTKDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
