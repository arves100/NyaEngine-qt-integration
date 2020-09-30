#pragma once

#include <QMainWindow>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	virtual ~MainWindow();

private:
	Ui_MainWindow m_pcUi;

private slots:
	void onAboutQt();
};
