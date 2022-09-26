#include "pch.h"
#include "GameObjectTemplate.h"

#include "SphereCollider.h"
#include "SkinnedMeshRenderer.h"
#include "Animator.h"
#include "SpriteAnimator.h"
#include "RayCollider.h"

#include "PlayerStateMachine.h"
#include "PlayerController.h"
#include "InventoryUI.h"
#include "PlayerCamera.h"
#include "PlayerCollider.h"
#include "PlayerMonoBehaviour.h"
#include "SwordAttack.h"

#include "Entrance.h"

#include "RewardBoxMonoBehaviour.h"
#include "IncreaseHp.h"
#include "MemoryGem.h"
#include "Money.h"


#include "AltarState.h"
#include "AltarMonoBehaviour.h"
#include "WeaponAltarState.h"
#include "WeaponAltarMonoBehaviour.h"

#include "DefaultWeapon.h"
#include "WeaponMonoBehaviour.h"

#include "MonsterState.h"
#include "MonsterMonoBehaviour.h"
#include "MonsterAttack.h"

#include "LongRangeMonsterState.h"

#include "BossTree.h"
#include "BossMonoBehaviour.h"
#include "BossAttackMonoBehaviour.h"
#include "BossBullet.h"

#include "SceneController.h"

#include "GameState.h"
#include "UIManager.h"

#include "DebugComponent.h"


namespace Heracles
{
	namespace InGameObject
	{
		std::shared_ptr<GameObject> CreatePlayer(SceneBase* scene)
		{
			std::shared_ptr<GameObject> player = scene->CreateEmpty();
			player->SetName(TEXT("Player"));
			player->SetTag(TEXT("Player"));

			player->GetComponent<Transform>()->SetScale(Vector(0.04f, 0.04f, 0.04f));
			player->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, -10.f));
			player->AddComponent<Player::PlayerStateMachine>()->skill = CreateSwordAttackNormal(scene);

			player->AddComponent<Player::PlayerController>();
			
			player->AddComponent<Player::PlayerCollider>()->SetSize(Vector(4.f, 10.f, 3.f));
			player->AddComponent<RayCollider>();
			player->AddComponent<InventoryUI>();

			player->AddComponent<SkinnedMeshRenderer>()->Init("Heracles", "Resources/Model/Heracles_attack3.ase", RendererBase::ModelType::ALL);
			player->AddComponent<Animator>()->AddAnimation("Resources/Model/Heracles_idle.ase", "Idle", 1.0f, true );
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_run.ase", "Run", 1.0f, true);
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_attack1.ase", "Attack1",1.5f,false);
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_attack2.ase", "Attack2",1.4f,false);
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_attack3.ase", "Attack3",1.5f,false);
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_hit.ase", "Hit", 2.f, false);
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_death.ase", "Death"); 
			player->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_skill.ase", "Skill"); 

			std::shared_ptr<GameObject> weaponObject = scene->CreateEmpty();
			weaponObject->SetName(TEXT("WeaponClub"));
			player->SetChild(weaponObject);

			weaponObject->AddComponent<SkinnedMeshRenderer>()->Init("WeaponClub", "Resources/Model/Heracles_attack1_weapon.ase", RendererBase::ModelType::NONE_RMA);
			weaponObject->AddComponent<Animator>()->AddAnimation("Resources/Model/Heracles_attack1_weapon.ase", "Attack1", 1.5f, false);
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_attack2_weapon.ase", "Attack2", 1.4f, false);
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_attack3_weapon.ase", "Attack3", 1.5f, false);
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_death_weapon.ase", "Death");
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_hit_weapon.ase", "Hit",2.f,false);
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_idle_weapon.ase", "Idle", 1.0f, true);
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_run_weapon.ase", "Run", 1.0f, true);
			weaponObject->GetComponent<Animator>()->AddAnimation("Resources/Model/Heracles_skill_weapon.ase", "Skill");

