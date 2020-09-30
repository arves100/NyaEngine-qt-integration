#include "QNyaEngine.h"

#include <render/render.h>
#include <system/system.h>
#include <system/shaders_cache_provider.h>

QNyaEngine::QNyaEngine(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), m_rot(0.0f)
{
	m_time = nya_system::get_time();
}

QNyaEngine::~QNyaEngine()
{
	destroyGL();
}

void QNyaEngine::paintGL()
{
	unsigned long time = nya_system::get_time();
	unsigned int dt = (unsigned)(time - m_time);
	m_time = time;

	nya_render::clear(true, true);

	m_rot += dt * 0.05f;
	if (m_rot > 360)
		m_rot = 0;

	nya_math::mat4 mv;
	mv.translate(0, 0, -2.0f);
	mv.rotate(30.0f, 1.0f, 0.0f, 0.0f);
	mv.rotate(m_rot, 0.0f, 1.0f, 0.0f);
	nya_render::set_modelview_matrix(mv);

	m_shader.bind();
	m_vbo.bind();
	m_vbo.draw();
	m_vbo.unbind();
	m_shader.unbind();

	static unsigned int fps_counter = 0, fps_update_timer = 0;
	++fps_counter;
	fps_update_timer += dt;
	if (fps_update_timer > 1000)
	{
		std::string a = "FPS: ";
		a += std::to_string(fps_counter);
		a += "\n";
		OutputDebugStringA(a.c_str());

		fps_update_timer %= 1000;
		fps_counter = 0;
	}
}

void QNyaEngine::resizeGL(int w, int h)
{
	nya_log::log() << "on_resize";

	nya_render::set_viewport(0, 0, w, h);

	nya_math::mat4 proj;
	proj.perspective(70.0f, float(w) / h, 0.01f, 100.0f);
	nya_render::set_projection_matrix(proj);
}

void QNyaEngine::initializeGL()
{
	nya_log::log() << "on_init\n";

	nya_render::set_clear_color(0.2f, 0.4f, 0.5f, 0.0f);
	nya_render::set_clear_depth(1.0f);
	nya_render::depth_test::enable(nya_render::depth_test::less);

	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f
	};

	unsigned short indices[] =
	{
		0,2,1, 1,2,3, // -x
		4,5,6, 5,7,6, // +x
		0,1,5, 0,5,4, // -y
		2,6,7, 2,7,3, // +y
		0,4,6, 0,6,2, // -z
		1,3,7, 1,7,5, // +z
	};

	m_vbo.set_vertex_data(vertices, sizeof(float) * 6, 8);
	m_vbo.set_vertices(0, 3);
	m_vbo.set_colors(sizeof(float) * 3, 3);
	m_vbo.set_index_data(indices, nya_render::vbo::index2b,
		sizeof(indices) / sizeof(unsigned short));

	if (nya_render::get_render_api() == nya_render::render_api_directx11)
	{
		static nya_system::compiled_shaders_provider csp;
		csp.set_load_path(nya_system::get_app_path());
		csp.set_save_path(nya_system::get_app_path());
		nya_render::set_compiled_shaders_provider(&csp);
	}

	const char* vs_code =
		"varying vec4 color;"
		"void main()"
		"{"
		"color=gl_Color;"
		"gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;"
		"}";

	const char* ps_code =
		"varying vec4 color;"
		"void main()"
		"{"
		"gl_FragColor=color;"
		"}";

	m_shader.add_program(nya_render::shader::vertex, vs_code);
	m_shader.add_program(nya_render::shader::pixel, ps_code);
}


void QNyaEngine::destroyGL()
{
	// Make sure the context is current and then explicitly
	// destroy all underlying OpenGL resources.
	makeCurrent();

	nya_log::log() << "on_free\n";

	m_vbo.release();
	m_shader.release();
}