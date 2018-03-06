#ifndef QRELEASESCROLLBAR_H
#define QRELEASESCROLLBAR_H

#include <QScrollBar>

class QReleaseScrollBar : public QScrollBar
{
	Q_OBJECT
public:
	explicit QReleaseScrollBar(QWidget *parent = 0);

	

signals:

	public slots :
		void slotRefresh();

private:
	


	void Refresh();

};

#endif // QRELEASESCROLLBAR_H
