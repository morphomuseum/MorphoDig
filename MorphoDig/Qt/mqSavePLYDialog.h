/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSavePLYDialog_h
#define _mqSavePLYDialog_h

#include <QDialog>

namespace Ui
{
class mqSavePLYDialog;
}




/**
* mqSavePLYDialog is the dialog used by MorphoDig to save PLY files.
*/
class  mqSavePLYDialog : public QDialog
{
  Q_OBJECT

public:
  mqSavePLYDialog(QWidget* Parent, QString fileName);
  
  ~mqSavePLYDialog();

  public slots:
  virtual void slotSavePlyFile();
  
  

protected:
	
private:
	QString m_fileName;
  Q_DISABLE_COPY(mqSavePLYDialog)
  Ui::mqSavePLYDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
