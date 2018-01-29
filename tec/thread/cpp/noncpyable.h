#ifndef __NONCPYABLE_H
#define __NONCPYABLE_H

class noncpyable
{
public:
	noncpyable() = default;
	noncpyable(const noncpyable& n) = delete;
	noncpyable operator=(const noncpyable &n) = delete;
};

#endif
