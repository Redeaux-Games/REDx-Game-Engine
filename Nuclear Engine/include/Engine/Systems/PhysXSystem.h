#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\ECS\Scene.h>
#include "Engine\PhysX\PhysXEngine.h"
#include<Engine\Components\RigidActorComponent.h>

namespace NuclearEngine
{
	namespace Systems
	{
		class NEAPI PhysXSystem : public ECS::System<PhysXSystem> {
		public:
			PhysXSystem(ECS::Scene* scene, PhysX::PxSceneDesc sceneDesc);
			~PhysXSystem();

			void AddActor(ECS::Entity entity);

			//Must Be called before update!
			void BeginSimulation(ECS::TimeDelta dt);

			void Update(ECS::EntityManager& es, ECS::EventManager& events, ECS::TimeDelta dt) override;
		private:
			ECS::Scene* mScene = nullptr;
		};

	}
}