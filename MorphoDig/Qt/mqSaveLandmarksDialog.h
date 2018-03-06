/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSaveLandmarksDialog_h
#define _mqSaveLandmarksDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveLandmarksDialog;
}




/**
* mqSaveLandmarkDialog is the dialog used by MorphoDig to save Landmark files.
*/
class  mqSaveLandmarksDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveLandmarksDialog(QWidget* Parent, int lmtype);
  
  ~mqSaveLandmarksDialog();

  public slots:
  virtual void slotSaveLandmarkFile();
  
  

protected:
	
private:
	
	int m_lmtype; //0 normal, 1 target, 2 node, 3 handle
  Q_DISABLE_COPY(mqSaveLandmarksDialog)
  Ui::mqSaveLandmarksDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
