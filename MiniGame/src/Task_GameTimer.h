#pragma once
#include "TaskSystem.h"
#include "TimeCounter.h"
#include "CardCounter.h"
#include "Task_GameMessage.h"

enum struct GameState
{
	Ready	= 180,
	Game	= 3600,
	GameEnd,
	Result,
	End,
};

namespace GameTimer
{
	const std::string	defGroupName("�Q�[���^�C�}�[");	//�O���[�v��

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		GameState gameState;
		std::unique_ptr<CardCounter> cardCnt[2];
		TimeCounter timeCnt;
		TimeCounter cardAppTimeCnt;
		std::shared_ptr<GameMessage::Task> gameMessage;

	public:
		Task();		//�R���X�g���N�^
		~Task();	//�f�X�g���N�^
		static std::shared_ptr<Task> Create();	//�^�X�N�̐���

		void Initialize();			//����������
		void Finalize() override;	//�I������
		void Update() override;		//�X�V
		void Draw() override;		//�`��

	private:
		void CountUpdate(bool isCntStart);

	public:
		const GameState& GetTimeState() const;
	};
}