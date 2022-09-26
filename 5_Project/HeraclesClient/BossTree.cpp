#include "pch.h"
#include "BossTree.h"

#include "BossAttackMonoBehaviour.h"
#include "BossMonoBehaviour.h"
#include "BossBullet.h"
#include "Animator.h"
#include "SpriteAnimator.h"
#include "Timer.h"
#include "GameState.h"

#include "UIManager.h"

#include "SoundManager.h"
#include "SceneManager.h"

namespace Heracles
{
	namespace Boss
	{
		bool Idle::afterAttack = false;
		bool DashAttackStart::dashAttackRunning = false;

		static Vector VecotrMultFloat(Vector v, float f)
		{
			return Vector
			{
				v.x * f,
				v.y * f,
				v.z * f,
				v.w * f
			};
		}

		BossTree::BossTree(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj)
		{
		}

		void BossTree::ActivateBullet(int index)
		{
			auto t = this->GetGameObject()->GetComponent<Transform>();
			auto p = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>();
			auto bullet = this->bullets[index];
			bullet->GetComponent<BossBullet>()->SetSpeed(25.f);
			bullet->GetComponent<BossBullet>()->SetInitialPos(t->GetWorldPosition()+Vector(0.f,10.f,0.f));
			bullet->GetComponent<BossBullet>()->SetDirection(Vector3Normalize(p->GetWorldPosition() - (t->GetWorldPosition()+ Vector(0.f, 10.f, 0.f))));
			bullet->GetComponent<BossBullet>()->Activate();
			bullet->GetChild(TEXT("BarrageEffect"))->GetComponent<SpriteAnimator>()->SetLoop(true);
		}
		
		void BossTree::DeactivateBulletAll()
		{
			for (auto b : this->bullets)
			{
				b->GetChild(TEXT("BarrageEffect"))->GetComponent<SpriteAnimator>()->SetLoop(false);
			}
		}

		void BossTree::GetDamage(float damage)
		{
			this->hp -= damage;
			this->hitCount += 1;
			SoundManager::GetInstance()->PlayEffect("MonsterAttacked2");

			SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(50.0f);
		}

		void BossTree::Awake() // 100 320
		{

		}

		void BossTree::Start()
		{
			this->root = std::make_shared<Selector>();
			this->root->AddChild(std::make_shared<BossStart>());
			this->root->AddChild(this->PhaseSelector());
			SoundManager::GetInstance()->PlayEffect("BossEnter");
		}

		void BossTree::PreUpdate(float tick)
		{
			//debug
			{
				Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
				Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
				std::string dist = "Distance-Player: ";
				dist.append(std::to_string(Vector3Length(player - boss)));

				UIManager::GetInstance()->SetTextUIText("Distance-Player", dist);

				std::string bossHP = "BossHp : ";
				bossHP.append(std::to_string(this->hp));
				UIManager::GetInstance()->SetTextUIText("BossHp", bossHP);
			}

			if (this->onStart)
			{
				return;
			}

			SoundManager::GetInstance()->PlayBossWalkSound("BossWalk", true);
			GameState::GetInstance()->bossHp = this->hp;
		}

		void BossTree::Update(float tick)
		{
			if (this->onStart)
			{
				return;
			}
			this->root->Run();
		}

		void BossTree::LateUpdate(float tick)
		{
			if (this->onStart)
			{
				return;
			}

			this->getDamage = false;
		}

		std::shared_ptr<Node> BossTree::TestPhase()
		{
			std::shared_ptr<Selector> phaseSelector = std::make_shared<Selector>();

			phaseSelector->AddChild(BossTree::MakeSequenceWithInstance<Sequencer, Phase1>(this->TestPhase1Module()));
			phaseSelector->AddChild(BossTree::MakeSequenceWithInstance<Sequencer, Phase2>(this->TestPhase2Module()));
			phaseSelector->AddChild(BossTree::MakeSequenceWithInstance<Sequencer, Phase3>(this->TestPhase2Module()));

			return phaseSelector;
		}

		std::shared_ptr<Node> BossTree::TestPhase1Module()
		{
			std::shared_ptr<Selector> actionSelector = std::make_shared<Selector>();

			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckDeath, Death>());
			actionSelector->AddChild(this->StunSequence());
			actionSelector->AddChild(this->TestAttack1());
			actionSelector->AddChild(this->MoveSequence());

