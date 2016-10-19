#include "crendertiles.h"
#include "GLM\ext.hpp"



// A type of geometric object (with information)
// -> Multiple Shaders -> Only target One FBO

struct RenderComponent
{
	// We want the gameObject's scale to be 1,
	// but the model doesn't fit that scale.
	// so we might an extra model matrix to bring the matrix
	// into the right orientation/size for our game object.

	glm::mat4 model;
	Geometry mesh;

	Texture norm, diff, spec;

	Shader  shader;
	Framebuffer fb;

	// gpass shader for this object.
	// draw(Shader,Target, Transform)
	// tdraw(...);
};

struct DirectionalLight
{
	glm::vec3 dir, color;
	float size;
	glm::mat4 getMatrix()
	{
		return glm::ortho<float>(-size, size, -size, size, -size, size)
			* glm::lookAt(-dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}
};




void main()
{

	//foo(34.f);
	/****************/
	// 45 degrees, length of 1.
	float x = 1, y = 1;

	// Half that angle means we should have to move
	// backwards 1 units.
	float a = 120 * 0.0174533;

	float d = y / tan(a / 2) - x;

	/////////////////////
	Window context;
	context.init(1280, 720);


	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadOBJ("../res/models/soulspear.obj");
	Geometry sphere = loadOBJ("../res/models/sphere.obj");

	Geometry cube = loadOBJ("../res/models/cube.obj");

	Geometry gaige[25];
	int gaige_count = loadAllOBJ("../res/models/Gaige.obj", gaige, 25);


	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

	Texture randDirTex = loadTexture("../res/textures/randnormals.jpg");

	Texture spear_Textures[3] = { spear_normal, spear_diffuse, spear_specular };

	const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
	Texture vertex_normals = makeTexture(1, 1, 4, norm_pixels);

	const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
	Texture white = makeTexture(1, 1, 4, white_pixels);

	Shader qdraw = loadShader("../res/shaders/quad.vert", "../res/shaders/quad.frag", false);
	Shader gpass = loadShader("../res/shaders/gpass.vert", "../res/shaders/gpass.frag");

	//////////////////////////////
	//// Using Light pass shader designed for shadows.
	//// Note that shadow pass can disable face-culling for some back-shadow improvements.
	Shader spass = loadShader("../res/shaders/spass.vert", "../res/shaders/spass.frag", true, false, false);
	Shader lpass = loadShader("../res/shaders/lspass.vert", "../res/shaders/lspass.frag", false, true);

	Shader ssaoPass = loadShader("../res/shaders/ssao.vert", "../res/shaders/ssao.frag");

	Shader cubePass = loadShader("../res/shaders/cube.vert", "../res/shaders/cube.frag", false, false, false);

	Framebuffer screen = { 0, 1280, 720 };

	// colors don't need floats, but position/normal should use it.
	bool isFtex[] = { false, true, false, true };

	Framebuffer gframe = makeFramebuffer(1280, 720, 4, isFtex);
	Framebuffer lframe = makeFramebuffer(1280, 720, 3);

	int g = 1;
	Framebuffer aoframe = makeFramebuffer(1280, 720, 1, (bool*)&g, (int*)&g);

	// Temporary shadow framebuffer. Will be cleared and reused by each light!
	// Its RESOLUTION WILL GREATLY EFFECT THE QUALITY. Try playing around with high/low res.
	Framebuffer sframe = makeFramebuffer(4096, 4096, 0);



	CubeTexture cbmp = loadCubeMap("../res/textures/stormydays_rt.tga", "../res/textures/stormydays_lf.tga",
		"../res/textures/stormydays_up.tga", "../res/textures/stormydays_dn.tga",
		"../res/textures/stormydays_bk.tga", "../res/textures/stormydays_ft.tga");


	// Camera information
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 camProj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	// Model Matrices
	glm::mat4 spearModel; // ROTATES in main
	glm::mat4 sphereModel = glm::translate(glm::vec3(0.3f, -1, -0.2f));
	glm::mat4 wallModel = glm::rotate(45.f, glm::vec3(0, -1, 0)) * glm::translate(glm::vec3(0, 0, -2)) * glm::scale(glm::vec3(2, 2, 1));
	glm::mat4 gaigeModel = glm::translate(glm::vec3(0, -2.f, 0))*glm::scale(glm::vec3(.05f, .05f, .05f))*glm::rotate(90.f, -glm::vec3(1, 0, 0));

	// Light Matrices and data
	// They can all use the same projection matrix...
	glm::mat4 lightProj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);

