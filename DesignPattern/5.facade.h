#ifndef _FACADE_H_
#define _FACADE_H_

#include "example.h"

#include <string>

class FacadePattern : public Example
{
public:
	FacadePattern() = default;
	~FacadePattern() = default;

	void on_update(float delta) override;

private:
	std::string str_text = u8"详见 framework/application.cpp 中的代码实现";

};

#endif // !_FACADE_H_
