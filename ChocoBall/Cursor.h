#pragma once
#include "C2DImage.h"
#include "DirectInput.h"

#define MAX_COMMAND_NUM m_num
enum COMMAND_SELECT { COMMAND1 = 0, COMMAND2, COMMAND3, COMMAND4};

class CCursor : public C2DImage
{
public:
	CCursor();
	~CCursor();
	void Initialize()override;
	void Update()override;
	void Setnum(short);
	void Select();
	virtual void Index();
private:
	short m_move;
	short m_dir;
	short m_num;
protected:
	CDirectInput m_Input;
	short m_selectIndex;
};