	glm::mat4   redView = glm::lookAt(glm::normalize(-glm::vec3(1, -1, -1)),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	glm::vec4   redColor = glm::vec4(1, 0, 0, 1);

	glm::mat4 greenView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 greenColor = glm::vec4(0, 1, 0, 1);

	float time = 0;

	while (context.step())
	{
		time += 0.001;
		glm::mat4 cubeBox = camProj * glm::scale(glm::vec3(5, 5, 5)) * glm::rotate(time, glm::vec3(0, 1, 0));

		tdraw(cubePass, cube, screen, cubeBox, cbmp);

		/////////////////////////////////////////////////////
		// Geometry Pass
		//
		clearFramebuffer(gframe);
		//tdraw(gpass, spear,  gframe, spearModel,  camView, camProj, white, spear_normal, white);
		//tdraw(gpass, sphere, gframe, sphereModel, camView, camProj, white, vertex_normals, white);
		tdraw(gpass, quad, gframe, wallModel, camView, camProj, white, vertex_normals, white, cbmp);

		///////////////////
		// FOR ARTHUR!!!!!!
		for (int i = 0; i < gaige_count - 1; ++i)
			tdraw(gpass, gaige[i], gframe, gaigeModel, camView, camProj, spear_diffuse, vertex_normals, white, cbmp);


		clearFramebuffer(aoframe);
		tdraw(ssaoPass, quad, aoframe, gframe.colors[1], gframe.colors[2], randDirTex);

		//tdraw(blur, quad, nframe, gframe.colors[1]);

		/////////////////////////////////////////////////////
		//// Light pass!
		clearFramebuffer(lframe);
		//////////////////////////
		// RED LIGHT

		// Shadow PrePass
		clearFramebuffer(sframe);
		//tdraw(spass, spear,  sframe, spearModel,  redView, lightProj);
		//tdraw(spass, sphere, sframe, sphereModel, redView, lightProj);
		tdraw(spass, quad, sframe, wallModel, redView, lightProj);

		for (int i = 0; i < gaige_count; ++i)
			tdraw(spass, gaige[i], sframe, gaigeModel, redView, lightProj);


		// Light Aggregation
		tdraw(lpass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, redColor, redView, lightProj);

		//////////////////////////
		// Green light!

		// Reuse the shadow pass!
		clearFramebuffer(sframe);
		tdraw(spass, spear,  sframe, spearModel,  greenView, lightProj);
		tdraw(spass, sphere, sframe, sphereModel, greenView, lightProj);
		tdraw(spass, quad, sframe, wallModel, greenView, lightProj);

		for (int i = 0; i < gaige_count; ++i)
			tdraw(spass, gaige[i], sframe, gaigeModel, greenView, lightProj);

		// add the green light now.
		tdraw(lpass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, greenColor, greenView, lightProj);



		//////////////////////////////////////////////////
		// Debug Rendering Stuff. Just single textures to quads-
		// drawing most of the images I've gathered so far.

		// note that the sframe (shadow pass) will only be from the most recent light.
		Texture debug_list[] = { gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			gframe.depth, lframe.colors[1], lframe.colors[2], sframe.depth };

		for (int i = 0; i < sizeof(debug_list) / sizeof(Texture); ++i)
		{
			glm::mat4 mod = glm::translate(glm::vec3(-.75f + .5f*(i % 4), 0.75f - .5f*(i / 4), 0))
				* glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			tdraw(qdraw, quad, screen, debug_list[i], mod);
		}

		glm::mat4 mod =
			glm::translate(glm::vec3(-.5f, -0.5f, 0)) *
			glm::scale(glm::vec3(0.5f, 0.5f, 1.f));
		tdraw(qdraw, quad, screen, aoframe.colors[0], mod);

	}

	context.term();
}