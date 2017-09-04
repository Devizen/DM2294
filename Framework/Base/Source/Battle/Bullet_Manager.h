#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <vector>
using std::vector;

class CBullet;

class CBullet_Manager
{
	static CBullet_Manager* s_instance;
public:
	static CBullet_Manager* GetInstance(void);

	void AddBullet(int _amount);
	void Render(void);
	void Update(double dt);
	vector<CBullet*>*GetBulletList(void);
private:
	vector<CBullet*>*bulletList;
protected:
};

#endif