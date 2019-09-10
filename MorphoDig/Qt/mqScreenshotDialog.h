/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScreenshotDialog_h
#define _mqScreenshotDialog_h


#include <QDialog>

namespace Ui
{
class mqScreenshotDialog;
}




/**
* mqScreenshotDialog is the dialog used by MorphoDig to take screenshots
*/
class  mqScreenshotDialog : public QDialog
{
  Q_OBJECT

public:
  mqScreenshotDialog(QWidget* Parent, QString fileName);
  
  ~mqScreenshotDialog();
  void Screenshot();
  
  public slots:
  
  
  
  virtual void slotScreenshot();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqScreenshotDialog)
  Ui::mqScreenshotDialog* const Ui;
  QString m_fileName;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
