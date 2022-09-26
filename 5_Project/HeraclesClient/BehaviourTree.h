#pragma once
#include <vector>
#include <random>

namespace Heracles
{
	enum class ActionType
	{
		IDLE,
		NONE
	};

	class Node
	{
	protected:
		int priority;
		bool running = false;
		friend class Condition;
		friend class Selector;

	public:
		virtual bool Run() = 0;
		inline int GetPriority() { return this->priority; }
		inline void SetPriority(int i) { this->priority = i; }
	};

	class CompositeNode : public Node
	{
	protected:
		std::vector<std::shared_ptr<Node>> children;
	public:
		const std::vector<std::shared_ptr<Node>>& getChildren() const { return this->children; }
		virtual void AddChild(std::shared_ptr<Node> child) { children.emplace_back(child); }
		//void SortChildren(bool(*CompareFunction)(std::shared_ptr<Node>)) { std::sort(children.begin(), children.end(), CompareFunction); }
	};

	class Selector : public CompositeNode
	{
	public:
		virtual bool Run() override;
	};

	class RandomSelector : public Selector
	{
	private:
		std::shared_ptr<Node> SelectAction();
		std::shared_ptr<Node> selectedAction = 0;
		int distRange;
		int weight;

	public:
		RandomSelector(int distRange, int weight);
		virtual bool Run() override;
	};


	class Sequencer : public CompositeNode
	{
	public:
		virtual bool Run() override;
	};

	class Action : public Node
	{
	protected:
		ActionType actionType;
		float timer = 0.f;
		float progressTime = 0.f;
	public:
		virtual bool Run() override;

		inline ActionType GetActionType() { return this->actionType; }
		inline void SetActionType(ActionType type) { this->actionType = type; }
	};

	class Condition : public Node
	{
	protected:
		std::shared_ptr<Node> nextNode;
	public:
		Condition(std::shared_ptr<Node> next = nullptr) :nextNode(next) {}
		virtual bool Run() override;

	};

	class Decorator : public Node
	{
	private:
		std::shared_ptr<Node> child;
	public:
		std::shared_ptr<Node> GetChild() { return this->child; }
		void SetChild(std::shared_ptr<Node> node) { this->child = node; }
	};

	class Succeeder : public Decorator
	{
	public:
		virtual bool Run() override;
	};

	class Failer : public Decorator
	{
	public:
		virtual bool Run() override;
	};

	class Inverter : public Decorator
	{
	public:
		virtual bool Run() override;
	};

}

