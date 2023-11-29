#pragma once

#include "ViewProjection.h"
#include"WorldTransform.h"

//�Ǐ]�J����
class FollowCamera {
public:

	//������
	void Initialize();

	//�X�V
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProJection() { return viewProjection_; }

private:
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//�Ǐ]�Ώ�
	const WorldTransform* target_ = nullptr;

};
