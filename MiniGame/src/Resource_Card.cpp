#include "Resource_Card.h"

namespace CardR
{
	std::weak_ptr<Resource> Resource::instance;
	//----------------------------------------------
	//リソースのコンストラクタ
	Resource::Resource()
	{
		imageName = "Card";
		Image::imageLoader.LoadDivImage(imageName, "data/image/PlayingCards.png", 55, 13, 5, 32, 48);
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
}