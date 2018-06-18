/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveTAGMAPDialog_h
#define _mqSaveTAGMAPDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveTAGMAPDialog;
}




/**
* mqSaveTAGMAPDialog is the dialog used by MorphoDig to save TAGMAP files.
*/
class  mqSaveTAGMAPDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveTAGMAPDialog(QWidget* Parent);
  
  ~mqSaveTAGMAPDialog();

  public slots:
  virtual void slotSaveTAGMAPFile();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveTAGMAPDialog)
  Ui::mqSaveTAGMAPDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqTAGMAPDialog_h
