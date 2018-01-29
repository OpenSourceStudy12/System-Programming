#ifndef NONCPYABLE_H
#define NONCPYABLE_H

class noncpyable {
public:
	noncpyable() = default;
	noncpyable(const noncpyable& n) = delete;
	noncpyable& operator=(const noncpyable& ) = delete;
};

#endif
