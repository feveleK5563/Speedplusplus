#include "Task_GameTimer.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "Task_Result_Ranking.h"
#include "Task_Result_VSMode.h"

namespace GameTimer
{
	//
	//

	//----------------------------------------------
	//^XNΜRXgN^
	Task::Task(Mode mode):
		TaskAbstract(defGroupName, Priority::countCard),
		gameState(GameState::Ready),
		waitCountDown	(180),
		waitGame		(60 * 60),
		waitGameEnd		(180),
		waitResult		(20),
		mode(mode)
	{ 
		
	}
	//----------------------------------------------
	//^XNΜfXgN^
	Task::~Task()
	{

	}
	//----------------------------------------------
	//^XNΜΆ¬
	std::shared_ptr<Task> Task::Create(Mode mode)
	{
		std::shared_ptr<Task> task = std::make_shared<Task>(mode);
		TS::taskSystem.RegistrationTask(task);

		task->Initialize();
		return task;
	}

	//
	//

	//----------------------------------------------
	//ϊ»
	//----------------------------------------------
	void Task::Initialize()
	{
		gameMessage = GameMessage::Task::Create(MessageType::Ready, 50);

		switch (mode)
		{
		case Mode::Single:
			cardCnt[0] = std::make_unique<CardCounter>(
				0,
				Math::Vec2(250, SystemDefine::windowSizeY + 200.f),
				Math::Vec2(120, 150), 0.5f);

			cardCnt[1] = std::make_unique<CardCounter>(
				(int)Suit::Etc_RedNum,
				Math::Vec2(120, SystemDefine::windowSizeY + 200.f),
				Math::Vec2(250, 150), 0.5f);
			break;

		case Mode::VS:
			cardCnt[0] = std::make_unique<CardCounter>(
				0,
				Math::Vec2(SystemDefine::windowSizeX / 2.f + 50.f, -200.f),
				Math::Vec2(SystemDefine::windowSizeX / 2.f - 40.f, 80.f), 0.3f);

			cardCnt[1] = std::make_unique<CardCounter>(
				(int)Suit::Etc_RedNum,
				Math::Vec2(SystemDefine::windowSizeX / 2.f - 50.f, -200.f),
				Math::Vec2(SystemDefine::windowSizeX / 2.f + 40.f, 80.f), 0.3f);
			break;

		default:
			KillMe();
			return;
		}
	}

	//----------------------------------------------
	//IΉ
	//----------------------------------------------
	void Task::Finalize()
	{

	}

	//----------------------------------------------
	//XV
	//----------------------------------------------
	void Task::Update()
	{
		CountUpdate(gameState == GameState::CountDown ||
					gameState == GameState::Game);

		switch (gameState)
		{
		case GameState::Ready:	//Q[JnO
			if (gameMessage->state != TaskState::Active)
			{
				gameState = GameState::CountDown;
				timeCnt.SetEndTime(waitCountDown);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::CountDown:	//Q[JnΌOΜJEg
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::Game;
				timeCnt.SetEndTime(waitGame);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::Game:		//Q[
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::GameEnd;
				gameMessage = GameMessage::Task::Create(MessageType::Finish, 90);

				timeCnt.SetEndTime(waitGameEnd);
				timeCnt.ResetCntTime();
			}
			if (Button::PushStartReset())
			{
				gameState = GameState::End;
			}
			break;

		case GameState::GameEnd:	//Q[IΉ
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::Result;
				switch (mode)
				{
				case Mode::Single:
					resultTaskState = &Result_Ranking::Task::Create()->state;
					break;

				case Mode::VS:
					resultTaskState = &Result_VSMode::Task::Create()->state;
					break;

				default:
					KillMe();
					break;
				}

				timeCnt.SetEndTime(-1);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::Result:		//ΚζΚ
			if (*resultTaskState == TaskState::Kill)
			{
				gameState = GameState::End;
			}
			break;

		case GameState::End:
			KillMe();
			break;
		}
	}

	//----------------------------------------------
	//`ζ
	//----------------------------------------------
	void Task::Draw()
	{
		for (int i = 0; i < 2; ++i)
		{
			cardCnt[i]->Draw();
		}
	}

	//----------------------------------------------
	//timeCntΜXVΖ\AκΜΚΜvZ
	void Task::CountUpdate(bool showCnt)
	{
		timeCnt.Run();

		cardAppTimeCnt.Run();

		int tenPlace = 0, unitPlace = 0;

		if (showCnt)
		{
			tenPlace = (timeCnt.GetRemainingTime() + 60) / 60 / 10;
			unitPlace = (timeCnt.GetRemainingTime() + 60) / 60 % 10;
		}

		cardCnt[0]->Update(tenPlace, true);

		if (gameState != GameState::Ready ||
			cardAppTimeCnt.GetNowCntTime() > 5)
		{
			//κΜΚΝAcθΤͺ3ΘΊΜκΤΙΘι
			if (tenPlace == 0 && unitPlace <= 3)
				cardCnt[1]->Update((int)Suit::Etc_RedNum + unitPlace, true);
			else
				cardCnt[1]->Update(unitPlace, true);
		}
	}

	//----------------------------------------------
	//σΤΜζΎ
	const GameState& Task::GetGameState() const
	{
		return gameState;
	}
}