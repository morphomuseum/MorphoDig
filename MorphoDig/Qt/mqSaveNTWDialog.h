/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveNTWDialog_h
#define _mqSaveNTWDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveNTWDialog;
}




/**
* mqSaveNTWDialog is the dialog used by MorphoDig to save NTW files.
*/
class  mqSaveNTWDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveNTWDialog(QWidget* Parent);
  
  ~mqSaveNTWDialog();

  public slots:
  virtual void slotSaveNTWFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveNTWDialog)
  Ui::mqSaveNTWDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqNTWDialog_h
