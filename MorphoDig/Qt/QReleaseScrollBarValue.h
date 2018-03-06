#ifndef QRELEASESCROLLBARVALUE_H
#define QRELEASESCROLLBARVALUE_H

#include <QWidget>

class QDial;
class QScrollBar;
class QReleaseScrollBar;
class QReleaseSlider;
//class QDoubleSlider;
class QSpinBox;

class QReleaseScrollBarValue : public QWidget
{
	Q_OBJECT

public:
	QReleaseScrollBarValue(Qt::Orientation orientation, const QString &title,
		QWidget *parent = 0);

signals:
	void valueChanged(int value);

	public slots:
	void setValue(int value);
	void setMinimum(int value);
	void setMaximum(int value);
	void invertAppearance(bool invert);
	void invertKeyBindings(bool invert);

private:
	//QReleaseSlider *slider;
	QReleaseScrollBar *slider;
	QSpinBox *spinbox;
	QDial *dial;
};

#endif