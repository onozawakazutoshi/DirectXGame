#include "skydome.h"

void skydome::Initalize(Model* model, uint32_t textureHandle) {
	textureHandle_ = textureHandle;
	model_ = model;
    
	worldTransform_.Initialize();
	
}

void skydome::Update() {}

void skydome::Draw(ViewProjection& viewprojection) { 
	
	model_->Draw(worldTransform_, viewprojection); 
}
