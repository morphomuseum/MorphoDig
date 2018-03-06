/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveCURasVERDialog_h
#define _mqSaveCURasVERDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveCURasVERDialog;
}




/**
* mqSaveCURasVERDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqSaveCURasVERDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveCURasVERDialog(QWidget* Parent);
  
  ~mqSaveCURasVERDialog();

  public slots:
  virtual void slotSaveCURasVERFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveCURasVERDialog)
  Ui::mqSaveCURasVERDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
