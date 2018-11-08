#include "f.h"


/**
 * 链接报错的原因是multiple definition of "i" ,应该是引用了两次f.h文件吧。
 * 不过这倒是给提供了一个思路，加上const，常量的话，就类加载的时候必须初始化，并且是全局唯一变量。
 * @return
 */

int main()
{
	f();

	return 0;
}
