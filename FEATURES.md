# Features

+ __Core__
  - Cross platform ready.
  - Modern Fast code using C++17.
  - Fast Robust Entity Component System "ECS" using EntityX.
  - Cross-platform Application creation through GLFW.
  - Support for XML & INI Parsers.
  - Cross-platform ready.
  
+ __Managers__  
  - AssetManager
    - Support for Reading PNG, JPEG, BMP, DDS, GIF, HDR, PSD and many more image formats (using Freeimage library).
    - Support for Reading OBJ, FBX, Blend, 3DS, CAD, GLtf and many more 3D model formats (using Assimp library).
	- Support for Reading OGG, FLAC, WAV, MP3 and many more audio formats (using FMOD library).
	- Prevention of repeated loading of a texture or mesh.
	- Provides access to all the created Textures and Meshes thorugh their hashed name.
	
  - ShaderManager
    - Support for automated shader creation instead of Re-Writing them

+ ___Audio___
  - Support for FMOD LowLevel API
  - Support for Preloading audio files to memory then playing them 			"Sound"

+ __Graphics__
  - Support for Abstracted RenderAPI called Diligent that supports DirectX 11, DirectX 12, Vulkan backends.
  - Uses HLSL Shaders which is Cross Compiled to GLSL or SPIR-V automatically by Diligent.
  - Supports 'dear ImGui'.
  - Basic Diffuse only SkyBox.
  - Available Rendering Pipelines:
  - Diffuse Only
	- BlinnPhong (Diffuse/Specualr only)
	- BlinnPhong with Normal maps
  - Basic Textured PBR (No IBL)
	
+ __Rendering__
  - Features Easy-to-use RenderSystem that create material and manage lighting by itself.
  - The RenderSystem support what is called "Rendering Pipelines", that allow rendering large number of meshes
  with different materials using different rendering methods such as(PBR - BlinnPhong) without having to tinker 
  with low level API stuff as shader or configuring material with each pipeline as everything is done automatically.

  - Render System and Render pipeline can be extended easily through simply inheriting from base class.
  - Material are auto-created and auto-configured for each pipeline without user intervention.
  
+ __Physics__
  - Using PhysX4.1, next version will use PhysX5.
  - Supports Collider, Ridgid Body Collisions, Ridgid Body Dynamics, Advanced Collision, Joints, Articulations, Vehicles and more.  

+ __Modules__
  - REDAI, next-gen AI algorithm for enemies, NPCs, weapons, vehicles, weather and environments. We build datasets based on game play and build up the AIs interactivity using machine learning.
  - NPC2PC, non playable character 2 player communication. Using speech to text, then text to speech, plugin into an AI speech sytensis module.
  - P2P Multi-player with 3d positional audio.
  - Dust3D (module)
  - EnTT ECS (module)
  - Quixel Mixer Megascan support (module)
  - Epic Online Services SDK (module) left_speech_bubble
  - Cinder Multimedia Framework (module)
  - VirtualCameraVTK (Images to 3d models) (module) camera_flash