			return actionSelector;
		}

		std::shared_ptr<Node> BossTree::TestPhase2Module()
		{
			std::shared_ptr<Selector> actionSelector = std::make_shared<Selector>();

			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckDeath, Death>());
			actionSelector->AddChild(this->StunSequence());
			actionSelector->AddChild(this->TestAttack2());
			actionSelector->AddChild(this->MoveSequence());

			return actionSelector;
		}

		std::shared_ptr<Node> BossTree::TestAttack1()
		{
			std::shared_ptr<Selector> attackSelector = std::make_shared<Selector>();

			attackSelector->AddChild(std::make_shared<Idle>());

			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, JumpRange, LongJumpAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			attackSelector->AddChild((BossTree::MakeSequence<Sequencer, CloseRange, CloseAttack>()));
			attackSelector->AddChild(std::make_shared<Idle>());


			return attackSelector;
		}

		std::shared_ptr<Node> BossTree::TestAttack2()
		{
			std::shared_ptr<Selector> attackSelector = std::make_shared<Selector>();

			attackSelector->AddChild(std::make_shared<Idle>());

			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, DashRange, LongJumpAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, JumpRange, LongJumpAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			attackSelector->AddChild((BossTree::MakeSequence<Sequencer, CloseRange, CloseAttack>()));
			attackSelector->AddChild(std::make_shared<Idle>());

			return attackSelector;
		}

		std::shared_ptr<Node> BossTree::PhaseSelector()
		{
			std::shared_ptr<Selector> phaseSelector = std::make_shared<Selector>();

			phaseSelector->AddChild(BossTree::MakeSequenceWithInstance<Sequencer, Phase1>(this->Phase1Module()));
			phaseSelector->AddChild(BossTree::MakeSequenceWithInstance<Sequencer, Phase2>(this->Phase2Module()));
			phaseSelector->AddChild(BossTree::MakeSequenceWithInstance<Sequencer, Phase3>(this->Phase3Module()));

			return phaseSelector;
		}

		std::shared_ptr<Node> BossTree::Phase1Module()
		{
			std::shared_ptr<Selector> actionSelector = std::make_shared<Selector>();

			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckDeath, Death>());
			actionSelector->AddChild(this->StunSequence());
			actionSelector->AddChild(this->Phase1AttackSequence());
			actionSelector->AddChild(this->MoveSequence());

			return actionSelector;
		}

		std::shared_ptr<Node> BossTree::Phase1AttackSequence()
		{
			std::shared_ptr<Selector> attackSelector = std::make_shared<Selector>();

			attackSelector->AddChild(std::make_shared<Idle>());

			//카운터 시퀸스
			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckHitCount, CounterAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 돌진
			attackSelector->AddChild(BossTree::MakeContinuousAction<Sequencer, DashRange, DashAttackStart, DashAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 점프
			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, JumpRange, LongJumpAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 근접공격 시퀸스
			{
				std::shared_ptr<Sequencer> closeAttack = std::make_shared<Sequencer>();

				std::shared_ptr<RandomSelector> closeAttackSelector = std::make_shared<RandomSelector>(10, 2);
				closeAttackSelector->AddChild(std::make_shared<CloseAttack>());
				closeAttackSelector->AddChild(std::make_shared<CloseJumpAttack>());

				closeAttack->AddChild(std::make_shared<CloseRange>(closeAttackSelector));
				closeAttack->AddChild(closeAttackSelector);

				attackSelector->AddChild(closeAttack);
			}
			attackSelector->AddChild(std::make_shared<Idle>());

			return attackSelector;
		}

		std::shared_ptr<Node> BossTree::Phase2Module()
		{
			std::shared_ptr<Selector> actionSelector = std::make_shared<Selector>();

			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckDeath, Death>());
			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, EnterPhase2, Phase2Roar>());
			actionSelector->AddChild(this->StunSequence());
			actionSelector->AddChild(this->Phase2AttackSequence());
			actionSelector->AddChild(this->MoveSequence());

			return actionSelector;
		}

		std::shared_ptr<Node> BossTree::Phase2AttackSequence()
		{
			std::shared_ptr<Selector> attackSelector = std::make_shared<Selector>();

			attackSelector->AddChild(std::make_shared<Idle>());

			//카운터 시퀸스
			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckHitCount, CounterAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 점프
			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, JumpRange, LongJumpAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 돌진
			attackSelector->AddChild(BossTree::MakeContinuousAction<Sequencer, DashRange, DashAttackStart, DashAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 근접공격 시퀸스
			{
				std::shared_ptr<Sequencer> closeAttack = std::make_shared<Sequencer>();

				std::shared_ptr<RandomSelector> closeAttackSelector = std::make_shared<RandomSelector>(10, 2);
				closeAttackSelector->AddChild(std::make_shared<CloseAttack>());
				closeAttackSelector->AddChild(std::make_shared<ShockWave>());
				closeAttackSelector->AddChild(std::make_shared<CloseJumpAttack>());

				closeAttack->AddChild(std::make_shared<CloseRange>(closeAttackSelector));
				closeAttack->AddChild(closeAttackSelector);

				attackSelector->AddChild(closeAttack);
			}
			attackSelector->AddChild(std::make_shared<Idle>());

			// 점프
			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, JumpRange, LongJumpAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			return attackSelector;
		}

		std::shared_ptr<Node> BossTree::Phase3Module()
		{

			std::shared_ptr<Selector> actionSelector = std::make_shared<Selector>();


			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckDeath, Death>());
			actionSelector->AddChild(BossTree::MakeSequence<Sequencer, EnterPhase3, Phase3Roar>());
			actionSelector->AddChild(this->StunSequence());
			actionSelector->AddChild(this->Phase3AttackSequence());
			actionSelector->AddChild(this->MoveSequence());

			return actionSelector;
		}

		std::shared_ptr<Node> BossTree::Phase3AttackSequence()
		{
			std::shared_ptr<Selector> attackSelector = std::make_shared<Selector>();

			attackSelector->AddChild(std::make_shared<Idle>());

			//카운터 시퀸스
			attackSelector->AddChild(BossTree::MakeSequence<Sequencer, CheckHitCount, CounterAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 돌진
			attackSelector->AddChild(BossTree::MakeContinuousAction<Sequencer, DashRange, DashAttackStart, DashAttack>());
			attackSelector->AddChild(std::make_shared<Idle>());

			// 원거리 시퀸스
			{
				std::shared_ptr<Sequencer> longAttack = std::make_shared<Sequencer>();

				std::shared_ptr<RandomSelector> longAttackSelector = std::make_shared<RandomSelector>(10, 3);
				longAttackSelector->AddChild(std::make_shared<Barrage>());
				longAttackSelector->AddChild(std::make_shared<LongJumpAttack>());

				longAttack->AddChild(std::make_shared<JumpRange>(longAttackSelector));
				longAttack->AddChild(longAttackSelector);

				attackSelector->AddChild(longAttack);
			}
			attackSelector->AddChild(std::make_shared<Idle>());

			// 근접공격 시퀸스
			{
				std::shared_ptr<Sequencer> closeAttack = std::make_shared<Sequencer>();

				std::shared_ptr<RandomSelector> closeAttackSelector = std::make_shared<RandomSelector>(10, 2);
				closeAttackSelector->AddChild(std::make_shared<CloseAttack>());
				closeAttackSelector->AddChild(std::make_shared<ShockWave>());
				closeAttackSelector->AddChild(std::make_shared<CloseJumpAttack>());

				closeAttack->AddChild(std::make_shared<CloseRange>(closeAttackSelector));
				closeAttack->AddChild(closeAttackSelector);

				attackSelector->AddChild(closeAttack);
			}
			attackSelector->AddChild(std::make_shared<Idle>());

			return attackSelector;
		}

		std::shared_ptr<Node> BossTree::StunSequence()
		{
			std::shared_ptr<Sequencer> stunSequencer = std::make_shared<Sequencer>();
			std::shared_ptr<Action> stun = std::make_shared<Stun>();
			stunSequencer->AddChild(std::make_shared<StunCondition>(stun));
			stunSequencer->AddChild(stun);

			return stunSequencer;
		}

		std::shared_ptr<Node> BossTree::MoveSequence()
		{
			std::shared_ptr<Sequencer> move = BossTree::MakeSequence<Sequencer, MoveRange, Move>();
			return move;
		}

		bool BossStart::Run()
		{
			static bool executed = false;

			if (!executed)
			{
				if (this->running)
				{
					if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
					{
						static float time = 0.0f;

						time += Timer::GetInstance()->DeltaTime();

						if (time >= 0.1f)
						{
							SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(300.0f);

							time -= 0.1f;
						}

						return true;
					}
					else
					{
						executed = true;
						this->running = false;
						return false;
					}
				}
				else
				{
					this->running = true;
					GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossRoar");
					SoundManager::GetInstance()->PlayEffect("BossStartRoar");
				}
			}
			else
			{
				return false;
			}
		}

		bool Phase1::Run()
		{
			if (GameState::GetInstance()->bossHp >= 700.f)
			{
				return true;
			}
			return false;
		}
		bool Phase2::Run()
		{
			auto hp = GameState::GetInstance()->bossHp;
			if (hp < 700.f && hp>300.f)
			{
				return true;
			}
			return false;
		}
		bool Phase3::Run()
		{
			auto hp = GameState::GetInstance()->bossHp;
			if (hp <= 300.f)
			{
				return true;
			}
			return false;
		}

		bool CloseAttack::Run()
		{
			static int attack = 0;

			static bool firstAttack = false;
			static bool secondAttack = false;
			static bool thirdAttack = false;

			if (this->running)
			{
				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (attack == 2)
					{
						if (this->progressTime > 1.6f && !firstAttack)
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
							SoundManager::GetInstance()->PlayEffect("BossAttack1");
							firstAttack = true;

						}
						else if (this->progressTime > 3.f && !secondAttack)
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
							SoundManager::GetInstance()->PlayEffect("BossAttack2");
							secondAttack = true;
						}
						else if (this->progressTime > 4.1f && !thirdAttack)
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
							SoundManager::GetInstance()->PlayEffect("BossAttack3");
							thirdAttack = true;
						}
						else
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
						}
					}
					else if (attack == 1)
					{
						if (this->progressTime > 1.6f && !firstAttack)
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
							SoundManager::GetInstance()->PlayEffect("BossAttack1");
							firstAttack = true;
						}
						else if (this->progressTime > 2.65f && !secondAttack)
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
							SoundManager::GetInstance()->PlayEffect("BossAttack2");
							secondAttack = true;
						}
						else
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
						}
					}
					else
					{
						if (!this->attacked && this->progressTime > 0.65f)
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
							SoundManager::GetInstance()->PlayEffect("BossAttack1");
							this->attacked = true;
						}
						else
						{
							GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
						}
					}
					return true;
				}
				else
				{
					this->running = false;
					this->attacked = false;
					this->progressTime = 0.f;

					firstAttack = false;
					secondAttack = false;
					thirdAttack = false;

					Idle::afterAttack = true;
					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;

				Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
				Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
				if (Vector3Length(player - boss) > 0.01f)
				{
					GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(player);
				}

				GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dist(0, 100);

				attack = dist(gen) % 3;

				switch (attack)
				{
					case 0:
					{
						GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossAttack1");
						break;
					}
					case 1:
					{
						GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossAttack2");
						break;
					}
					case 2:
					{
						GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossAttack3");
						break;
					}
				}

				return true;
			}
		}

		bool CloseJumpAttack::Run()
		{
			if (this->running)
			{
				//Debug
				{
					std::string a = "BossState : CloseJumpAttack";
					UIManager::GetInstance()->SetTextUIText("BossState", a);
				}

				if (this->attackTime - this->progressTime > 0.01f && GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->ParabolicMove();
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (!this->attacked && this->progressTime > 1.35f)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
						this->attacked = true;
					}
					else
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					}
					return true;
				}
				else
				{
					this->running = false;
					this->attacked = false;
					Idle::afterAttack = true;

					this->initialPos = Vector::Zero;
					this->targetPos = Vector::Zero;

					Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
					Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

					boss.y = 0.f;
					GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->SetPosition(boss, Space::WORLD);
					GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(player);

					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					SoundManager::GetInstance()->PlayEffect("BossJumpLand");
					//SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(100.f);

					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;

				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossLongJump");

				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
				Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
				Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

				this->initialPos = boss;
				this->targetPos = player;

				this->dx = (player.x - boss.x) / this->attackTime;
				this->dz = (player.z - boss.z) / this->attackTime;

				GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;

				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossCloseJump");

				SoundManager::GetInstance()->PlayEffect("BossJumpStart");


				return true;
			}
		}

		void CloseJumpAttack::ParabolicMove()
		{
			Vector dir = Vector::Zero;
			dir.x = this->dx * this->progressTime;
			dir.z = this->dz * this->progressTime;
			dir.y = -20.f / this->attackTime / this->attackTime * pow(this->attackTime / 2.f - this->progressTime, 2) + 5.f;

			auto boss = GameState::GetInstance()->GetBoss();
			boss->GetComponent<Transform>()->SetPosition(this->initialPos + dir, Space::WORLD);
		}

		bool LongJumpAttack::Run()
		{
			if (this->running)
			{
				//debug
				{
					std::string a = "BossState : LongJump";
					UIManager::GetInstance()->SetTextUIText("BossState", a);
				}
				Vector currPos = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
				if (this->attackTime - this->progressTime > 0.001f && GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->ParabolicMove();
					this->progressTime += Timer::GetInstance()->DeltaTime();

					return true;
				}
				else
				{
					Idle::afterAttack = true;
					this->running = false;
					this->initialPos = Vector::Zero;
					this->targetPos = Vector::Zero;

					Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
					Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

					boss.y = 0.f;
					GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->SetPosition(boss, Space::WORLD);

					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;

					SoundManager::GetInstance()->PlayEffect("BossJumpLand");
					//SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(300.f);

					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossLongJump");

				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
				Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
				Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

				this->initialPos = boss;
				this->targetPos = player;

				this->dx = ((player.x - boss.x) + this->additionalDistance) / this->attackTime;
				this->dz = ((player.z - boss.z) + this->additionalDistance) / this->attackTime;

				GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
				SoundManager::GetInstance()->PlayEffect("BossJumpStart");

				return true;
			}

		}

		void LongJumpAttack::ParabolicMove()
		{
			Vector dir = Vector::Zero;
			dir.x = this->dx * this->progressTime;
			dir.z = this->dz * this->progressTime;
			dir.y = -20.f / this->attackTime / this->attackTime * pow(this->attackTime / 2.f - this->progressTime, 2) + 5.f;

			auto boss = GameState::GetInstance()->GetBoss();
			boss->GetComponent<Transform>()->SetPosition(this->initialPos + dir, Space::WORLD);
		}

		bool CloseRange::Run()
		{
			if (__super::Run())
			{
				return true;
			}
			Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
			if (Vector3Length(player - boss) <= this->closeAttackDist)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool JumpRange::Run()
		{
			if (__super::Run())
			{
				return true;
			}
			Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
			if (Vector3Length(player - boss) <= this->jumpAttackDist && Vector3Length(player - boss) >= this->moveDist)
			{
				return true;
			}
			else
			{
				return false;
			}

		}

		bool DashRange::Run()
		{
			if (__super::Run())
			{
				return true;
			}
			Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();

			if (Vector3Length(player - boss) >= this->dashAttackDist)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool MoveRange::Run()
		{
			Vector boss = GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->GetWorldPosition();
			Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

			float distance = Vector3Length(boss - player);

			if (true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool Move::Run()
		{
			SoundManager::GetInstance()->PlayBossWalkSound("BossWalk", false);

			auto bossGameObject = GameState::GetInstance()->GetBoss();

			if (bossGameObject->GetComponent<Animator>()->IsPlaying() == false)
			{
				bossGameObject->GetComponent<Animator>()->Play("BossRun");
			}

			auto boss = bossGameObject->GetComponent<Transform>()->GetWorldPosition();
			auto player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

			bossGameObject->GetComponent<Transform>()->LookAt(player);

			if (Vector3Equal(player - boss, Vector::Zero) != true)
				bossGameObject->GetComponent<Transform>()->SetPosition(boss + VecotrMultFloat(Vector3Normalize(player - boss), this->speed * Timer::GetInstance()->DeltaTime()));

			return true;
		}

		bool Idle::Run()
		{
			if (this->afterAttack == true)
			{
				if (this->running)
				{
					//debug
					{
						std::string a = "BossState : Idle";
						UIManager::GetInstance()->SetTextUIText("BossState", a);
					}
					if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying() == false)
					{
						this->afterAttack = false;
						this->running = false;
						return false;
					}
					else
					{
						return true;
					}
				}
				else
				{
					this->running = true;
					GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossIdle");

					Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
					GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(player);

					return true;
				}
			}
			else
			{
				return false;
			}
		}

		bool CheckDeath::Run()
		{
			if (__super::Run())
			{
				return true;
			}
			if (GameState::GetInstance()->GetBoss()->GetComponent<BossTree>()->hp <= 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool Stun::Run()
		{

			if (this->running)
			{
				//debug
				{
					std::string a = "BossState : Stun";
					UIManager::GetInstance()->SetTextUIText("BossState", a);
				}
				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					return true;
				}
				else
				{
					this->running = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossStun");
				return true;
			}
		}

		bool DashAttack::Run()
		{
			if (this->running)
			{
				//debug
				{
					std::string a = "BossState : DashAttack";
					UIManager::GetInstance()->SetTextUIText("BossState", a);
				}

				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->DashMove();
					return true;
				}
				else
				{
					this->running = false;
					Idle::afterAttack = true;
					DashAttackStart::dashAttackRunning = false;
					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossDash");
				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());

				GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;

				return true;
			}
		}

		void DashAttack::DashMove()
		{
			auto bossTransform = GameState::GetInstance()->GetBoss()->GetComponent<Transform>();
			bossTransform->SetPosition(bossTransform->GetWorldPosition() + VecotrMultFloat(bossTransform->GetLook(), this->dashSpeed * Timer::GetInstance()->DeltaTime())); // 일단 5정도로 두자
		}


		bool DashAttackStart::Run()
		{
			if (this->dashAttackRunning)
			{
				return false;
			}

			if (this->running)
			{
				//debuggks50qjsehffuqhk
				{
					std::string a = "BossState : DashAttackStart";
					UIManager::GetInstance()->SetTextUIText("BossState", a);
				}

				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->DashStartMove();
					return true;
				}
				else
				{
					this->running = false;
					this->dashAttackRunning = true;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossDashStart");
				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
				SoundManager::GetInstance()->PlayEffect("BossDashStart");

				return true;
			}
		}
		void DashAttackStart::DashStartMove()
		{
		}

		bool StunCondition::Run()
		{
			if (__super::Run())
			{
				return true;
			}
			if (GameState::GetInstance()->GetBoss()->GetComponent<BossTree>()->collisionEnterToWall)
			{
				GameState::GetInstance()->GetBoss()->GetComponent<BossTree>()->collisionEnterToWall = false;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool Death::Run()
		{
			if (this->running)
			{
				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying() == false) // 일단 무한루프
				{
					GameState::GetInstance()->GameClear();
					this->running = false;
					GameState::GetInstance()->GetBoss()->GetComponent<BossTree>()->hp = 0.f;
					
					//엔딩 크레딧 띄우기
					SceneManager::GetInstance()->LoadScene(TEXT("EndingScene"));
					
					//SceneManager::GetInstance()->LoadScene(TEXT("MainMenu"));

					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossDeath");
				SoundManager::GetInstance()->PlayEffect("BossDeath");
				SoundManager::GetInstance()->PlayBGM("GameClear");

				return true;
			}
		}

		bool ShockWave::Run()
		{
			static bool firstAttack = false;
			static bool secondAttack = false;
			static bool thirdAttack = false;
			static bool fourthAttack = false;
			static bool fifthAttack = false;
			static bool sixAttack = false;
			if (this->running)
			{

				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > 0.6f && !firstAttack)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetChild(TEXT("BossShockWave"))->GetComponent<SpriteAnimator>()->SetLoop(true);

						firstAttack = true;

					}
					else if (this->progressTime > 1.4f && !secondAttack)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
						secondAttack = true;
					}
					else if (this->progressTime > 2.2f && !thirdAttack)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;

						thirdAttack = true;
					}
					else if (this->progressTime > 3.0f && !fourthAttack)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;

						fourthAttack = true;
					}
					else if (this->progressTime > 3.7f && !fifthAttack)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;

						fifthAttack = true;
					}
					else if (this->progressTime > 4.4f && !sixAttack)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;

						sixAttack = true;
					}
					else
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					}

					return true;
				}
				else
				{
					this->running = false;
					this->attacked = false;
					Idle::afterAttack = true;
					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetChild(TEXT("BossShockWave"))->GetComponent<SpriteAnimator>()->SetLoop(false);

					firstAttack = false;
					secondAttack = false;
					thirdAttack = false;
					fourthAttack = false;
					fifthAttack = false;
					sixAttack = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->attacked = false;
				this->progressTime = 0.f;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossShockWave");
				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());

				GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
				return true;
			}
		}

		bool CheckIdle::Run()
		{
			if (__super::Run())
			{
				return true;
			}
			else
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dist(0, 100);
				if (dist(gen) % 15 == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

		}

		bool CheckHitCount::Run()
		{
			auto boss = GameState::GetInstance()->GetBoss()->GetComponent<BossTree>();
			if (__super::Run())
			{
				return true;
			}

			if (boss->hitCount >= 6)
			{
				boss->hitCount = 0;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool CounterAttack::Run()
		{
			if (this->running)
			{
				//debug
				{
					std::string a = "BossState : CounterAttack";
					UIManager::GetInstance()->SetTextUIText("BossState", a);
				}

				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (!this->attacked && this->progressTime > 1.f)
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = true;
						SoundManager::GetInstance()->PlayEffect("BossAttack1");
						this->attacked = true;
					}
					else
					{
						GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					}
					return true;
				}
				else
				{
					this->running = false;
					this->attacked = false;
					Idle::afterAttack = true;
					GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;

				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossCounterAttack");
				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
				GameState::GetInstance()->GetBoss()->GetChild(TEXT("BossCloseAttack"))->GetComponent<BossAttackMonoBehaviour>()->attacking = false;


				return true;
			}
		}

		bool EnterPhase2::Run()
		{
			static bool triger = true;
			static float time = 0.0f;

			time += Timer::GetInstance()->DeltaTime();

			if (time >= 0.3f)
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(200.0f);

				time -= 0.3f;
			}

			if (__super::Run())
			{
				return true;
			}

			if (triger)
			{
				triger = false;

				return true;
			}
			else
			{
				return false;
			}
		}
		bool Phase2Roar::Run()
		{
			if (this->running)
			{
				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					return true;
				}
				else
				{
					this->running = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				this->progressTime = 0.f;
				SoundManager::GetInstance()->PlayEffect("BossRoar");
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossRoar");

				return true;
			}

		}
		bool EnterPhase3::Run()
		{
			static bool triger = true;
			static float time = 0.0f;

			time += Timer::GetInstance()->DeltaTime();

			if (time >= 0.3f)
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(200.0f);

				time -= 0.3f;
			}

			if (__super::Run())
			{
				return true;
			}

			if (triger)
			{
				triger = false;

				return true;
			}
			else
			{
				return false;
			}
		}
		bool Barrage::Run()
		{
			if (this->running)
			{
				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > this->nextFireTime && this->currIndex <= this->maxIndex)
					{
						SoundManager::GetInstance()->PlayEffect("BossAttack1");
						GameState::GetInstance()->GetBoss()->GetComponent<BossTree>()->ActivateBullet(this->currIndex);
						this->nextFireTime += this->increment;
						this->currIndex += 1;
					}
					else
					{

					}

					return true;
				}
				else
				{
					this->running = false;
					this->nextFireTime = 0.5f;
					this->currIndex = 0;
					Idle::afterAttack = true;
					GameState::GetInstance()->GetBoss()->GetComponent<BossTree>()->DeactivateBulletAll();
					return false;
				}
			}
			else
			{
				this->running = true;
				this->nextFireTime = 0.5f;
				this->progressTime = 0.f;
				this->currIndex = 0;
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossBarrage");
				GameState::GetInstance()->GetBoss()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());

				return true;
			}
		}
		bool Phase3Roar::Run()
		{
			if (this->running)
			{
				if (GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->IsPlaying())
				{
					return true;
				}
				else
				{
					this->running = false;
					return false;
				}
			}
			else
			{
				this->running = true;
				SoundManager::GetInstance()->PlayEffect("BossRoar");
				GameState::GetInstance()->GetBoss()->GetComponent<Animator>()->Play("BossRoar");

				return true;
			}
		}

	}
}