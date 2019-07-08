#pragma once
#include "Common.h"

class Sample2 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Core::Input Input;
	//Asset Manager (Loader)
	Managers::AssetManager AssetLoader;
	Managers::CameraManager SceneCameraManager;

	Assets::Mesh* SponzaAsset;

	Assets::Material SphereMaterial;
	Assets::Material* SponzaMaterial;

	Components::CameraComponent Camera;

	Components::DirectionalLight dirlight;
	Components::PointLight pointlight1;

	Components::SpotLight spotLight;

	Graphics::PBR PBR;
	Graphics::DiffuseOnly DiffuseRP;
	Graphics::WireFrame WireFrameRP;

	//ECS
	ECS::Scene PBRScene;
	ECS::Entity ESponza;
	ECS::Entity ESphere;

	// positions of the point lights
	Math::Vector3 pointLightPositions[9] =
	{
		Math::Vector3(0.0f,  0.0f,  10.0f),
	};
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

	void SetupLights()
	{
		dirlight.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dirlight.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		pointlight1.SetIntensity(10.0f);
	}
	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;

		//Load Sponza Model
		std::tie(SponzaAsset, SponzaMaterial) = AssetLoader.Import("Assets/Common/Models/Cerberus.FBX", ModelDesc);

		//Load some textures manually
		Importers::TextureLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRSphereSet;
		PBRSphereSet.mData.push_back({ 0, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/albedo.png", Assets::TextureUsageType::Diffuse) });
		PBRSphereSet.mData.push_back({ 1, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/metallic.png", Assets::TextureUsageType::Specular) });
		PBRSphereSet.mData.push_back({ 2, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/normal.png", Assets::TextureUsageType::Normal) });
		PBRSphereSet.mData.push_back({ 3, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/roughness.png", Assets::TextureUsageType::Roughness) });
		PBRSphereSet.mData.push_back({ 4, AssetLoader.Import("Assets/Common/Textures/PBR/RustedIron/ao.png", Assets::TextureUsageType::AO) });

		SphereMaterial.mPixelShaderTextures.push_back(PBRSphereSet);
		Renderer->CreateMaterialForAllPipelines(&SphereMaterial);
		Renderer->CreateMaterialForAllPipelines(SponzaMaterial);

		PBRSphereSet.mData.clear();
		//CubeMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//CubeMaterial.SetMaterialVariable("Shininess", 64.0f);

		//NanosuitMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//NanosuitMaterial.SetMaterialVariable("Shininess", 64.0f);
	}
	void SetupEntities()
	{
		//Create Entities
		ESphere = PBRScene.CreateEntity();
		ESponza = PBRScene.CreateEntity();

		//Assign Components
		ESphere.Assign<Components::MeshComponent>(Assets::DefaultMeshes::GetSphereAsset(), &SphereMaterial);
		ESponza.Assign<Components::MeshComponent>(SponzaAsset, SponzaMaterial);
	}

	void InitRenderer()
	{
		Renderer = PBRScene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		PBRScene.Systems.Configure();
		Renderer->AddRenderingPipeline(&PBR);
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->AddRenderingPipeline(&WireFrameRP);

		Renderer->AddLight(&spotLight);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&dirlight);
		Renderer->Bake();
	}

	void Load()
	{
		Assets::DefaultTextures::Initalize(&AssetLoader);
		Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(&Camera);

		InitRenderer();

		SetupLights();

		SetupAssets();

		SetupEntities();

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		Math::Matrix4 model;
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = Math::Matrix4(1.0);
				model = Math::translate(model, Math::Vector3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					0.0f
				));
				model = Math::scale(model, Math::Vector3(2.0f));

				ESphere.GetComponent<Components::MeshComponent>()->mMultiRenderTransforms.push_back(model);
			}
		}

		ESphere.GetComponent<Components::MeshComponent>()->mMultiRender = true;


		Math::Matrix4 TSponza(1.0f);
		TSponza = Math::scale(TSponza, Math::Vector3(0.05f));
		ESponza.GetComponent<Components::TransformComponent>()->SetTransform(TSponza);

		Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
	}
	void OnMouseMovement(int xpos_a, int ypos_a) override
	{
		if (!isMouseDisabled)
		{
			float xpos = static_cast<float>(xpos_a);
			float ypos = static_cast<float>(ypos_a);

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;

			Camera.ProcessEye(xoffset, yoffset);
		}
	}

	bool RenderSponza = false;

	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
		Camera.SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
	}

	void Update(float deltatime) override
	{
		//Movement
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_LEFT_SHIFT) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.MovementSpeed = 10;
		else
			Camera.MovementSpeed = 4.5;

		//Change Mouse Mode
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = true;
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
		}
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = false;
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
		}

		Camera.Update();
		SceneCameraManager.UpdateBuffer();
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.3f,  0.3f,  0.3f, 1.0f };
		Graphics::Context::GetContext()->ClearRenderTarget(nullptr, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(nullptr, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());
		
		Renderer->Update(PBRScene.Entities, PBRScene.Events, dt);


		{
			using namespace Graphics;
			ImGui::Begin("Sample2 Control Box");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Text("Active Rendering Pipeline:");
			static int e = 0;
			ImGui::RadioButton("PBR", &e, 0);
			ImGui::RadioButton("DiffuseOnly", &e, 1);
			ImGui::RadioButton("WireFrame", &e, 2);

			//Change Rendering Pipeline
			if (e == 0)
					Renderer->SetActiveRenderingPipeline(PBR.GetID());
			else if (e == 1)
					Renderer->SetActiveRenderingPipeline(DiffuseRP.GetID());
			else if (e == 2)
				Renderer->SetActiveRenderingPipeline(WireFrameRP.GetID());
			
			ESponza.GetComponent<Components::MeshComponent>()->mRender = RenderSponza;
			ESphere.GetComponent<Components::MeshComponent>()->mRender = !RenderSponza;

			ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);
			ImGui::Checkbox("Render Sponza", &RenderSponza);

			static ImVec4 Lightcolor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			static float f = 1.0f;
			ImGui::SliderFloat("PointLight Intensity", &f, 1.0f, 100.0f);
			ImGui::ColorEdit3("PointLight Color", (float*)& Lightcolor);

			pointlight1.SetColor(Graphics::Color(Lightcolor.x, Lightcolor.y, Lightcolor.z, Lightcolor.w));
			pointlight1.SetIntensity(f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};