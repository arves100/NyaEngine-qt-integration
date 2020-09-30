#pragma once

#include <QOpenGLWidget>

#include <render/vbo.h>
#include <render/shader.h>

class QNyaEngine : public QOpenGLWidget
{
public:
	QNyaEngine(QWidget* parent = nullptr, Qt::WindowFlags f = { 0 });
	virtual ~QNyaEngine();

protected:
	void paintGL();
	void resizeGL(int w, int h);
	void initializeGL();
	void destroyGL();

private:
	nya_render::vbo m_vbo;
	nya_render::shader m_shader;
	float m_rot;
	unsigned long m_time;
};
