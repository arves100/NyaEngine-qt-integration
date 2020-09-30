#include "MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	m_pcUi.setupUi(this);
}

MainWindow::~MainWindow()
{

}

// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV QT SLOTS VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

void MainWindow::onAboutQt()
{
	QMessageBox::aboutQt(this);
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ QT SLOTS ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
