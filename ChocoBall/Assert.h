#pragma once

/*!
*@brief	アサート
*@param[in]	flag	偽のときアサートが発生します。
*/
static inline void Assert(bool flag)
{
	if (!flag){
		MessageBox(nullptr, "エラーが発生しました", "アサート", MB_OK);
		std::abort();
	}
}

#ifdef _DEBUG
#define 	CH_ASSERT( flg)	Assert( flg)
#else
#define 	CH_ASSERT(flg)
#endif 
