#include "Task_Back.h"
#include "DxLib.h"
#include "ImageLoader.h"
#include "SystemDefine.h"

namespace Back
{
	std::weak_ptr<Resource> Resource::instance;
	//----------------------------------------------
	//リソースのコンストラクタ
	Resource::Resource()
	{
		imageBack = "Back";
		Image::imageLoader.LoadOneImage(imageBack, "data/image/Back.png");
		imageDataBack = Image::imageLoader.GetImageData(imageBack);

		imageFrame = "CardFrame";
		Image::imageLoader.LoadOneImage(imageFrame, "data/image/CardFrame.png");
		imageDataFrame = Image::imageLoader.GetImageData(imageFrame);
	}
	//----------------------------------------------
	//リソースのデストラクタ
	Resource::~Resource()
	{
		Image::imageLoader.DeleteImageData(imageBack);
		Image::imageLoader.DeleteImageData(imageFrame);
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
		imageBack(res->imageDataBack, Math::Vec2(0, 0)),
		imageFrame(res->imageDataFrame, Math::Vec2(res->imageDataFrame.rect.w / 2.f, res->imageDataFrame.rect.h / 2.f))
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