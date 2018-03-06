/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveFlagsDialog_h
#define _mqSaveFlagsDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveFlagsDialog;
}




/**
* mqSaveFlagDialog is the dialog used by MorphoDig to save Flag files.
*/
class  mqSaveFlagsDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveFlagsDialog(QWidget* Parent);
  
  ~mqSaveFlagsDialog();

  public slots:
  virtual void slotSaveFlagFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveFlagsDialog)
  Ui::mqSaveFlagsDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqFlagDialog_h
