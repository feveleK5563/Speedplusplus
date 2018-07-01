#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>

enum struct TaskState
{
	Ready,		//準備
	Active,		//通常
	Stop,		//停止
	Kill,		//消滅要請
	Delete,		//消滅
}; 

class TaskAbstract
{
public:
	const std::string	groupName;	//グループ名
	float				priority;	//描画優先度
	TaskState			state;		//状態

	//コンストラクタ
	TaskAbstract(	const std::string& groupName,
					float priority,
					TaskState state = TaskState::Active);

	//デストラクタ
	virtual ~TaskAbstract() {};
	
	virtual void Initialize() = 0;	//初期化処理
	virtual void Finalize() = 0;	//終了処理
	virtual void Update() = 0;		//更新
	virtual void Draw() = 0;		//描画

protected:
	void KillMe();	//自分を殺す
};