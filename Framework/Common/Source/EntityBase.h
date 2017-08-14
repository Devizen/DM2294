#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"
#include "Collider\Collider.h"

class EntityBase : public CCollider
{
public:
	enum ITEM
	{
		NONE = 0,
		BULLET,
		HEALTH
	};

	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetRotate(const float& _value) { rotate = _value; };
	inline float GetRotate() { return rotate; };

	inline void SetRotationAxis(const Vector3& _value) { rotationAxis = _value; };
	inline Vector3 GetRotationAxis() { return rotationAxis; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	inline void SetLight(const bool& _value) { light = _value; };
	inline float GetLight() { return light; };

	inline void SetItem(const ITEM& _value) { item = _value; };
	inline ITEM GetItem() { return item; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);

protected:
	Vector3 position;
	float rotate;
	Vector3 rotationAxis;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;
	bool light;
	ITEM item;
};

#endif // ENTITY_BASE_H