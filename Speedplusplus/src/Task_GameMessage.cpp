#include "Task_GameMessage.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "GameDefine.h"

namespace GameMessage
{
	std::weak_ptr<Resource> Resource::instance;
	//----------------------------------------------
	//リソースのコンストラクタ
	Resource::Resource()
	{
		imageName = "MessageImg";
		Image::imageLoader.LoadDivImage(imageName, "data/image/GameMessage.png", 2, 1, 2, 1280, 56);
		imageData = Image::imageLoader.GetImageData(imageName);
	}
	//----------------------------------------------
	//リソースのデストラクタ
	Resource::~Resource()
	{
		Image::imageLoader.DeleteImageData(imageName);
	}
	//----------------------------------------------
	//リソースの生成
	std::shared_ptr<Resource> Resource::Create()
	{
		auto sp = instance.lock();
		if (!sp)
		{
			sp = std::make_shared<Resource>();
			instance = sp;
		}
		return sp;
	}

	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(MessageType type, int waitTime):
		TaskAbstract(defGroupName, Priority::message),
		res(Resource::Create()),
		easeMover(	Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 50.f),
					Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
					1.f, 1.f, 0.f, 0.f),
		imageDrawer(res->imageData, true),
		type(type),
		progress(0),
		waitTimeCnt(waitTime)
	{
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(MessageType type, int waitTime)
	{
		std::shared_ptr<Task> task = 
			std::make_shared<Task>(type, waitTime);
		TS::taskSystem.RegistrationTask(task);

		task->Initialize();
		return task;
	}

	//◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆
	//◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

	//----------------------------------------------
	//初期化処理
	//----------------------------------------------
	void Task::Initialize()
	{

	}

	//----------------------------------------------
	//終了処理
	//----------------------------------------------
	void Task::Finalize()
	{

	}

	//----------------------------------------------
	//更新
	//----------------------------------------------
	void Task::Update()
	{
		bool isMoveEnd = easeMover.Update(30.f);
		switch (progress)
		{
		case 0:
			if (isMoveEnd)
				++progress;
			break;

		case 1:
			waitTimeCnt.Run();
			if (waitTimeCnt.IsTimeEnd())
			{
				easeMover.SetEndMove(
					Math::Vec2(SystemDefine::windowSizeX / 2.f, -50.f),
					1.f, 0.f);
				++progress;
			}
			break;

		case 2:
			if (isMoveEnd)
				KillMe();
			break;
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		imageDrawer.DrawOne(
			easeMover.GetPos(),
			easeMover.GetScale(),
			easeMover.GetAngle(),
			false,
			(int)type,
			Color(255, 255, 255, 255)
		);
	}
}