			//player attack Collider
			{
				std::shared_ptr<GameObject> weapon = scene->CreateEmpty();
				player->SetChild(weapon);
				weapon->SetName(TEXT("Weapon"));
				weapon->SetTag(TEXT("Weapon"));

				weapon->AddComponent<Weapon::DefaultWeapon>();
				weapon->AddComponent<Weapon::WeaponMonoBehaviour>();
				weapon->AddComponent<BoxCollider>()->SetSize(Vector(4.f, 10.f, 4.f));
				weapon->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 90.f),Space::LOCAL);
			}

			//피격
			{
				std::shared_ptr<GameObject> hitEffect = scene->CreateEmpty();
				player->SetChild(hitEffect);

				hitEffect->SetName(TEXT("HitEffect"));
				hitEffect->SetTag(TEXT("HitEffect"));

				hitEffect->AddComponent<SpriteAnimator>()->Init("HitEffect");
				hitEffect->GetComponent<SpriteAnimator>()->SetSpriteTexture("HitEffect", "Resources/Effect/playerHit.png", 2048, 1024, 4, 2, 0.05f);
				hitEffect->GetComponent<SpriteAnimator>()->SetPosition(Vector(-5.f, 7.f, -5.f));
				hitEffect->GetComponent<SpriteAnimator>()->SetWidth(5.0f);
				hitEffect->GetComponent<SpriteAnimator>()->SetHeight(5.0f);
				hitEffect->GetComponent<SpriteAnimator>()->SetLoop(false);

			}

			// 이동
			{
				std::shared_ptr<GameObject> moveEffect = scene->CreateEmpty();
				player->SetChild(moveEffect);

				moveEffect->SetName(TEXT("MoveEffect"));
				moveEffect->SetTag(TEXT("MoveEffect"));
				moveEffect->AddComponent<SpriteAnimator>()->Init("MoveEffect");
				moveEffect->GetComponent<SpriteAnimator>()->SetSpriteTexture("MoveEffect", "Resources/Effect/PlayerMove.png", 2048, 2048, 4, 4, 0.01f);
				moveEffect->GetComponent<SpriteAnimator>()->SetWidth(2.0f);
				moveEffect->GetComponent<SpriteAnimator>()->SetHeight(2.0f);
				moveEffect->GetComponent<SpriteAnimator>()->SetLoop(false);

			}

			// 대쉬
			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				player->SetChild(effect);

				effect->SetName(TEXT("DashEffect"));
				effect->SetTag(TEXT("DashEffect"));
				effect->AddComponent<SpriteAnimator>()->Init("DashEffect");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("DashEffect", "Resources/Effect/PlayerDash.png", 1024, 512, 4, 2, 0.05f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(2.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(2.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(true);
				effect->SetGameObjectEnable(false);
			}

			// 검기강화버전 -> 여기에는 collider, monobehaviour 추가해야 할듯
			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				player->SetChild(effect);

				effect->SetName(TEXT("SkillUpgradeEffect"));
				effect->SetTag(TEXT("SkillUpgradeEffect"));
				effect->AddComponent<SpriteAnimator>()->Init("SkillUpgradeEffect");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("SkillUpgradeEffect", "Resources/Effect/SkillUpgradeEffect.png", 2048, 1024, 4, 2, 0.05f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(2.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(2.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(true);
				effect->SetGameObjectEnable(false);

				}


			// 강화 이펙트
			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				player->SetChild(effect);

				effect->SetName(TEXT("UgradeEffect"));
				effect->SetTag(TEXT("UgradeEffect"));
				effect->AddComponent<SpriteAnimator>()->Init("UgradeEffect");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("UgradeEffect", "Resources/Effect/UgradeEffect.png", 2048, 2048, 4, 3, 0.025f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(5.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(5.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(false);
				effect->GetComponent<SpriteAnimator>()->SetPosition(Vector(0.f, 0.f, 0.f));
			}

			// 죽음
			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				player->SetChild(effect);

				effect->SetName(TEXT("PlayerDeath"));
				effect->SetTag(TEXT("PlayerDeath"));
				effect->AddComponent<SpriteAnimator>()->Init("PlayerDeath");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("UgradeEffect", "Resources/Effect/PlayerDeath.png", 2048, 1024, 4, 2, 0.01f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(5.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(5.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(false);
				effect->GetComponent<SpriteAnimator>()->SetPosition(Vector(-5.f, 0.f, -5.f));

			}

			// attack1
			//{
			//	std::shared_ptr<GameObject> effect = scene->CreateEmpty();
			//	player->SetChild(effect);

			//	effect->SetName(TEXT("Attack1Effect"));
			//	effect->SetTag(TEXT("Attack1Effect"));
			//	effect->AddComponent<SpriteAnimator>()->Init("Attack1Effect");
			//	effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("DeathEffect", "Resources/Effect/nomal_attack1.png", 2048, 1024, 4, 2, 0.05f);
			//	effect->GetComponent<SpriteAnimator>()->SetWidth(2.0f);
			//	effect->GetComponent<SpriteAnimator>()->SetHeight(2.0f);
			//	effect->GetComponent<SpriteAnimator>()->SetLoop(true);
			//	effect->SetGameObjectEnable(false);
			//}

			//// attack2
			//{
			//	std::shared_ptr<GameObject> effect = scene->CreateEmpty();
			//	player->SetChild(effect);

			//	effect->SetName(TEXT("Attack1Effect"));
			//	effect->SetTag(TEXT("Attack1Effect"));
			//	effect->AddComponent<SpriteAnimator>()->Init("Attack1Effect");
			//	effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("DeathEffect", "Resources/Effect/nomal_attack2.png", 2048, 1024, 4, 2, 0.05f);
			//	effect->GetComponent<SpriteAnimator>()->SetWidth(2.0f);
			//	effect->GetComponent<SpriteAnimator>()->SetHeight(2.0f);
			//	effect->GetComponent<SpriteAnimator>()->SetLoop(true);
			//	effect->SetGameObjectEnable(false);
			//}

			//// attack3
			//{
			//	std::shared_ptr<GameObject> effect = scene->CreateEmpty();
			//->SetChild(effect);

			//	effect->SetName(TEXT("Attack1Effect"));
			//	effect->SetTag(TEXT("Attack1Effect"));
			//	effect->AddComponent<SpriteAnimator>()->Init("Attack1Effect");
			//	effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("DeathEffect", "Resources/Effect/nomal_attack3.png", 2048, 1024, 4, 2, 0.05f);
			//	effect->GetComponent<SpriteAnimator>()->SetWidth(2.0f);
			//	effect->GetComponent<SpriteAnimator>()->SetHeight(2.0f);
			//	effect->GetComponent<SpriteAnimator>()->SetLoop(true);
			//	effect->SetGameObjectEnable(false);
			//}


			GameState::GetInstance()->SetPlayer(player);

			//Debug
			{
				player->AddComponent<Debug::DebugComponent>();
			}

			return player;
		}

		std::shared_ptr<GameObject> CreateSwordAttackNormal(SceneBase* scene)
		{
			std::shared_ptr<GameObject> o = scene->CreateEmpty();

			o->SetName(TEXT("SkillEffect"));
			o->SetTag(TEXT("SkillEffect"));

			o->AddComponent<BoxCollider>()->SetSize(Vector(2.f, 2.f, 2.f));
			o->AddComponent<Player::SwordAttack>();

			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				o->SetChild(effect);
				
				effect->SetName(TEXT("SkillEffect"));
				effect->SetTag(TEXT("SkillEffect"));

				effect->AddComponent<SpriteAnimator>()->Init("SkillEffect");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("SkillEffect", "Resources/Effect/skill_normal.png", 2048, 1024, 4, 2, 0.025f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(4.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(4.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(false);
			}

			return o;
		}

		std::shared_ptr<GameObject> CreateCamera(SceneBase* scene)
		{
			std::shared_ptr<GameObject> camera = scene->CreateEmpty();
			scene->SetMainCamera(camera->AddComponent<Camera>());
			camera->AddComponent<Player::PlayerCamera>();
			return camera;
		}

		std::shared_ptr<GameObject> CreateAltar(SceneBase* scene)
		{
			std::shared_ptr<GameObject> altar = scene->CreateEmpty();
			altar->SetName(TEXT("Altar"));
			altar->SetTag(TEXT("Altar"));


			altar->AddComponent<Altar::AltarState>();
			altar->AddComponent<SphereCollider>()->SetRadius(2.f);
			altar->AddComponent<Altar::AltarMonoBehaviour>();

			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				altar->SetChild(effect);

				effect->SetName(TEXT("AltarEffectLoop"));
				effect->SetTag(TEXT("AltarEffectLoop"));
				effect->AddComponent<SpriteAnimator>()->Init("AltarEffectLoop");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("AltarEffectLoop", "Resources/Effect/altar.png", 2048, 2048, 4, 3, 0.025f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(6.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(6.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(true);
				effect->GetComponent<SpriteAnimator>()->SetPosition(Vector(0.f, 4.f, 0.f));
			}

			altar->AddComponent<SpriteAnimator>()->Init("AltarInteraction");
			altar->GetComponent<SpriteAnimator>()->SetSpriteTexture("AltarInteraction", "Resources/UI/pressFKey_pray.png",240, 178, 1, 1, 1.0f);
			altar->GetComponent<SpriteAnimator>()->SetWidth(8.f);
			altar->GetComponent<SpriteAnimator>()->SetHeight(6.f);
			altar->GetComponent<SpriteAnimator>()->SetLoop(true);
			altar->GetComponent<SpriteAnimator>()->SetPosition(Vector(28.f, 6.f, 0.f));
			altar->GetComponent<SpriteAnimator>()->SetIsEnable(false);
			
			
			altar->GetComponent<Transform>()->SetPosition({25.f, 0.f, 0.f}, Space::WORLD);
			return altar;
		}

		std::shared_ptr<GameObject> CreateWeaponAltar(SceneBase* scene)
		{
			std::shared_ptr<GameObject> altar = scene->CreateEmpty();
			altar->SetName(TEXT("WeaponAltar"));
			altar->SetTag(TEXT("WeaponAltar"));
			
			altar->AddComponent<Altar::WeaponAltarState>();
			altar->AddComponent<SphereCollider>()->SetRadius(2.f);
			altar->AddComponent<Altar::WeaponAltarMonoBehaviour>();

			{
				std::shared_ptr<GameObject> effect = scene->CreateEmpty();
				altar->SetChild(effect);

				effect->SetName(TEXT("WeaponAltarEffectLoop"));
				effect->SetTag(TEXT("WeaponAltarEffectLoop"));
				effect->AddComponent<SpriteAnimator>()->Init("WeaponAltarEffectLoop");
				effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("AltarEffectLoop", "Resources/Effect/altar.png", 2048, 2048, 4, 3, 0.025f);
				effect->GetComponent<SpriteAnimator>()->SetWidth(6.0f);
				effect->GetComponent<SpriteAnimator>()->SetHeight(6.0f);
				effect->GetComponent<SpriteAnimator>()->SetLoop(true);
				effect->GetComponent<SpriteAnimator>()->SetPosition(Vector(0.f, 4.f, 0.f));
			}

			altar->AddComponent<SpriteAnimator>()->Init("WeaponAltarInteraction");
			altar->GetComponent<SpriteAnimator>()->SetSpriteTexture("WeaponAltarInteraction", "Resources/UI/pressFKey_Enhance.png",
				240, 178, 1, 1, 1.0f);
			altar->GetComponent<SpriteAnimator>()->SetWidth(8.f);
			altar->GetComponent<SpriteAnimator>()->SetHeight(6.f);
			altar->GetComponent<SpriteAnimator>()->SetLoop(true);
			altar->GetComponent<SpriteAnimator>()->SetPosition(Vector(28.f, 6.f, 0.f));
			altar->GetComponent<SpriteAnimator>()->SetIsEnable(false);

			altar->GetComponent<Transform>()->SetPosition({ 25.f, 0.f, 0.f }, Space::WORLD);
			return altar;
		}

		std::shared_ptr<GameObject> CreateWeapon(SceneBase* scene)
		{
			std::shared_ptr<GameObject> weapon = scene->CreateEmpty();
			weapon->SetName(TEXT("PlayerWeapon"));
			weapon->SetTag(TEXT("PlayerWeapon"));

			weapon->AddComponent<Weapon::DefaultWeapon>();
			return weapon;
		}

		std::shared_ptr<GameObject> CreateTrap(SceneBase* scene, Vector pos)
		{
			scene->CreateEmpty()->SetName(TEXT("Trap"));
			return std::shared_ptr<GameObject>();
		}

		std::shared_ptr<GameObject> CreateRewardBox(SceneBase* scene, Vector pos, int index)
		{
			std::shared_ptr<GameObject> rw = scene->CreateEmpty();

			rw->SetName(TEXT("RewardBox"));
			rw->SetTag(TEXT("RewardBox"));

			rw->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
			rw->GetComponent<Transform>()->SetRotation(Vector(0.f,180.f,0.f), Space::WORLD);

			rw->AddComponent<BoxCollider>()->SetSize(Vector(5.f,5.f, 5.f));
			
			switch (index)
			{
			case 0:
			{
				rw->AddComponent<SkinnedMeshRenderer>()->Init("HPReward", "Resources/Model/HPTreasureBox_OpenAnimation.ase", RendererBase::ModelType::NONE_AO);
				rw->AddComponent<Animator>()->AddAnimation("Resources/Model/HPTreasureBox_OpenAnimation.ase", "HPRewardOpen");
				rw->GetComponent<Animator>()->Stop();
				rw->AddComponent<RewardBoxMonoBehaviour>()->SetType("HPRewardOpen");
				break;
			}
			case 1:
			{
				rw->AddComponent<SkinnedMeshRenderer>()->Init("MemoryReward", "Resources/Model/MemoryTreasureBox_OpenAnimation.ase", RendererBase::ModelType::NONE_AO);
				rw->AddComponent<Animator>()->AddAnimation("Resources/Model/MemoryTreasureBox_OpenAnimation.ase", "MemoryRewardOpen");
				rw->GetComponent<Animator>()->Stop();
				rw->AddComponent<RewardBoxMonoBehaviour>()->SetType("MemoryRewardOpen");
				break;
			}
			case 2:
			{
				rw->AddComponent<SkinnedMeshRenderer>()->Init("WeaponReward", "Resources/Model/WeaponTreasureBox_OpenAnimation.ase", RendererBase::ModelType::NONE_AO);
				rw->AddComponent<Animator>()->AddAnimation("Resources/Model/WeaponTreasureBox_OpenAnimation.ase", "WeaponRewardOpen");
				rw->GetComponent<Animator>()->Stop();
				rw->AddComponent<RewardBoxMonoBehaviour>()->SetType("WeaponRewardOpen");
				break;
			}
			}

			rw->GetComponent<RewardBoxMonoBehaviour>()->SetIndex(index);

			return rw;
		}

		std::shared_ptr<GameObject> CreateRewardWithIndex(SceneBase* scene, int index)
		{
			switch (index)
			{
			case 0:
			{
				return CreateHP(scene);
			}
			case 1:
			{
				return CreateMemoryGem(scene);
			}
			case 2:
			{
				return CreateMoney(scene);
			}
			default:
				return nullptr;
			}
		}

		std::shared_ptr<GameObject> CreateHP(SceneBase* scene, Vector pos)
		{
			std::shared_ptr<GameObject> hp = scene->CreateEmpty();
			hp->SetName(TEXT("Hp"));
			hp->SetTag(TEXT("Hp"));
			hp->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);

			hp->AddComponent<BoxCollider>()->SetSize(Vector(1.f, 2.f, 1.f));
			hp->AddComponent<IncreaseHp>();
			hp->AddComponent<MeshRenderer>()->Init("Wine", "Resources/Model/Wine.ase", RendererBase::ModelType::NONE_AO);

			hp->SetGameObjectEnable(false);
			return hp;
		}

		std::shared_ptr<GameObject> CreateMemoryGem(SceneBase* scene, Vector pos)
		{
			std::shared_ptr<GameObject> gem = scene->CreateEmpty();
			gem->SetName(TEXT("MemoryGem"));
			gem->SetTag(TEXT("MemoryGem"));
			gem->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
			
			gem->AddComponent<BoxCollider>()->SetSize(Vector(1.f, 2.f, 1.f));
			gem->AddComponent<MemoryGem>();
			gem->AddComponent<MeshRenderer>()->Init("MemoryGem", "Resources/Model/MemoryGem.ase", RendererBase::ModelType::NONE_AO);

			gem->SetGameObjectEnable(false);
			return gem;
		}

		std::shared_ptr<GameObject> CreateMoney(SceneBase* scene, Vector pos)
		{
			std::shared_ptr<GameObject> money = scene->CreateEmpty();
			money->SetName(TEXT("Money"));
			money->SetTag(TEXT("Money"));
			money->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);

			money->AddComponent<BoxCollider>()->SetSize(Vector(1.f, 2.f, 1.f));
			money->AddComponent<Money>();
			money->AddComponent<MeshRenderer>()->Init("WeaponCoin", "Resources/Model/Weapon_Coin_Box.ase", RendererBase::ModelType::NONE_AO);

			money->SetGameObjectEnable(false);

			return money;
		}

		std::shared_ptr<GameObject> CreateObstacle(SceneBase* scene, Vector pos,float width,float height)
		{
			std::shared_ptr<GameObject> obstacle = scene->CreateEmpty();
			obstacle->AddComponent<Obstacle::ObstacleState>()->Init(pos, width, height);
			obstacle->AddComponent<BoxCollider>()->SetSize(Vector(width, 5.f, height));
			obstacle->SetName(TEXT("Obstacle"));
			obstacle->SetTag(TEXT("Obstacle"));
			obstacle->AddComponent<MeshRenderer>()->Init("testObstacle","Resources/Model/Pot1.ase", RendererBase::ModelType::NONE_AO);

			obstacle->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);

			return obstacle;
		}

		std::shared_ptr<GameObject> CreateStaticMesh(SceneBase* scene, std::string name,std::string path)
		{
			std::shared_ptr<GameObject> mesh = scene->CreateEmpty();
			mesh->GetComponent<Transform>()->SetPosition(Vector::Zero, Space::WORLD);

			mesh->AddComponent<MeshRenderer>()->Init(name, path, RendererBase::ModelType::NONE_RMA);

			return mesh;
		}

		std::shared_ptr<GameObject> CreateMonster(SceneBase* scene, Vector pos)
		{
			std::shared_ptr<GameObject> monster = scene->CreateEmpty();
			monster->SetName(TEXT("Monster"));
			monster->SetTag(TEXT("Monster"));

			monster->AddComponent<Monster::MonsterState>()->monsterHp = 55.f;

			monster->AddComponent<Monster::MonsterMonoBehaviour>();
			monster->AddComponent<SphereCollider>();
			monster->AddComponent<RayCollider>();

			monster->AddComponent<SkinnedMeshRenderer>()->Init("Skeleton", "Resources/Model/Skeleton.ase", RendererBase::ModelType::ALBEDO_ONLY);
			monster->GetComponent<SkinnedMeshRenderer>()->SetMetallic(0.0f);
			monster->GetComponent<SkinnedMeshRenderer>()->SetRoughness(0.7f);
			
			monster->AddComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_idle.ase","MonsterIdle", 1.0f, true);
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_run.ase", "MonsterRun", 1.0f, true);
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_attack.ase", "MonsterAttack");
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_hit.ase", "MonsterHit");
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_death.ase", "MonsterDeath");

			monster->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
			monster->GetComponent<Transform>()->SetScale(Vector(0.05f,0.05f,0.05f));

			{
				std::shared_ptr<GameObject> monsterAttack = scene->CreateEmpty();
				monster->SetChild(monsterAttack);

				monsterAttack->SetName(TEXT("MonsterAttack"));
				monsterAttack->SetTag(TEXT("MonsterAttack"));

				monsterAttack->AddComponent<BoxCollider>()->SetSize(Vector(2.f, 5.f, 3.f));
				monsterAttack->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 90.f));

				monsterAttack->AddComponent<Monster::MonsterAttack>();
			}

			{
				std::shared_ptr<GameObject> direction = scene->CreateEmpty();
				monster->SetChild(direction);

				direction->SetName(TEXT("Direction"));
				direction->SetTag(TEXT("Direction"));

				direction->AddComponent<BoxCollider>()->SetSize(Vector(1.f, 1.f, 1.f));

			}

			// 피격 이펙트
			//{
			//	std::shared_ptr<GameObject> hitEffect = scene->CreateEmpty();
			//	monster->SetChild(hitEffect);

			//	hitEffect->SetName(TEXT("MonsterHitEffect"));
			//	hitEffect->SetTag(TEXT("MonsterHitEffect"));

			//	hitEffect->AddComponent<SpriteAnimator>()->Init("MonsterHitEffect");
			//	hitEffect->GetComponent<SpriteAnimator>()->SetSpriteTexture("MonsterHitEffect", "Resources/Effect/Smoke.png", 2048, 1024, 4, 2, 0.05f);
			//	hitEffect->GetComponent<SpriteAnimator>()->SetPosition(Vector(0.f, 5.f, 0.f));
			//	hitEffect->GetComponent<SpriteAnimator>()->SetWidth(5.0f);
			//	hitEffect->GetComponent<SpriteAnimator>()->SetHeight(5.0f);
			//	hitEffect->GetComponent<SpriteAnimator>()->SetLoop(false);
			//}

			return monster;
		}

		std::shared_ptr<GameObject> CreateBigMonster(SceneBase* scene, Vector pos)
		{
			std::shared_ptr<GameObject> monster = scene->CreateEmpty();
			monster->SetName(TEXT("BigMonster"));
			monster->SetTag(TEXT("BigMonster"));

			// 여기서 몬스터 속성 변경

			monster->AddComponent<Monster::MonsterState>()->attackPower = 20.f;
			monster->GetComponent<Monster::MonsterState>()->monsterHp = 85.f;
			monster->GetComponent<Monster::MonsterState>()->detectRange = 15.f;
			monster->GetComponent<Monster::MonsterState>()->attackRange = 5.f;
			monster->GetComponent<Monster::MonsterState>()->hitSound = "MonsterAttacked3";

			monster->AddComponent<Monster::MonsterMonoBehaviour>();
			monster->AddComponent<SphereCollider>();
			monster->AddComponent<RayCollider>();

			monster->AddComponent<SkinnedMeshRenderer>()->Init("Skeleton", "Resources/Model/Skeleton.ase", RendererBase::ModelType::ALBEDO_ONLY);
			monster->GetComponent<SkinnedMeshRenderer>()->SetMetallic(0.0f);
			monster->GetComponent<SkinnedMeshRenderer>()->SetRoughness(0.7f);

			monster->AddComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_idle.ase", "MonsterIdle", 1.0f, true);
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_run.ase", "MonsterRun", 1.0f, true);
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_attack.ase", "MonsterAttack");
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_hit.ase", "MonsterHit");
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Skeleton_death.ase", "MonsterDeath");

			monster->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
			monster->GetComponent<Transform>()->SetScale(Vector(0.1f, 0.1f, 0.1f));

			{
				std::shared_ptr<GameObject> monsterAttack = scene->CreateEmpty();
				monster->SetChild(monsterAttack);

				monsterAttack->SetName(TEXT("MonsterAttack"));
				monsterAttack->SetTag(TEXT("MonsterAttack"));

				monsterAttack->AddComponent<BoxCollider>()->SetSize(Vector(4.f, 5.f, 4.f));
				monsterAttack->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 45.f));

				monsterAttack->AddComponent<Monster::MonsterAttack>();
			}

			{
				std::shared_ptr<GameObject> direction = scene->CreateEmpty();
				monster->SetChild(direction);

				direction->SetName(TEXT("Direction"));
				direction->SetTag(TEXT("Direction"));

				direction->AddComponent<BoxCollider>()->SetSize(Vector(1.f, 1.f, 1.f));

			}

			//{
			//	std::shared_ptr<GameObject> hitEffect = scene->CreateEmpty();
			//	monster->SetChild(hitEffect);

			//	hitEffect->SetName(TEXT("MonsterHitEffect"));
			//	hitEffect->SetTag(TEXT("MonsterHitEffect"));

			//	hitEffect->AddComponent<SpriteAnimator>()->Init("MonsterHitEffect");
			//	hitEffect->GetComponent<SpriteAnimator>()->SetSpriteTexture("MonsterHitEffect", "Resources/Effect/Smoke.png", 2048, 1024, 4, 2, 0.05f);
			//	hitEffect->GetComponent<SpriteAnimator>()->SetPosition(Vector(0.f, 5.f, 0.f));
			//	hitEffect->GetComponent<SpriteAnimator>()->SetWidth(7.0f);
			//	hitEffect->GetComponent<SpriteAnimator>()->SetHeight(7.0f);
			//	hitEffect->GetComponent<SpriteAnimator>()->SetLoop(false);
			//}

			return monster;
		}

		std::shared_ptr<GameObject> CreateLongRangeMonster(SceneBase* scene, Vector pos)
		{
			std::shared_ptr<GameObject> monster = scene->CreateEmpty();
			monster->SetName(TEXT("Monster"));
			monster->SetTag(TEXT("Monster"));

			monster->AddComponent<LongRangeMonster::LongRangeMonsterState>();
			monster->AddComponent<Monster::MonsterMonoBehaviour>();
			monster->AddComponent<SphereCollider>();
			monster->AddComponent<RayCollider>();

			monster->AddComponent<SkinnedMeshRenderer>()->Init("Ely", "Resources/Model/Ely_idle.ase", RendererBase::ModelType::NONE_RMA);
			monster->AddComponent<Animator>()->AddAnimation("Resources/Model/Ely_idle.ase", "Ely_Idle");
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Ely_run.ase", "Ely_Run");
			monster->GetComponent<Animator>()->AddAnimation("Resources/Model/Ely_attack.ase", "Ely_Attack");

			monster->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
			monster->GetComponent<Transform>()->SetScale(Vector(0.01f, 0.01f, 0.01f));

			{
				for (int i = 0; i < 4; i++)
				{
					std::shared_ptr<GameObject> bullet = scene->CreateEmpty();
					bullet->SetName(TEXT("Bullet"));
					bullet->SetTag(TEXT("Bullet"));

					monster->GetComponent<LongRangeMonster::LongRangeMonsterState>()->AddBullet(bullet);
				}
				//탄환 생성해서 LongRangeMonsterState의 탄환 벡터에 넣어주자
			}

			return monster;
		}

		std::shared_ptr<GameObject> CreateSceneController(SceneBase* scene)
		{
			std::shared_ptr<GameObject> s = scene->CreateEmpty();
			s->AddComponent<SceneController>();
			return s;
		}

		std::shared_ptr<GameObject> CreateBoss(SceneBase* scene)
		{
			std::shared_ptr<GameObject> boss = scene->CreateEmpty();
			boss->SetName(TEXT("Boss"));
			boss->SetTag(TEXT("Boss"));

			boss->AddComponent<Boss::BossTree>();
			boss->AddComponent<Boss::BossMonoBehaviour>();
			boss->AddComponent<BoxCollider>()->SetSize(Vector(12.f,12.f, 12.f));

			boss->AddComponent<SkinnedMeshRenderer>()->Init("Boss","Resources/Model/Boss.ase", RendererBase::ModelType::ALL);

			boss->AddComponent<Animator>()->AddAnimation("Resources/Model/Boss_idle.ase", "BossStart"); // 애니메이션?

			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_idle.ase", "BossIdle",1.4f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_run.ase", "BossRun");
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_death.ase", "BossDeath");

			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_stun.ase", "BossStun",1.4f,false);

			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_count.ase", "BossCounterAttack",1.25f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_attack1.ase","BossAttack1",1.1f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_attack2.ase", "BossAttack2",1.3f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_attack3.ase", "BossAttack3",1.2f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_jump1.ase", "BossCloseJump",1.5f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_jump2.ase", "BossLongJump",2.f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_doljin_start.ase", "BossDashStart",1.5f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_doljin_run.ase", "BossDash",1.5f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_badak.ase", "BossShockWave",4.f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_tanmak.ase", "BossBarrage",1.5f,false);
			boss->GetComponent<Animator>()->AddAnimation("Resources/Model/Boss_pohyo.ase", "BossRoar");


			boss->GetComponent<Transform>()->SetScale(Vector(0.035f, 0.035f, 0.035f));
			boss->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 0.f), Space::WORLD);

			{
				std::shared_ptr<GameObject> bossAttack = scene->CreateEmpty();
				bossAttack->SetName(TEXT("BossAttack"));
				bossAttack->SetTag(TEXT("BossAttack"));
				
				boss->SetChild(bossAttack);

				bossAttack->AddComponent<BoxCollider>()->SetSize(Vector(16.f,6.f,16.f));
				bossAttack->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 20.f), Space::LOCAL);
				bossAttack->AddComponent<Boss::BossAttackMonoBehaviour>();

				{
					std::shared_ptr<GameObject> effect = scene->CreateEmpty(); //BossShockWave.png
					bossAttack->SetChild(effect);

					effect->SetName(TEXT("BossShockWave"));
					effect->SetTag(TEXT("BossShockWave"));
					effect->AddComponent<SpriteAnimator>()->Init("BossShockWave");
					effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("BossShockWave", "Resources/Effect/BossShockWave.png", 2048, 2048, 4, 4, 0.05f);
					effect->GetComponent<SpriteAnimator>()->SetWidth(60.f);
					effect->GetComponent<SpriteAnimator>()->SetHeight(60.f);
					effect->GetComponent<SpriteAnimator>()->SetLoop(false);
					effect->GetComponent<SpriteAnimator>()->SetPosition(Vector(-1.f, 3.f, -1.f));
				}

				//충격파용 스프라이트 추가
			}

			// 근거리 공격용 콜라이더
			{
				std::shared_ptr<GameObject> bossCloseAttack = scene->CreateEmpty();
				bossCloseAttack->SetName(TEXT("BossCloseAttack"));
				bossCloseAttack->SetTag(TEXT("BossCloseAttack"));

				boss->SetChild(bossCloseAttack);

				bossCloseAttack->AddComponent<BoxCollider>()->SetSize(Vector(13.f, 20.f, 10.f));
				bossCloseAttack->GetComponent<Transform>()->SetPosition(Vector(0.f, 0.f, 160.f),Space::LOCAL);
				bossCloseAttack->AddComponent<Boss::BossAttackMonoBehaviour>()->attackDamage = 5.f;
			}

			// bullets
			{
				for (int i = 0; i <= 10; i++)
				{
					std::shared_ptr<GameObject> bullet = scene->CreateEmpty();
					bullet->SetName(TEXT("Bullet"));
					bullet->SetTag(TEXT("Bullet"));
					bullet->AddComponent<Boss::BossBullet>();
					bullet->AddComponent<BoxCollider>()->SetSize(Vector(5.f, 5.f, 5.f));
					
					std::shared_ptr<GameObject> effect = scene->CreateEmpty();
					bullet->SetChild(effect);

					std::string temp = "BarrageEffect";
					temp.append(std::to_string(i));

					effect->SetName(TEXT("BarrageEffect"));
					effect->SetTag(TEXT("BarrageEffect"));
					effect->AddComponent<SpriteAnimator>()->Init(temp);
					effect->GetComponent<SpriteAnimator>()->SetSpriteTexture("Bullet", "Resources/Effect/BossBullet.png", 2048, 1024, 4, 2, 0.05f);
					effect->GetComponent<SpriteAnimator>()->SetWidth(10.f);
					effect->GetComponent<SpriteAnimator>()->SetHeight(10.f);
					effect->GetComponent<SpriteAnimator>()->SetLoop(false);
					effect->GetComponent<SpriteAnimator>()->SetPosition(Vector(-1.f, 0.f, -1.f));
					
					boss->GetComponent<Boss::BossTree>()->AddBullet(bullet);
				}
			}




			GameState::GetInstance()->SetBoss(boss);

			return boss;
		}

		std::shared_ptr<GameObject> CreateEntrance(SceneBase* scene, tstring name, Vector pos)
		{
			std::shared_ptr<GameObject> entrance = scene->CreateEmpty();

			entrance->GetComponent<Transform>()->SetPosition(pos+Vector(5.f,0.f,5.f), Space::WORLD);
			entrance->GetComponent<Transform>()->SetScale(Vector(1.2f, 1.2f, 1.2f));

			entrance->SetName(name);
			entrance->SetTag(name);

			entrance->AddComponent<SkinnedMeshRenderer>()->Init("Door1", "Resources/Model/Door_OpenAnimation.ase", RendererBase::ModelType::NONE_AO);
			entrance->AddComponent<Animator>()->AddAnimation("Resources/Model/Door_OpenAnimation.ase", "DoorOpen", 1.0f, false);
			entrance->GetComponent<Animator>()->Stop();

			entrance->AddComponent<BoxCollider>()->SetSize(Vector(10.f, 10.f, 2.f));
			

			entrance->AddComponent<Entrance>();

			return entrance;
		}

		std::shared_ptr<GameObject> CreateEntranceWithIndex(SceneBase* scene, Vector pos,int index)
		{
			std::shared_ptr<GameObject> entrance = scene->CreateEmpty();

			entrance->GetComponent<Transform>()->SetPosition(pos + Vector(5.f, 0.f, 5.f), Space::WORLD);
			entrance->GetComponent<Transform>()->SetScale(Vector(1.2f, 1.2f, 1.2f));

			switch (index)
			{
				case 0:
				{
					entrance->SetName(TEXT("Entrance0"));
					entrance->SetTag(TEXT("Entrance"));
					entrance->AddComponent<SkinnedMeshRenderer>()->Init("Door0", "Resources/Model/Door_OpenAnimation0.ase", RendererBase::ModelType::NONE_AO);
					entrance->AddComponent<Animator>()->AddAnimation("Resources/Model/Door_OpenAnimation0.ase", "DoorOpen", 1.0f, false);
					entrance->GetComponent<Animator>()->Stop();
					break;
				}
				case 1:
				{
					entrance->SetName(TEXT("Entrance1"));
					entrance->SetTag(TEXT("Entrance"));
					entrance->AddComponent<SkinnedMeshRenderer>()->Init("Door1", "Resources/Model/Door_OpenAnimation1.ase", RendererBase::ModelType::NONE_AO);
					entrance->AddComponent<Animator>()->AddAnimation("Resources/Model/Door_OpenAnimation1.ase", "DoorOpen", 1.0f, false);
					entrance->GetComponent<Animator>()->Stop();
					break;
				}
				case 2:
				{
					entrance->SetName(TEXT("Entrance2"));
					entrance->SetTag(TEXT("Entrance"));
					entrance->AddComponent<SkinnedMeshRenderer>()->Init("Door2", "Resources/Model/Door_OpenAnimation2.ase", RendererBase::ModelType::NONE_AO);
					entrance->AddComponent<Animator>()->AddAnimation("Resources/Model/Door_OpenAnimation2.ase", "DoorOpen", 1.0f, false);
					entrance->GetComponent<Animator>()->Stop();
					break;
				}
				case 3:
				{
					entrance->SetName(TEXT("Entrance3"));
					entrance->SetTag(TEXT("Entrance"));
					entrance->AddComponent<SkinnedMeshRenderer>()->Init("Door3", "Resources/Model/Door_OpenAnimation3.ase", RendererBase::ModelType::NONE_AO);
					entrance->AddComponent<Animator>()->AddAnimation("Resources/Model/Door_OpenAnimation3.ase", "DoorOpen", 1.0f, false);
					entrance->GetComponent<Animator>()->Stop();
					break;
				}
				case 4:
				{
					entrance->SetName(TEXT("EntranceBoss"));
					entrance->SetTag(TEXT("Entrance"));
					entrance->AddComponent<SkinnedMeshRenderer>()->Init("BossDoor", "Resources/Model/BossDoor_OpenAnimation.ase", RendererBase::ModelType::NONE_AO);
					entrance->AddComponent<Animator>()->AddAnimation("Resources/Model/BossDoor_OpenAnimation.ase", "DoorOpen", 1.0f, false);
					entrance->GetComponent<Animator>()->Stop();
					break;
				}

			}
			entrance->AddComponent<BoxCollider>()->SetSize(Vector(6.f, 10.f, 2.f));
			entrance->AddComponent<Entrance>();

			return entrance;
		}


		std::shared_ptr<GameObject> CreateWall(SceneBase* scene, Vector pos, float width, float height)
		{
			std::shared_ptr<GameObject> wall = scene->CreateEmpty();
			wall->GetComponent<Transform>()->SetPosition(pos,Space::WORLD);

			wall->SetName(TEXT("Wall"));
			wall->SetTag(TEXT("Obstacle"));

			wall->AddComponent<Obstacle::ObstacleState>()->Init(pos, width, height);
			wall->AddComponent<BoxCollider>()->SetSize(Vector(width, 5.f, height));
			return wall;
		}
	}
}
