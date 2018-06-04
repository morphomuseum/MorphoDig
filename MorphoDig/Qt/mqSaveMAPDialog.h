/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveMAPDialog_h
#define _mqSaveMAPDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveMAPDialog;
}




/**
* mqSaveMAPDialog is the dialog used by MorphoDig to save MAP files.
*/
class  mqSaveMAPDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveMAPDialog(QWidget* Parent);
  
  ~mqSaveMAPDialog();

  public slots:
  virtual void slotSaveMAPFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveMAPDialog)
  Ui::mqSaveMAPDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqMAPDialog_h
