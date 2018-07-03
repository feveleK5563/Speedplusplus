#include "Task_Back.h"
#include "DxLib.h"
#include "SystemDefine.h"

namespace Back
{
	std::weak_ptr<Resource> Resource::instance;
	//----------------------------------------------
	//リソースのコンストラクタ
	Resource::Resource()
	{
		imageName[0] = "Back";
		Image::imageLoader.LoadOneImage(imageName[0], "data/image/Back.png");
		imageData[0] = Image::imageLoader.GetImageData(imageName[0]);

		imageName[1] = "CardFrame";
		Image::imageLoader.LoadOneImage(imageName[1], "data/image/CardFrame.png");
		imageData[1] = Image::imageLoader.GetImageData(imageName[1]);
	}
	//----------------------------------------------
	//リソースのデストラクタ
	Resource::~Resource()
	{
		for (int i = 0; i < resourceNum; ++i)
		{
			Image::imageLoader.DeleteImageData(imageName[i]);
		}
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
	Task::Task():
		TaskAbstract(defGroupName, defPriority),
		res(Resource::Create()),
		imageBack(res->imageData[0], false),
		imageFrame(res->imageData[1], true)
	{ 
		Initialize();
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	const std::shared_ptr<const Task> Task::Create()
	{
		std::shared_ptr<Task> task = std::make_shared<Task>();
		TS::taskSystem.RegistrationTask(task);

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
		
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		imageBack.DrawOne(
			Math::Vec2(0, 0),
			1.f,
			0.f,
			false,
			0,
			Color(255, 255, 255, 255)
		);

		imageFrame.DrawOne(
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
			8.f,
			0.f,
			false,
			0,
			Color(255, 255, 255, 255)
		);
	}
}