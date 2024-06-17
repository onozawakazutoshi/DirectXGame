#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class RailCamera {
public:
	void Initialize(Vector3 radian);
	
	void Update();

	void Draw();

	ViewProjection& GetViewProjection();


private:
	
	WorldTransform worldTransform_;
	ViewProjection viewprojection_;
	


};
