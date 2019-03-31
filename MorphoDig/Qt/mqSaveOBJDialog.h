/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveOBJDialog_h
#define _mqSaveOBJDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveOBJDialog;
}




/**
* mqSaveOBJDialog is the dialog used by MorphoDig to save OBJ files.
*/
class  mqSaveOBJDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveOBJDialog(QWidget* Parent, QString fileName);
  
  ~mqSaveOBJDialog();

  public slots:
  virtual void slotSaveOBJFile();
  
  

protected:
	
private:
	QString m_fileName;
  Q_DISABLE_COPY(mqSaveOBJDialog)
  Ui::mqSaveOBJDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
