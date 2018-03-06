/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveSTVDialog_h
#define _mqSaveSTVDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveSTVDialog;
}




/**
* mqSaveSTVDialog is the dialog used by MorphoDig to save STV files.
*/
class  mqSaveSTVDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveSTVDialog(QWidget* Parent);
  
  ~mqSaveSTVDialog();

  public slots:
  virtual void slotSaveSTVFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveSTVDialog)
  Ui::mqSaveSTVDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqSTVDialog_h
