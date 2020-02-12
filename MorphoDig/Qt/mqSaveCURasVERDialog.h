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

class SignalBlocker2 {
public:
	SignalBlocker2(QObject *o) : object(o), alreadyBlocked(object->signalsBlocked()) {
		if (!alreadyBlocked) {
			object->blockSignals(true);
		}
	}
	~SignalBlocker2() {
		if (!alreadyBlocked) {
			object->blockSignals(false);
		}
	}

private:
	QObject *object;
	bool alreadyBlocked;
};


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

	
	virtual void slotDecimationChanged(int newdecimation);
	virtual void slotDefaultDecimationChanged(int newdecimation);
	virtual void slotReinitializeSegments();
protected:
	
private:
	

  Q_DISABLE_COPY(mqSaveCURasVERDialog)
  Ui::mqSaveCURasVERDialog* const Ui;
  
  void RefreshDecimationTable(int toDefault=0);
  void updateDecimation(int row, int newdecimation);
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
