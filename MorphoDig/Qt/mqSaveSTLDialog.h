/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveSTLDialog_h
#define _mqSaveSTLDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveSTLDialog;
}




/**
* mqSaveSTLDialog is the dialog used by MorphoDig to save STL files.
*/
class  mqSaveSTLDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveSTLDialog(QWidget* Parent, QString fileName);
  
  ~mqSaveSTLDialog();

  public slots:
  virtual void slotSaveSTLFile();
  
  

protected:
	
private:
	QString m_fileName;
  Q_DISABLE_COPY(mqSaveSTLDialog)
  Ui::mqSaveSTLDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
