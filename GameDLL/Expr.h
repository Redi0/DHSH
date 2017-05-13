#ifndef __DHSH_GAMEDLL_EXPRESSION_EXPR_H__
#define __DHSH_GAMEDLL_EXPRESSION_EXPR_H__

#include "GameBase.h"
#include <MyTools/CLExpression.h>

class CExpr : public MyTools::CExprFunBase, virtual public MyTools::CClassInstance<CExpr>
{
public:
	CExpr();
	virtual ~CExpr();

	virtual VOID Release();

	virtual std::vector<MyTools::ExpressionFunPtr>& GetVec();

private:
	virtual VOID Help(CONST std::vector<std::wstring>& VecParm);

	virtual VOID AroundObject(CONST std::vector<std::wstring>& VecParm);
};




#endif // !__DHSH_GAMEDLL_EXPRESSION_EXPR_H__