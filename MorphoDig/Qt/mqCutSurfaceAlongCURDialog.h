/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqCutSurfaceAlongCURDialog_h
#define _mqCutSurfaceAlongCURDialog_h

#include <QDialog>

namespace Ui
{
class mqCutSurfaceAlongCURDialog;
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
* mqCutSurfaceAlongCURDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqCutSurfaceAlongCURDialog : public QDialog
{
  Q_OBJECT

public:
  mqCutSurfaceAlongCURDialog(QWidget* Parent);
  
  ~mqCutSurfaceAlongCURDialog();

  public slots:
	  virtual void slotCutSurface();

	
	virtual void slotDecimationChanged(int newdecimation);
	virtual void slotDefaultDecimationChanged(int newdecimation);
	virtual void slotReinitializeSegments();
protected:
	
private:
	

  Q_DISABLE_COPY(mqCutSurfaceAlongCURDialog)
  Ui::mqCutSurfaceAlongCURDialog* const Ui;
  
  void RefreshDecimationTable(int toDefault=0);
  void updateDecimation(int row, int newdecimation);
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
