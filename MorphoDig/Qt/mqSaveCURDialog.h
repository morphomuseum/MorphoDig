/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveCURDialog_h
#define _mqSaveCURDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveCURDialog;
}




/**
* mqSaveCURDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqSaveCURDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveCURDialog(QWidget* Parent);
  
  ~mqSaveCURDialog();

  public slots:
  virtual void slotSaveCURFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveCURDialog)
  Ui::mqSaveCURDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
