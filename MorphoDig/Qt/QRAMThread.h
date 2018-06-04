#ifndef QRAMTHREAD_H
#define QRAMTHREAD_H

#include <QThread>

class QRAMThread : public QThread
{
	Q_OBJECT
public:
	explicit QRAMThread(QThread *parent = 0);
	void run() override;
	int exec();
	//explicit QReleaseSlider(QWidget *parent = 0);

	

signals:
	//void resultReady(const QString &s);
	void usedRAM(int percent);
	
	public slots :
	//


};

#endif // QRAMTHREAD_H
